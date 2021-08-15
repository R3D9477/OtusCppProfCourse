/** \file */

#include "async.h"

AsyncContext* connect(const size_t block_size)
{
    return new AsyncContext(block_size);
}

void receive(AsyncContext* context, std::istream& in_buff)
{
    context->receive(in_buff);
    AsyncLogger::get().push_context(context);
}

void disconnect(AsyncContext* context)
{
    delete context;
    //context.~unique_ptr();
}
