#pragma once

#include <string>
#include <vector>
#include <iostream>
#include <algorithm>

void show_ip_pool (std::vector<std::vector<int>>& ip_pool)
{
    for (const auto& ip: ip_pool)
    {
        for (auto ip_part = ip.cbegin(); ip_part != ip.end(); ++ip_part)
        {
            if (ip_part != ip.begin()) std::cout << ".";
            std::cout << (*ip_part);
        }
        std::cout << std::endl;
    }
}