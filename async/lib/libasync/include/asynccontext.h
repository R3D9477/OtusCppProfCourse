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

#include <filesystem>
namespace fs = std::filesystem;

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
                    if (launch_fwriter(m_worker_file1, m_worker_file1_in_progress, bulk_buf)) break;
                    if (launch_fwriter(m_worker_file2, m_worker_file2_in_progress, bulk_buf)) break;
                    std::this_thread::sleep_for(1ms);
                }
                print_bulk(m_log_buf, std::move(bulk_buf));
            }

            std::this_thread::sleep_for(1ms);
        }
    }

    bool launch_fwriter(std::thread& fthr, std::atomic_bool& fthr_busy, std::vector<std::string> data)
    {
        if (!fthr_busy.load())
        {
            if (fthr.joinable())
                fthr.join();
            fthr = std::thread([this,&fthr_busy,data](){
                fthr_busy.store(true);
                fwriter_proc(data);
                fthr_busy.store(false);
            });
            return true;
        }
        return false;
    }

    void fwriter_proc(std::vector<std::string> bulk)
    {
        std::ofstream bulk_f;
        std::string bulk_f_name = "bulk" + std::to_string(duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count());

        {
            std::lock_guard fname_locker(m_fname_mutex);

            if (fs::exists(bulk_f_name+".log"))
            {
                bulk_f_name += "-";
                size_t f_name_index = 0;
                for (; f_name_index < 1000000 && fs::exists(bulk_f_name+std::to_string(f_name_index)+".log") ; ++f_name_index)
                    ;
                if (f_name_index == 1000000)
                    throw "Unable to create unique file!";
                else
                    bulk_f_name += std::to_string(f_name_index);
            }

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

