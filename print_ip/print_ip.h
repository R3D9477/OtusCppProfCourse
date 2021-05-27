/** \file */

#pragma once

#include <iostream>
#include <vector>
#include <tuple>
#include <list>

template <typename T, unsigned long Size = sizeof(T),
    std::enable_if_t<
        std::is_arithmetic<T>::value,
    bool> = true>
/**
 * @brief specialization of print_ip function, which prints incomming parameter in IP Address format, for arithmetic data types: char,short,int,long,etc.
 *          this specification uses recurcive algorhythm.
 * @param ip - value which sould be printed in IP Address format
 * @param os - target data stream where ip should be printed, default is std::cout
 */
void print_ip (const T ip, std::ostream& os = std::cout)
{
    if constexpr (Size > 0)
    {
        os << int( ( ip >> ( 8* (Size-1) ) ) & 0xFF );

        if constexpr (Size-1 > 0)
        {
            os << ".";
            print_ip<T, Size-1, true>(ip, os);
        }
        else
        {
            std::ignore = ip;
            os << std::endl;
        }
    }
}


template <typename T,
    std::enable_if_t<
        std::is_same<T, std::string>::value,
    bool> = true>
/**
 * @brief specialization of print_ip function, which prints incomming parameter in IP Address format, for std::string data type.
 * @param ip - value which sould be printed in IP Address format
 * @param os - target data stream where ip should be printed, default is std::cout
 */
void print_ip (const T& ip, std::ostream& os = std::cout)
{
    os << ip << std::endl;
}


template <typename T,
    std::enable_if_t<
            std::is_same<T, std::list<typename T::value_type>>::value
        ||  std::is_same<T, std::vector<typename T::value_type>>::value,
    bool> = true>
/**
 * @brief specialization of print_ip function, which prints incomming parameter in IP Address format, for containers: std::vector and std::list.
 * @param ip - value which sould be printed in IP Address format
 * @param os - target data stream where ip should be printed, default is std::cout
 */
void print_ip (const T& ip, std::ostream& os = std::cout)
{
    bool dot = true;
    for (auto elem: ip)
    {
        if ( dot ) dot = false;
        else os << '.';
        os << elem;
    }
    os << std::endl;
}


template <typename T, unsigned long Index = 0, typename T1 = void,
    std::enable_if_t<
        (std::tuple_size<T>() > 0),
    bool> = true>
/**
 * @brief specialization of print_ip function, which prints incomming parameter in IP Address format, for tuples; types of all elements must be the same.
 *          this specification uses recurcive algorhythm.
 * @param ip - value which sould be printed in IP Address format
 * @param os - target data stream where ip should be printed, default is std::cout
 */
void print_ip (const T ip, std::ostream& os = std::cout)
{
    if constexpr ( Index == 0 )
    {
        print_ip< T, Index+1, decltype(std::get<0>(ip)) >(ip, os);
    }
    else if constexpr ( Index <= std::tuple_size<T>() )
    {
        static_assert( std::is_same< decltype(std::get<Index-1>(ip)) , T1 >::value , "invalid type" );

        if constexpr ( Index > 1)
            os << '.';

        os << std::get<Index-1>(ip);

        print_ip< T, Index+1, T1 >(ip, os);
    }
    else os << std::endl;
}
