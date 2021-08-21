#pragma once

#include <mutex>

#include "bulkconnection.h"

class BulkServer
{
    boost::asio::io_service& m_io_service;
    tcp::acceptor m_acceptor;

    short m_bulk_size{3};
    async::handle_t m_static_context{nullptr};
    std::mutex m_static_context_mux;

public:

    BulkServer(boost::asio::io_service& io_service, short port, short bulk_size):
        m_io_service{io_service},
        m_acceptor{io_service, tcp::endpoint(tcp::v4(), port)},
        m_bulk_size{bulk_size}
    {
        create_static_context();
        start_accept();
    }

    ~BulkServer () { destroy_static_context(); }

private:

    void start_accept()
    {
        BulkConnection* new_session = new BulkConnection(
            m_io_service,
            [this](char* data, size_t size)
            {
                std::lock_guard<std::mutex> lock{m_static_context_mux};
                if (m_static_context)
                    async::receive(m_static_context, data, size);
            }
        );

        m_acceptor.async_accept(
            new_session->socket(),
            boost::bind( &BulkServer::handle_accept, this, new_session, boost::asio::placeholders::error )
        );
    }

    void handle_accept(BulkConnection* new_session, const boost::system::error_code& error)
    {
        if (!error) new_session->start();
        else delete new_session;

        start_accept();
    }

    void create_static_context()
    {
        std::lock_guard<std::mutex> lock{m_static_context_mux};
        m_static_context = async::connect(m_bulk_size);
    }

    void destroy_static_context()
    {
        std::lock_guard<std::mutex> lock{m_static_context_mux};
        if (m_static_context)
            async::disconnect(m_static_context);
    }
};
