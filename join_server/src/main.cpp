/** \file */

#include "joinserver.h"

/**
 * @brief main is the 'entry point' of BULK application.
 * @return result of application execution, non-zero value is code of the error.
 */
int main(int argc, const char* argv[])
{
    try
    {
        boost::asio::io_service io_service;

        JoinServer s(
            io_service,
            (argc > 2 ? atoi(argv[1]) : 9000)
        );

        io_service.run();
    }
    catch (std::exception& e)
    {
        std::cerr << "Exception: " << e.what() << "\n";
    }

    return 0;
}
