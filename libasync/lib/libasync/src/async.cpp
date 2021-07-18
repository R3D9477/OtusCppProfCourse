/** \file */

#include "async.h"

std::unique_ptr<AsyncContext> connect(const size_t block_size, std::ostream& log_buf)
{
    return std::make_unique<AsyncContext>(block_size, log_buf);
}

void receive(std::unique_ptr<AsyncContext>& context, std::istream& in_buff)
{
    context->receive(in_buff);
}

void disconnect(std::unique_ptr<AsyncContext>&& context)
{
    context.~unique_ptr();
}
