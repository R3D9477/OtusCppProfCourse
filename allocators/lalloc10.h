#pragma once

#include <tuple>
#include <cassert>

template<typename T>
struct LAlloc10
{
    using value_type = T;

    template<typename U>
    struct rebind { using other = LAlloc10<U>; };

    LAlloc10()
    {
        reserve(10);
    }

    ~LAlloc10()
    {
        clear();
    }

    template<typename U>
    LAlloc10(const LAlloc10<U>&) { }

    void reserve (size_t n)
    {
        if (this->max_size < n)
        {
            if (this->max_size > 0) {
                if (void* tmp_p = std::realloc(this->buf, n*sizeof(T)))
                    this->buf = static_cast<T*>(tmp_p);
                else
                    throw std::bad_alloc();
            }
            else this->buf = new T [n];

            if (!this->buf)
                throw std::bad_alloc();

            this->max_size = n;
        }
    }

    void clear ()
    {
        if (max_size > 0)
        {
            this->max_size = 0;
            this->allocated_size_n = 0;

            delete [] this->buf;
        }
    }

    T* allocate(std::size_t n)
    {
        if ( (this->allocated_size_n+n-1) >= this->max_size )
            throw std::bad_alloc();

        auto p = &this->buf[this->allocated_size_n];

        this->allocated_size_n += n;

        return p;
    }

    void deallocate(T* p, std::size_t n)
    {
        std::ignore = p;

        if (n >= max_size)
            delete [] buf;
    }

private:

    T* buf;
    size_t max_size = 0;
    size_t allocated_size_n = 0;
};
