#pragma once

#include "eorm_strutils.hpp"

namespace eorm {

struct SqlStrData
{
    std::string value;
    SqlStrData(const std::string& value_ = ""): value(value_) {}
    inline bool operator== (const std::string& op) const { return this->value == op; }
    friend std::ostream& operator<< (std::ostream &out, const SqlStrData& value_) { out << value_.value; return out; }
};

}
