/** \file */

#pragma once

#include <string>
#include <sstream>

namespace eorm {

template<typename T>
typename std::enable_if<std::is_fundamental<T>::value,
    std::string>::type stringify(const T& t)
{
    return std::to_string(t);
}

template<typename T>
typename std::enable_if<!std::is_fundamental<T>::value,
    std::string>::type stringify(const T& t)
{
    return std::string(t);
}

template<typename T>
typename std::enable_if<std::is_same<T, const char*>::value,
    std::string>::type stdstr_or_type(const T t)
{
    std::string buf = ((const char *)(t));
    return buf;
}

template<typename T>
typename std::enable_if<!std::is_same<T, const char*>::value,
    T>::type stdstr_or_type(const T t)
{
    return t;
}

[[maybe_unused]] static std::string removeSqlDelimiter(const std::string& expr_)
{
    return expr_.empty() ? expr_ : expr_[expr_.size()-1] == ';' ? expr_.substr(0,expr_.size()-1) : expr_;
}

[[maybe_unused]] static void pop_back(std::stringstream& stream)
{
    stream.str(stream.str().erase(long(stream.tellp())-1));
}

[[maybe_unused]] static void pop_back(std::ostringstream& stream)
{
    stream.str(stream.str().erase(long(stream.tellp())-1));
}

}
