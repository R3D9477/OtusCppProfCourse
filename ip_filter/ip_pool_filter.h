#pragma once

#include <string>
#include <vector>
#include <algorithm>

auto filter (const std::vector<std::vector<int>>& ip_pool, const int ip_b1, const int ip_b2 = 255)
{
    std::vector<std::vector<int>> ip_pool_filtered;
    ip_pool_filtered.reserve(255);

    std::copy_if (ip_pool.begin(), ip_pool.end(), std::back_inserter(ip_pool_filtered), [&](const auto& ip) {
        return ip[0] == ip_b1 && (ip_b2 == 255 ? true : ip[1] == ip_b2);
    });

    return ip_pool_filtered;
}

auto filter_any (const std::vector<std::vector<int>>& ip_pool, const int ip_b)
{
    std::vector<std::vector<int>> ip_pool_filtered;
    ip_pool_filtered.reserve(255);

    std::copy_if (ip_pool.begin(), ip_pool.end(), std::back_inserter(ip_pool_filtered), [&](const auto& ip) {
        return
               ip[0] == ip_b
            || ip[1] == ip_b
            || ip[2] == ip_b
            || ip[3] == ip_b;
    });

    return ip_pool_filtered;
}