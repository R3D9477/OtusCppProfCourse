#define BOOST_TEST_MODULE test_factorial

#include "factorial.h"

#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_SUITE(test_factorial)

BOOST_AUTO_TEST_CASE(test_factorial_calc)
{
    BOOST_CHECK(calc_factorial(-1) == 0);
    BOOST_CHECK(calc_factorial(0)  == 1);
    BOOST_CHECK(calc_factorial(1)  == 1);
    BOOST_CHECK(calc_factorial(2)  == 2);
    BOOST_CHECK(calc_factorial(3)  == 6);
    BOOST_CHECK(calc_factorial(4)  == 24);
    BOOST_CHECK(calc_factorial(10) == 3628800);
}

}
