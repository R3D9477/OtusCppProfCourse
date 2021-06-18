/** \file */

#include "bulk.h"

/**
 * @brief main is the 'entry point' of BULK application.
 * @return result of application execution, non-zero value is code of the error.
 */
int main(int argc, const char* argv[])
{
    std::vector<std::vector<std::string>> bulks;
    bulks.reserve(50);

    get_bulks( std::cin, std::cout, (argc > 2 ? atoi(argv[1]) : 3), bulks );

    return 0;
}
