#pragma once

#include <mutex>

#include "bulkconnection.h"

class BulkServer
{
    tcp::acceptor m_acceptor;

    short m_bulk_size{3};
    async::handle_t m_static_context{nullptr};
    std::mutex m_static_context_mux;

public:

    BulkServer(boost::asio::io_context& io_context, short port, short bulk_size):
        m_acceptor{io_context, tcp::endpoint(tcp::v4(), port)},
        m_bulk_size{bulk_size}
    {
        create_static_context();
        start_accept();
    }

    ~BulkServer () { destroy_static_context(); }

private:

    void start_accept()
    {
        m_acceptor.async_accept(
            [this](const boost::system::error_code& error, tcp::socket socket)
            {
                if (!error)
                {
                    std::make_shared<BulkConnection>(
                        std::move(socket),
                        [this](char* data, size_t size)
                        {
                            std::lock_guard<std::mutex> lock{m_static_context_mux};
                            if (m_static_context)
                                async::receive(m_static_context, data, size);
                        }
                    )->start();
                }
                start_accept();
            }
        );
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
