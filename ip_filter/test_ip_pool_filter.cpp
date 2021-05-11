#define BOOST_TEST_MODULE test_ip_pool_filter

#include "ip_pool_filter.h"

#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_SUITE(test_ip_pool_filter)

BOOST_AUTO_TEST_CASE(test_valid_ip_pool_filter1)
{
    std::vector<std::vector<int>> test_ip_pool =
    {
        { 1,  1,  1,  1  },
        { 1,  10, 1,  1  },
        { 1,  46, 1,  1  },
        { 46, 1,  1,  0  },
        { 2,  42, 42, 1  },
        { 1,  46, 1,  42 },
        { 1,  46, 42, 42 }
    };

    std::vector<std::vector<int>> valid_ip_pool =
    {
        { 1,  1,  1,  1  },
        { 1,  10, 1,  1  },
        { 1,  46, 1,  1  },
        { 1,  46, 1,  42 },
        { 1,  46, 42, 42 }
    };

    BOOST_CHECK( filter(test_ip_pool, 1) == valid_ip_pool );
}

BOOST_AUTO_TEST_CASE(test_valid_ip_pool_filter_1)
{
    std::vector<std::vector<int>> test_ip_pool =
    {
        { 1,  1,  1,  1  },
        { 1,  10, 1,  1  },
        { 1,  46, 1,  1  },
        { 46, 1,  1,  0  },
        { 2,  42, 42, 1  },
        { 1,  46, 1,  42 },
        { 1,  46, 42, 42 }
    };

    std::vector<std::vector<int>> valid_ip_pool =
    {
        { 1,  46, 1,  1  },
        { 1,  46, 1,  42 },
        { 1,  46, 42, 42 }
    };

    BOOST_CHECK( filter(test_ip_pool, 1, 46) == valid_ip_pool );
}

BOOST_AUTO_TEST_CASE(test_valid_ip_pool_filter_2)
{
    std::vector<std::vector<int>> test_ip_pool =
    {
        { 1,  1,  1,  1  },
        { 1,  10, 1,  1  },
        { 1,  46, 1,  1  },
        { 46, 1,  1,  0  },
        { 2,  42, 42, 1  },
        { 1,  46, 1,  42 },
        { 1,  46, 42, 42 }
    };

    std::vector<std::vector<int>> valid_ip_pool =
    {
        { 1,  46, 1,  1  },
        { 46, 1,  1,  0  },
        { 1,  46, 1,  42 },
        { 1,  46, 42, 42 }
    };

    BOOST_CHECK( filter_any(test_ip_pool, 46) == valid_ip_pool );
}

}
