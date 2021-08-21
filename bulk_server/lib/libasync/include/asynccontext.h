/** \file */

#pragma once

#include <vector>
#include <string>

#include "bulk.h"

class AsyncContext
{
    size_t m_block_size;
    std::queue<std::vector<std::string>> m_bulks;

public:

    AsyncContext(const size_t block_size): m_block_size{block_size} { }

    void receive(std::istream& in_buff)
    {
        get_bulks( in_buff, m_block_size, m_bulks );
    }

    bool try_pop_front(std::vector<std::string>& bulk_buf)
    {
        if (!m_bulks.empty())
        {
            bulk_buf = std::move(m_bulks.front());
            m_bulks.pop();
            return true;
        }
        return false;
    }

    bool empty() const { return m_bulks.empty(); }
};

