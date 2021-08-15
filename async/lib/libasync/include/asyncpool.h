#pragma once

#include <thread>
#include <atomic>
#include <functional>

#include "asyncqueue.h"

class AsyncPool
{
    static AsyncPool* m_instance;
    static std::once_flag m_instance_once;

    std::atomic_bool m_done{false};
    std::vector<std::thread> m_threads;
    AsyncQueue<std::function<void()>> m_tasks;

public:

    static AsyncPool& get()
    {
        std::call_once(m_instance_once, [](){ m_instance = new AsyncPool; });
        return *m_instance;
    }
    void add_task(std::function<void()> task)
    {
        m_tasks.push(task);
    }

private:

    AsyncPool()
    {
        for (int i = 0; i < 2; ++i)
            m_threads.emplace_back(std::thread(&AsyncPool::worker_proc, this));
    }
    ~AsyncPool()
    {
        m_done = true;
        for (auto& t: m_threads)
            if (t.joinable())
                t.join();
    }

    AsyncPool(const AsyncPool&) = delete;
    AsyncPool& operator=(const AsyncPool&) = delete;

    void worker_proc()
    {
        while(!m_done)
        {
            std::function<void()> task;
            if (m_tasks.try_pop(task)) task();
            else std::this_thread::yield();
        }
    }
};

