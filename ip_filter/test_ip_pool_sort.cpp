#define BOOST_TEST_MODULE test_ip_pool_sort

#include "ip_pool_sort.h"

#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_SUITE(test_ip_pool_sort)

BOOST_AUTO_TEST_CASE(test_valid_ip_pool_sort)
{
    std::vector<std::vector<int>> ip_pool =
    {
        { 1,1,1,1  },
        { 1,10,1,1 },
        { 1,2,1,1  }
    };

    std::vector<std::vector<int>> valid_ip_pool =
    {
        { 1,10,1,1 },
        { 1,2,1,1  },
        { 1,1,1,1  }
    };

    sort_ip_pool(ip_pool);

    BOOST_CHECK(ip_pool == valid_ip_pool);
}

}