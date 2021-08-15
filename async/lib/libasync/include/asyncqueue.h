#pragma once

#include <queue>
#include <mutex>

template <typename T>
class AsyncQueue
{
    std::queue<T> m_q;
    std::mutex m_qmux;
public:
    void push(T elem)
    {
        std::lock_guard<std::mutex> lock{m_qmux};
        m_q.push(elem);
    }
    bool try_pop(T& t)
    {
        std::lock_guard<std::mutex> lock{m_qmux};
        if (!m_q.empty())
        {
            t = std::move(m_q.front());
            m_q.pop();
            return true;
        }
        return false;
    }
};
