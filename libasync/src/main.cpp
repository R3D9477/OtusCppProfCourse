/** \file */

#include <sstream>
#include <iostream>

#include "async.h"

/**
 * @brief main is the 'entry point' of BULK application.
 * @return result of application execution, non-zero value is code of the error.
 */
int main(int argc, const char* argv[])
{
    std::stringstream in_buf;

    auto context = connect( (argc > 2 ? atoi(argv[1]) : 3) );

    for ( std::string cmd; std::getline(std::cin, cmd) && !cmd.empty(); )
        in_buf << cmd << std::endl;
    receive(context, in_buf);

    std::this_thread::sleep_for(1s);

    disconnect(std::move(context));

    return 0;
}
