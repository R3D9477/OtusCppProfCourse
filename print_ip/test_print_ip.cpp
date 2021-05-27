#define BOOST_TEST_MODULE test_print_ip

#include "print_ip.h"

#include <sstream>
#include <iostream>

#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_SUITE(test_print_ip)

BOOST_AUTO_TEST_CASE(test_print_ip_arithmetic)
{
    std::stringstream ss;

    print_ip( char{-1}, ss );
    print_ip( short{0}, ss );
    print_ip( int{2130706433}, ss );
    print_ip( long{8875824491850138409}, ss );
    print_ip( std::string{"Hello, World!"}, ss );

    std::string valid_result{R"(255
0.0
127.0.0.1
123.45.67.89.101.112.131.41
Hello, World!
)"};

    BOOST_CHECK(ss.str() == valid_result);
}

BOOST_AUTO_TEST_CASE(test_print_ip_containers)
{
    std::stringstream ss;

    print_ip( std::vector<int>{100,200,300,400}, ss );
    print_ip( std::list<short>{400,300,200,100}, ss );

    std::string valid_result{R"(100.200.300.400
400.300.200.100
)"};

    BOOST_CHECK(ss.str() == valid_result);
}

BOOST_AUTO_TEST_CASE(test_print_ip_tuple)
{
    std::stringstream ss;

    print_ip( std::make_tuple(123,456,789,0), ss );

    std::string valid_result{R"(123.456.789.0
)"};

    BOOST_CHECK(ss.str() == valid_result);
}

}
