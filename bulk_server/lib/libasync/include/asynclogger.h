/** \file */

#pragma once

#include <mutex>
#include <thread>
#include <condition_variable>

#include <iostream>
#include <fstream>
#include <sstream>

#include <filesystem>
namespace fs = std::filesystem;

#include <chrono>
using namespace std::chrono;

#include "asynccontext.h"
#include "asyncpool.h"

#include "bulk.h"

class AsyncLogger
{
    std::atomic_bool m_done{false};
    std::mutex m_mtx;

    AsyncContext* m_context;
    bool m_context_ready = false;

    std::condition_variable m_context_cv;

    std::mutex m_fname_mutex;

    std::thread m_logger_thr;

    std::ostream* m_out_buf{&std::cout};

public:

    static AsyncLogger& get()
    {
        static AsyncLogger m_instance;
        return m_instance;
    }
    void push_context(AsyncContext* context)
    {
        std::lock_guard<std::mutex> lock{m_mtx};
        m_context = std::move(context);
        m_context_ready = true;
        m_context_cv.notify_one();
    }
    void set_output_buffer(std::ostream* out_buf)
    {
        m_out_buf = out_buf;
    }

private:

    AsyncLogger()
    {
        m_logger_thr = std::thread(&AsyncLogger::consumer_proc, this);
    }
    ~AsyncLogger()
    {
        m_context = nullptr;
        m_context_ready = true;
        m_context_cv.notify_one();

        m_done = true;
        m_logger_thr.join();
    }

    void consumer_proc()
    {
        while(!m_done)
        {
            std::unique_lock<std::mutex> lock{m_mtx};
            m_context_cv.wait(lock, [this]() { return m_context_ready; });

            if (m_context ? !m_context->empty() : false)
            {
                std::vector<std::string> bulk_buf;
                while (m_context->try_pop_front(bulk_buf))
                {
                    AsyncPool::get().add_task( [this,bulk_buf]() { fwriter_proc(bulk_buf); } );

                    if (m_out_buf)
                        print_bulk(*m_out_buf, std::move(bulk_buf));
                }
            }
            else m_done = true;

            m_context_ready = false;
            lock.unlock();
        }
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
