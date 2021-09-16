/** \file */

#pragma once

#include "eorm_sqlstrdata.hpp"

namespace eorm {

struct SqlName: SqlStrData
{
    SqlName(const std::string& value_ = ""): SqlStrData(value_){}
};

}
