/** \file */

#define BOOST_TEST_MODULE test_bulk

#include <queue>
#include <string.h>

#include "async.h"
#include "asynclogger.h"

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

    const char* data = "cmd1\ncmd2\n{\ncmd3\ncmd4\ncmd5\ncmd6\n}\ncmd7";
    const size_t size = strlen(data);

    std::stringstream out_buf;
    AsyncLogger::get().set_output_buffer(&out_buf);

    auto context = async::connect(3);
    async::receive(context, data, size);
    std::this_thread::sleep_for(1s);
    async::disconnect(context);

    BOOST_CHECK(out_buf.str() == out_valid.str());
}

}
