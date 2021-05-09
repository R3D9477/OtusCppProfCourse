#include "version_lib.h"

#ifdef DEBUG

int version()
{
    return 1;
}

#else

#include "version.h"

int version()
{
    return PROJECT_VERSION_PATCH;
}

#endif