/** \file */

#include "bulkserver.h"

/**
 * @brief main is the 'entry point' of BULK application.
 * @return result of application execution, non-zero value is code of the error.
 */
int main(int argc, const char* argv[])
{
    try
    {
        boost::asio::io_context io_context;

        BulkServer s(
            io_context,
            (argc > 2 ? atoi(argv[1]) : 9000),
            (argc > 3 ? atoi(argv[2]) : 3)
        );

        io_context.run();
    }
    catch (std::exception& e)
    {
        std::cerr << "Exception: " << e.what() << "\n";
    }

    return 0;
}
