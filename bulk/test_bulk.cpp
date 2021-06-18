/** \file */

#define BOOST_TEST_MODULE test_bulk

#include "bulk.h"

#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_SUITE(test_bulk)

/*!
 *  тест функции get_bulks ввода/вывода пачек команд
 */
BOOST_AUTO_TEST_CASE(test_get_bulks)
{
    std::vector<std::vector<std::string>> bulks;
    bulks.reserve(50);

    std::stringstream ss_valid;

    ss_valid << "bulk: cmd1, cmd2" << std::endl;
    ss_valid << "bulk: cmd3, cmd4, cmd5, cmd6" << std::endl;
    ss_valid << "bulk: cmd7" << std::endl;

    std::stringstream ss_in;

    ss_in << "cmd1" << std::endl;
    ss_in << "cmd2" << std::endl;
    ss_in << "{"    << std::endl;
    ss_in << "cmd3" << std::endl;
    ss_in << "cmd4" << std::endl;
    ss_in << "cmd5" << std::endl;
    ss_in << "cmd6" << std::endl;
    ss_in << "}"    << std::endl;
    ss_in << "cmd7" << std::endl;

    std::stringstream ss_out;

    get_bulks( ss_in, ss_out, 3, bulks );

    BOOST_CHECK(ss_out.str() == ss_valid.str());
}

}
