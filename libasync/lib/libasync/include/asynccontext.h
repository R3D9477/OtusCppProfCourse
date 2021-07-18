/** \file */

#pragma once

#include <tuple>
#include <vector>
#include <string>
#include <mutex>
#include <thread>
#include <atomic>

#include <iostream>
#include <fstream>
#include <sstream>

#include <chrono>
using namespace std::chrono;

#include "bulk.h"

class AsyncContext
{
public:

    AsyncContext(const size_t block_size, std::ostream& log_buf = std::cout): m_block_size{block_size}, m_log_buf{log_buf}
    {
        m_is_started.store(true);
        m_worker_log = std::thread([this]{ logger_proc(); });
    }

    ~AsyncContext()
    {
        m_is_started.store(false);
        if (m_worker_file1.joinable()) m_worker_file1.join();
        if (m_worker_file2.joinable()) m_worker_file2.join();
        if (m_worker_log.joinable())   m_worker_log.join();
    }

    void receive(std::istream& in_buff)
    {
        std::lock_guard m_bulks_locker(m_bulks_mutex);
        get_bulks( in_buff, m_block_size, m_bulks );
    }

private:

    size_t m_block_size;

    std::queue<std::vector<std::string>> m_bulks;
    std::mutex m_bulks_mutex;

    std::ostream& m_log_buf{std::cout};
    std::thread m_worker_log;

    std::atomic_bool m_worker_file1_in_progress{false};
    std::thread m_worker_file1;
    std::atomic_bool m_worker_file2_in_progress{false};
    std::thread m_worker_file2;

    std::mutex  m_fname_mutex;

    std::atomic_bool m_is_started{false};

    void logger_proc()
    {
        while (m_is_started.load())
        {
            std::vector<std::string> bulk_buf;

            {
                std::lock_guard m_bulks_lock(m_bulks_mutex);

                if (!m_bulks.empty())
                {
                    bulk_buf = std::move(m_bulks.front());
                    m_bulks.pop();
                }
            }

            if (!bulk_buf.empty())
            {
                while (m_is_started.load())
                {
                    if (!m_worker_file1_in_progress.load())
                    {
                        if (m_worker_file1.joinable())
                            m_worker_file1.join();
                        m_worker_file1 = std::thread([this,bulk_buf](){
                            m_worker_file1_in_progress.store(true);
                            fwriter_proc(bulk_buf);
                            m_worker_file1_in_progress.store(false);
                        });
                        break;
                    }
                    else if (!m_worker_file2_in_progress.load())
                    {
                        if (m_worker_file2.joinable())
                            m_worker_file2.join();
                        m_worker_file2 = std::thread([this,bulk_buf](){
                            m_worker_file2_in_progress.store(true);
                            fwriter_proc(bulk_buf);
                            m_worker_file2_in_progress.store(false);
                        });
                        break;
                    }
                }
                print_bulk(m_log_buf, std::move(bulk_buf));
            }

            std::this_thread::sleep_for(1ms);
        }
    }

    void fwriter_proc(std::vector<std::string> bulk)
    {
        std::ofstream bulk_f;
        std::string bulk_f_name = "bulk" + std::to_string(duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count());

        {
            std::lock_guard fname_locker(m_fname_mutex);

            /*if ((bulk_f_name + ".log" exists))
            {
                bulk_f_name += "-";
                size_t f_name_index = 0;
                for (; f_name_index < 1000000 && (bulk_f_name + to_string(f_name_index) + ".log" exists) ; ++f_name_index)
                    ;
                if (f_name_index == 1000000)
                    throw "Unable to create unique file!";
                else
                    bulk_f_name += std::to_string(f_name_index);
            }*/

            bulk_f.open(bulk_f_name + ".log");
        }

        if (bulk_f.is_open())
        {
            for (auto& cmd: bulk)
                bulk_f << cmd << std::endl;
            bulk_f.close();
        }
    }
};

