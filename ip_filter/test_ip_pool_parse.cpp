#define BOOST_TEST_MODULE test_ip_pool_parse

#include "ip_pool_parse.h"

#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_SUITE(test_ip_pool_parse)

BOOST_AUTO_TEST_CASE(test_valid_string_split)
{
    BOOST_CHECK(split("1.1.1.1\t0\t1", '\t', true) == (std::vector<std::string>{"1.1.1.1","0","1"}));
    BOOST_CHECK(split("", '\t', true)              == (std::vector<std::string>{}));

    BOOST_CHECK(split("1.2.3.4", '.', true) == (std::vector<std::string>{"1","2","3","4"}));
    BOOST_CHECK(split("", '.', true)        == (std::vector<std::string>{}));
}

BOOST_AUTO_TEST_CASE(test_valid_ip_pool_parse)
{
    std::vector<std::string> test_lines =
    {
        "1.1.1.1",
        "1.10.1.1",
        "1.2.1.1",
        "1.1.1.",
        ""
    };

    std::vector<std::vector<int>> valid_ip_pool =
    {
        { 1,1,1,1  },
        { 1,10,1,1 },
        { 1,2,1,1  }
    };

    std::vector<std::vector<int>> ip_pool;
    ip_pool.reserve(4);

    for (const auto& str_line: test_lines)
        push_ip(str_line, ip_pool);

    BOOST_CHECK(ip_pool == valid_ip_pool);
}

}