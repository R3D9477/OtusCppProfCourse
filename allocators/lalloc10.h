#pragma once

#include <tuple>
#include <cassert>

template<typename T, std::size_t SIZE = 10>
struct LAlloc10
{
    using value_type = T;

    template<typename U>
    struct rebind { using other = LAlloc10<U,SIZE>; };

    LAlloc10() = default;

    ~LAlloc10()
    {
        clear();
    }

    template<typename U>
    LAlloc10(const LAlloc10<U,SIZE>&) { }

    void clear ()
    {
        if (max_size > 0)
        {
            max_size = 0;
            last_allocated_index = 0;

            delete [] buf;
        }
    }

    T* allocate(std::size_t n)
    {
        reserve(SIZE);

        if ( (last_allocated_index+n-1) >= max_size )
            throw std::bad_alloc();

        auto p = &buf[last_allocated_index];

        last_allocated_index += n;

        return p;
    }

    void deallocate(T* p, std::size_t n)
    {
        std::ignore = p;

        if (n >= max_size)
            delete [] buf;
    }

private:

    T* buf = nullptr;
    size_t max_size = 0;
    size_t last_allocated_index = 0;

    void reserve (size_t n)
    {
        if (max_size < n)
        {
            if (max_size > 0)
                buf = nullptr; // extend(n)
            else
                buf = new T [n];

            if (!buf)
                throw std::bad_alloc();

            max_size = n;
        }
    }

    void extend (size_t n)
    {
        if (void* tmp_p = std::realloc(buf, n*sizeof(T)))
            buf = static_cast<T*>(tmp_p);
        else
            throw std::bad_alloc();
    }
};
