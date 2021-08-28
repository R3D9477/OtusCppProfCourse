#pragma once

#include <vector>
#include <sstream>
#include <iostream>
#include <functional>

#include <boost/algorithm/string.hpp>

#include <boost/bind.hpp>
#include <boost/asio.hpp>
using boost::asio::ip::tcp;

#include "dbmanager.h"

const std::string TABLE_HEADER{"id | "+LEFT_TABLE+"         | "+RIGHT_TABLE+"       \n---+-----------+---------"};

static auto split(const std::string& str, char delim)
{
    std::vector<std::string> result;
    std::stringstream ss{str};
    std::string item;

    while (getline(ss, item, delim))
    {
        boost::trim(item);
        result.push_back(item);
    }

    return result;
}

class JoinConnection: private DBResultHandler
{
    tcp::socket m_sock;
    char m_data_buf[1024];

public:

    JoinConnection(boost::asio::io_service& io_service): m_sock{io_service} { }

    ~JoinConnection() { }

    tcp::socket& socket() { return m_sock; }

    void start()
    {
        m_sock.async_receive(
            boost::asio::buffer(m_data_buf, 1024),
            boost::bind(
                &JoinConnection::handle_read,
                this,
                boost::asio::placeholders::error,
                boost::asio::placeholders::bytes_transferred
            )
        );
    }

private:

    void send_data(const std::string& snd_data)
    {
        std::cout << "< " << snd_data << std::endl;

        boost::system::error_code error;
        m_sock.write_some(boost::asio::buffer((snd_data+'\n').data(), snd_data.size()+1), error);
    }

    int handle_row(int id, std::string name_left, std::string name_right) override
    {
        std::stringstream row;

        row << ' ' << id << " | ";

        row << name_left;
        for (int i = name_left.size(); i < 9; ++i)
            row << ' ';

        row << " | " << name_right;

        send_data(row.str());

        return 0;
    }

    void handle_read(const boost::system::error_code& error, size_t bytes_transferred)
    {
        if (!error)
        {
            if (bytes_transferred > 0)
            {
                std::cout << "> " << m_data_buf;

                auto in_buf = split(m_data_buf, ' ');

                if (in_buf.size() > 0)
                {
                    std::transform(in_buf[0].begin(), in_buf[0].end(), in_buf[0].begin(), ::toupper);

                    if (in_buf[0] == "INSERT")
                    {
                        if (in_buf.size() > 3)
                            send_data( DBManager::get().insert(in_buf[1], std::stoi(in_buf[2]), in_buf[3]) ? "OK" : ("ERR duplicate "+in_buf[2]) );
                    }
                    else if (in_buf[0] == "TRUNCATE")
                    {
                        if (in_buf.size() > 1)
                            send_data( DBManager::get().truncate(in_buf[2]) ? "OK" : "ERR" );
                    }
                    else if (in_buf[0] == "INTERSECTION")
                    {
                        send_data(TABLE_HEADER);
                        if (!DBManager::get().intersection(this))
                            send_data("ERR");
                    }
                    else if (in_buf[0] == "SYMMETRIC_DIFFERENCE")
                    {
                        send_data(TABLE_HEADER);
                        if (!DBManager::get().symmetric_difference(this))
                            send_data("ERR");
                    }
                    else
                    {
                        send_data("ERR unknown command");
                    }
                }
                else send_data("ERR empty cmd sequence");
            }
        }
        else delete this;
    }
};
