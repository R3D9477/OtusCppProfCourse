/** \file */

#pragma once

#include <tuple>
#include <memory>
#include <iostream>

#include "asynccontext.h"

std::unique_ptr<AsyncContext> connect(const size_t block_size, std::ostream& log_buf = std::cout);
void receive(std::unique_ptr<AsyncContext>& context, std::istream& in_buf);
void disconnect(std::unique_ptr<AsyncContext>&& context);
