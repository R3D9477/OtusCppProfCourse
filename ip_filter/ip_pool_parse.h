#pragma once

#include <vector>
#include <string>
#include <sstream>

auto split (const std::string& line, char seperator, bool remove_empty = false)
{
    std::vector<std::string> strs;

    std::string::size_type prev_pos = 0, pos = 0;

    while((pos = line.find(seperator, pos)) != std::string::npos)
    {
        std::string substring( line.substr(prev_pos, pos-prev_pos) );

        if (remove_empty)
        {
            std::stringstream(substring) >> substring;
            if (substring.size() > 0) strs.push_back(substring);
        }
        else strs.push_back(substring);

        prev_pos = ++pos;
    }

    auto lw = line.substr(prev_pos, pos-prev_pos);

    if (remove_empty)
    {
        std::stringstream(lw) >> lw;
        if (lw.size() > 0) strs.push_back(lw);
    }
    else strs.push_back(lw);

    return strs;
}

void push_ip (const std::string& str, std::vector<std::vector<int>>& ip_pool)
{
    auto ip_buf = split(str, '\t', true);

    if (ip_buf.size() > 0)
    {
        ip_buf = split(ip_buf.at(0), '.', true);

        if (ip_buf.size() == 4)
        {
            std::vector<int> ip;
            ip.reserve(4);

            for (const std::string& ip_s: ip_buf)
                ip.push_back(stoi(ip_s));

            ip_pool.push_back(ip);
        }
    }
}