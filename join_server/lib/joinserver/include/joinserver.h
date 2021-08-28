#pragma once

#include "joinconnection.h"

class JoinServer
{
    boost::asio::io_service& m_io_service;
    tcp::acceptor m_acceptor;

public:

    JoinServer(boost::asio::io_service& io_service, short port):
        m_io_service{io_service},
        m_acceptor{io_service, tcp::endpoint(tcp::v4(), port)}
    {
        start_accept();
    }

    ~JoinServer () { }

private:

    void start_accept()
    {
        JoinConnection* new_session = new JoinConnection(m_io_service);

        m_acceptor.async_accept(
            new_session->socket(),
            boost::bind( &JoinServer::handle_accept, this, new_session, boost::asio::placeholders::error )
        );
    }

    void handle_accept(JoinConnection* new_session, const boost::system::error_code& error)
    {
        if (!error) new_session->start();
        else delete new_session;

        start_accept();
    }
};
