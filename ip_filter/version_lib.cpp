#include "version_lib.h"

#ifdef DEBUG

int version()
{
    return 0;
}

#else

#include "version.h"

int version()
{
    return PROJECT_VERSION_PATCH;
}

#endif