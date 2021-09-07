#pragma once

#include <iostream>
#include <functional>

#include <boost/bind.hpp>
#include <boost/asio.hpp>

#include "asynccontext.h"
#include "async.h"

using boost::asio::ip::tcp;

#define MAX_LENGTH 1024

class BulkConnection: public std::enable_shared_from_this<BulkConnection>
{
    tcp::socket m_sock;
    char m_data_buf[MAX_LENGTH];

    async::handle_t m_dynamic_context{nullptr};
    std::function<void(char*,size_t)> m_server_hanlder{nullptr};

public:

    BulkConnection(
        tcp::socket socket,
        decltype(m_server_hanlder) server_hanlder
    ):
        m_sock{std::move(socket)},
        m_server_hanlder{server_hanlder}
    { }

    ~BulkConnection()
    {
        if (m_dynamic_context)
            async::disconnect(m_dynamic_context);
    }

    tcp::socket& socket() { return m_sock; }

    void start()
    {
        auto self(shared_from_this());
        m_sock.async_read_some(
            boost::asio::buffer(m_data_buf, MAX_LENGTH),
            boost::bind(
                &BulkConnection::handle_read,
                self,
                boost::asio::placeholders::error,
                boost::asio::placeholders::bytes_transferred
            )
      );
    }

private:

    void handle_read(const boost::system::error_code& error, size_t bytes_transferred)
    {
        if (!error)
        {
            if (bytes_transferred == 1)
            {
                if (m_data_buf[0] == '{')
                {
                    m_dynamic_context = async::connect(1);
                }
                else if (m_data_buf[1] == '}')
                {
                    if (m_dynamic_context)
                    {
                        async::disconnect(m_dynamic_context);
                        m_dynamic_context = nullptr;
                    }
                }
            }

            if (m_dynamic_context) async::receive(m_dynamic_context, m_data_buf, bytes_transferred);
            else if (m_server_hanlder) m_server_hanlder(m_data_buf, bytes_transferred);
        }
    }
};
