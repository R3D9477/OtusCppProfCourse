/** \file */

#pragma once

#include <tuple>
#include <memory>
#include <iostream>

#include "asynccontext.h"
#include "asynclogger.h"

AsyncContext* connect(const size_t block_size);
void receive(AsyncContext* context, std::istream& in_buf);
void disconnect(AsyncContext* context);
