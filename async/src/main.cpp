/** \file */

#include <iostream>
#include <thread>
#include <chrono>
#include <string.h>

#include "async.h"

/**
 * @brief main is the 'entry point' of BULK application.
 * @return result of application execution, non-zero value is code of the error.
 */
int main(int argc, const char* argv[])
{
    const char* data = "cmd1\ncmd2\ncmd3\ncmd4";
    const size_t size = strlen(data);

    auto context = async::connect( (argc > 2 ? atoi(argv[1]) : 3) );

    async::receive(context, data, size);

    std::this_thread::sleep_for(std::chrono::seconds(1));

    async::disconnect(context);

    return 0;
}
