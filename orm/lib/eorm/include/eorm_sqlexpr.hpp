/** \file */

#pragma once

#include "eorm_sqlstrdata.hpp"

namespace eorm {

struct SqlExpr: SqlStrData
{
    SqlExpr(const std::string& value_ = ""): SqlStrData(value_){}
};

}
