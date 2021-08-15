/** \file */

#include <iostream>
#include <sstream>

#include "async.h"
#include "asynccontext.h"
#include "asynclogger.h"

namespace async
{

handle_t connect(std::size_t bulk)
{
    return new AsyncContext(bulk);
}

void receive(handle_t handle, const char *data, std::size_t size)
{
    if (handle)
    {
        std::stringstream in_buff;
        in_buff.write(data,size);

        ((AsyncContext*)(handle))->receive(in_buff);
        AsyncLogger::get().push_context(((AsyncContext*)(handle)));
    }
}

void disconnect(handle_t handle)
{
    if (handle)
        delete ((AsyncContext*)(handle));
}

}
