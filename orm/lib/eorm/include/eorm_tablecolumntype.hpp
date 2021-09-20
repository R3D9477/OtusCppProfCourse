/** \file */

#pragma once

#include <ctime>
#include <string>
#include <type_traits>

namespace eorm {
namespace core {

enum TableColumnType
{
    INTEGER,
    REAL,
    DATETIME,
    TEXT
};

template <typename T>
static TableColumnType getTableColumnType()
{
         if (std::is_same<T, std::time_t>::value)   return TableColumnType::DATETIME;
    else if (std::is_same<T, float>::value)         return TableColumnType::REAL;
    else if (std::is_same<T, double>::value)        return TableColumnType::REAL;
    else if (std::is_same<T, bool>::value)          return TableColumnType::INTEGER;
    else if (std::is_same<T, int>::value)           return TableColumnType::INTEGER;
    else if (std::is_same<T, const char *>::value)  return TableColumnType::TEXT;
    else if (std::is_same<T, std::string>::value)   return TableColumnType::TEXT;
    else throw "unknown data type";
}

typedef TableColumnType TCT;

}
}
