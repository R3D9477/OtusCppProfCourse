#define BOOST_TEST_MODULE test_customcontainer

#include "lalloc10.h"
#include "customcontainer.h"

#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_SUITE(test_customcontainer)

BOOST_AUTO_TEST_CASE(test_customcontainer_stdalloc)
{
    CustomContainer<int, 10> ic;

    for (size_t i = 0; i < ic.size(); ++i)
        ic.push_back(i);

    int j = 0;
    for (auto elem: ic) BOOST_CHECK(elem == j++);
}

BOOST_AUTO_TEST_CASE(test_customcontainer_lalloc10)
{
    CustomContainer<int, 10, LAlloc10<int>> ic;

    for (size_t i = 0; i < ic.size(); ++i)
        ic.push_back(i);

    int j = 0;
    for (auto elem: ic) BOOST_CHECK(elem == j++);
}

BOOST_AUTO_TEST_CASE(test_customcontainer_overflow)
{
    CustomContainer<int, 10, LAlloc10<int>> ic;

    for (size_t i = 0; i < ic.size(); ++i)
        ic.push_back(i);

    bool is_excepted = false;

    try
    {
        ic.push_back(11);
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
}

}
