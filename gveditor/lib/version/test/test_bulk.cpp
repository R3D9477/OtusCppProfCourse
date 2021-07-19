/** \file */

#define BOOST_TEST_MODULE test_bulk

#include "async.h"

#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_SUITE(test_bulk)

/*!
 *  тест функции get_bulks ввода/вывода пачек команд
 */
BOOST_AUTO_TEST_CASE(test_get_bulks)
{
    std::queue<std::vector<std::string>> bulks;

    std::stringstream out_valid;

    out_valid << "bulk: cmd1, cmd2" << std::endl;
    out_valid << "bulk: cmd3, cmd4, cmd5, cmd6" << std::endl;
    out_valid << "bulk: cmd7" << std::endl;

    std::stringstream in_buf;

    in_buf << "cmd1" << std::endl;
    in_buf << "cmd2" << std::endl;
    in_buf << "{"    << std::endl;
    in_buf << "cmd3" << std::endl;
    in_buf << "cmd4" << std::endl;
    in_buf << "cmd5" << std::endl;
    in_buf << "cmd6" << std::endl;
    in_buf << "}"    << std::endl;
    in_buf << "cmd7" << std::endl;

    std::stringstream out_buf;

    auto context = connect(3, out_buf);
    receive(context, in_buf);
    std::this_thread::sleep_for(1s);
    disconnect(std::move(context));

    BOOST_CHECK(out_buf.str() == out_valid.str());
}

}
