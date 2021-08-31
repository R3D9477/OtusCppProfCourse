#pragma once

#include <queue>
#include <mutex>
#include <thread>
#include <atomic>
#include <functional>
#include <condition_variable>
#include <iostream>

class AsyncPool
{
    static AsyncPool* m_instance;
    static std::once_flag m_instance_once;

    std::atomic_bool m_done{false};
    std::vector<std::thread> m_threads;

    std::mutex m_tasks_mtx;
    std::queue<std::function<void()>> m_tasks;
    std::condition_variable m_tasks_cv;

public:

    static AsyncPool& get()
    {
        std::call_once(m_instance_once, [](){ m_instance = new AsyncPool; });
        return *m_instance;
    }
    void add_task(std::function<void()> task)
    {
        {
            std::lock_guard<std::mutex> lock{m_tasks_mtx};
            m_tasks.push(task);
        }
        m_tasks_cv.notify_one();
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
            std::unique_lock<std::mutex> lock{m_tasks_mtx};
            m_tasks_cv.wait(lock, [this]() { return !m_tasks.empty(); });
            auto task = std::move(m_tasks.front());
            m_tasks.pop();
            lock.unlock();

            task();
        }
    }
};

