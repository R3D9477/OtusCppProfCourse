#pragma once

#include <string>
#include <vector>
#include <iostream>
#include <algorithm>

template <typename T> int spaceship (T op1, T op2) { return (op2 < op1) - (op1 < op2); }

void sort_ip_pool (std::vector<std::vector<int>>& ip_pool)
{
    std::sort(ip_pool.begin(), ip_pool.end(), [&](const auto& ip1, const auto& ip2)
    {
        for (auto ip1_iter = ip1.cbegin(), ip2_iter = ip2.cbegin() ; ip1_iter != ip1.end() && ip2_iter != ip2.end() ; ++ip1_iter, ++ip2_iter) {
            switch ( spaceship( (*ip1_iter), (*ip2_iter) ) ) {
                case  1: ; return true;
                case -1: ; return false;
            }
        }
        return false;
    });
}