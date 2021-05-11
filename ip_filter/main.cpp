#include <cassert>
#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

#include "ip_pool_show.h"
#include "ip_pool_sort.h"
#include "ip_pool_filter.h"
#include "ip_pool_parse.h"

int main()
{
    try
    {
        std::vector<std::vector<int>> ip_pool;
        ip_pool.reserve(1000);

        /*auto tmpBuf = std::vector<std::string>{
            { "1.2.1.1"      },
            { "1.10.1.1"     },
            { "1.1.1.1"      },
            { "46.70.147.26" },
            { "46.70.113.73" },
            { "46.70.225.39" }
        };
        for (std::string& line: tmpBuf)*/
        for(std::string line; std::getline(std::cin, line);)
            push_ip(line, ip_pool);

        // reverse lexicographically sort
        sort_ip_pool(ip_pool);
        show_ip_pool(ip_pool);

        // filter by first byte and output
        auto ip_pool_filtered = filter(ip_pool, 1);
        show_ip_pool(ip_pool_filtered);

        // filter by first and second bytes and output
        ip_pool_filtered = filter(ip_pool, 46, 70);
        show_ip_pool(ip_pool_filtered);

        // filter by any byte and output
        ip_pool_filtered = filter_any(ip_pool, 46);
        show_ip_pool(ip_pool_filtered);
    }
    catch(const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}
