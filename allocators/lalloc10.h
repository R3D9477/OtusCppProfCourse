#pragma once

#include <tuple>
#include <cassert>

template<typename T>
struct LAlloc10
{
    using value_type = T;

    T* buf;
    size_t max_size = 0;
    size_t allocated_size_n = 0;

    template<typename U>
    struct rebind { using other = LAlloc10<U>; };

    LAlloc10() = default;
    ~LAlloc10() = default;

    template<typename U>
    LAlloc10(const LAlloc10<U>&) { }

    void reserve (size_t n)
    {
        if (max_size < n)
        {
            max_size = n;
            buf = new T [max_size];
        }
    }

    T* allocate(std::size_t n)
    {
        std::ignore = n;

        reserve(10);

        if (allocated_size_n >= max_size)
            throw std::bad_alloc();

        return &buf[allocated_size_n++];
    }

    void deallocate(T* p, std::size_t n)
    {
        std::ignore = p;

        if (n >= max_size)
            delete [] buf;
    }
};
