#define BOOST_TEST_MODULE test_lalloc10

#include <vector>
#include <map>

#include "../include/lalloc10.h"

#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_SUITE(test_lalloc10)

BOOST_AUTO_TEST_CASE(test_lalloc10_map_add)
{
    std::map<int, int, std::less<int>, LAlloc10<std::pair<const int, int>>> mi;

    for (int i = 0; i < 10; ++i)
        mi.insert({i, i+10});

    for (int i = 0; i < int(mi.size()); ++i)
    {
        auto val_iter = mi.find(i);
        BOOST_CHECK(val_iter != mi.end());
        BOOST_CHECK(val_iter->first == i);
        BOOST_CHECK(val_iter->second == i+10);
    }
}

BOOST_AUTO_TEST_CASE(test_lalloc10_map_overflow)
{
    std::map<int, int, std::less<int>, LAlloc10<std::pair<const int, int>>> mi;

    for (int i = 0; i < 10; ++i)
        mi.insert({i, i+10});

    bool is_excepted = false;

    try
    {
        mi.insert({10, 20});
    }
    catch (const std::bad_alloc& e)
    {
        is_excepted = true;
    }
    catch (...)
    {
        is_excepted = false;
    }

    BOOST_CHECK(is_excepted);
    BOOST_CHECK(mi.size() == 10);
}

}
