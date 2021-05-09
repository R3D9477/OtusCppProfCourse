#include <cassert>
#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

// ("",  '.') -> [""]
// ("11", '.') -> ["11"]
// ("..", '.') -> ["", "", ""]
// ("11.", '.') -> ["11", ""]
// (".11", '.') -> ["", "11"]
// ("11.22", '.') -> ["11", "22"]

template <typename T> int sign (T value) { return (T(0) < value) - (value < T(0)); }

int main()
{
    try
    {
        auto split = [](const auto& str, char d)
        {
            std::vector<std::string> r;

            std::string::size_type start = 0;
            std::string::size_type stop  = str.find_first_of(d);

            while(stop != std::string::npos)
            {
                r.push_back(str.substr(start, stop - start));

                start = stop + 1;
                stop  = str.find_first_of(d, start);
            }

            r.push_back(str.substr(start));

            return r;
        };

        std::vector<std::vector<std::string>> ip_pool;/* =
        {
            { "1", "10", "1", "1" },
            { "1", "1",  "1", "1" },
            { "46", "70", "147", "26" },
            { "46", "70", "113", "73" },
            { "46", "70", "225", "39" },
            { "46", "70", "29", "76" },
            { "1", "2",  "1", "1" },
            { "5", "2",  "1", "1" },
            { "39", "46", "86", "85" },
            { "5", "189", "203", "46" }
        };*/

        for(std::string line; std::getline(std::cin, line);)
        {
            std::vector<std::string> v = split(line, '\t');
            ip_pool.push_back(split(v.at(0), '.'));
        }

        auto show_ip_pool = [](const auto& ip_pool)
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
        };

        // TODO reverse lexicographically sort
        std::sort(ip_pool.begin(), ip_pool.end(), [&](const auto& ip1, const auto& ip2)
        {
            for (auto ip1_iter = ip1.cbegin(), ip2_iter = ip2.cbegin() ; ip1_iter != ip1.end() && ip2_iter != ip2.end() ; ++ip1_iter, ++ip2_iter) {
                switch (sign( stoi(*ip1_iter) - stoi(*ip2_iter) )) {
                    case  1: ; return true;
                    case -1: ; return false;
                }
            }
            return false;
        });
        show_ip_pool(ip_pool);
        // 222.173.235.246
        // 222.130.177.64
        // 222.82.198.61
        // ...
        // 1.70.44.170
        // 1.29.168.152
        // 1.1.234.8

        auto filter = [&](const int ip_b1, const int ip_b2 = 255)
        {
            decltype(ip_pool) ip_pool_filtered;
            ip_pool_filtered.reserve(255);

            std::copy_if (ip_pool.begin(), ip_pool.end(), std::back_inserter(ip_pool_filtered), [&](const auto& ip) {
                return stoi(ip[0]) == ip_b1 && (ip_b2 == 255 ? true : stoi(ip[1]) == ip_b2);
            });

            return ip_pool_filtered;
        };

        auto filter_any = [&](const int ip_b)
        {
            decltype(ip_pool) ip_pool_filtered;
            ip_pool_filtered.reserve(255);

            std::copy_if (ip_pool.begin(), ip_pool.end(), std::back_inserter(ip_pool_filtered), [&](const auto& ip) {
                return
                       stoi(ip[0]) == ip_b
                    || stoi(ip[1]) == ip_b
                    || stoi(ip[2]) == ip_b
                    || stoi(ip[3]) == ip_b;
            });

            return ip_pool_filtered;
        };

        // TODO filter by first byte and output
        // ip = filter(1)
        auto ip_pool_filtered = filter(1);
        show_ip_pool(ip_pool_filtered);
        // 1.231.69.33
        // 1.87.203.225
        // 1.70.44.170
        // 1.29.168.152
        // 1.1.234.8

        // TODO filter by first and second bytes and output
        // ip = filter(46, 70)
        ip_pool_filtered = filter(46, 70);
        show_ip_pool(ip_pool_filtered);
        // 46.70.225.39
        // 46.70.147.26
        // 46.70.113.73
        // 46.70.29.76

        // TODO filter by any byte and output
        // ip = filter_any(46)
        ip_pool_filtered = filter_any(46);
        show_ip_pool(ip_pool_filtered);
        // 186.204.34.46
        // 186.46.222.194
        // 185.46.87.231
        // 185.46.86.132
        // 185.46.86.131
        // 185.46.86.131
        // 185.46.86.22
        // 185.46.85.204
        // 185.46.85.78
        // 68.46.218.208
        // 46.251.197.23
        // 46.223.254.56
        // 46.223.254.56
        // 46.182.19.219
        // 46.161.63.66
        // 46.161.61.51
        // 46.161.60.92
        // 46.161.60.35
        // 46.161.58.202
        // 46.161.56.241
        // 46.161.56.203
        // 46.161.56.174
        // 46.161.56.106
        // 46.161.56.106
        // 46.101.163.119
        // 46.101.127.145
        // 46.70.225.39
        // 46.70.147.26
        // 46.70.113.73
        // 46.70.29.76
        // 46.55.46.98
        // 46.49.43.85
        // 39.46.86.85
        // 5.189.203.46
    }
    catch(const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}
