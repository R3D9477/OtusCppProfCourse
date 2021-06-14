#pragma once

#include <memory>

template <typename T, size_t max_size, class A=std::allocator<T>>
class CustomContainer
{
public:

    struct Iterator
    {
        Iterator(T* ptr) : m_ptr(ptr) {}

        T& operator*() const { return *m_ptr; }
        T* operator->() { return m_ptr; }

        Iterator& operator++() { m_ptr++; return *this; }
        Iterator operator++(int) { Iterator tmp = *this; ++(*this); return tmp; }

        friend bool operator== (const Iterator& a, const Iterator& b) { return a.m_ptr == b.m_ptr; };
        friend bool operator!= (const Iterator& a, const Iterator& b) { return a.m_ptr != b.m_ptr; };

    private:

        T* m_ptr;
    };

    CustomContainer ()
    {
        data = std::allocator_traits<A>::allocate(allocator, max_size);
        pos = 0;
    }

    void push_back (T value)
    {
        if (pos >= max_size)
            throw std::bad_array_new_length();

        std::allocator_traits<A>::construct(allocator, &data[pos], value);
        pos++;
    }

    Iterator begin() { return Iterator(&data[0]); }
    Iterator end()   { return Iterator(&data[max_size]); }

    size_t size () const { return max_size; }

private:

    A allocator;

    T* data;
    size_t pos;
};

/*

    void push_back (T value)
    {
        auto p = std::allocator_traits<A>::allocate(allocator, 1);
        std::allocator_traits<A>::construct(allocator, p, value);

        if (first_ptr == nullptr) first_ptr = p;
        else last_ptr = p;
    }

    Iterator begin() { return Iterator(first_ptr);  }
    Iterator end()   { return Iterator(last_ptr+1); }

private:

    A allocator;

    T* first_ptr = nullptr;
    T* last_ptr  = nullptr;

*/
