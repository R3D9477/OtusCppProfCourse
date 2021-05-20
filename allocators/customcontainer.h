#pragma once

#include <memory>

template <typename T, size_t max_size = 10, class A=std::allocator<T>>
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
        this->data = std::allocator_traits<A>::allocate(allocator, max_size);
        this->pos = 0;
    }

    void push_back (T value)
    {
        std::allocator_traits<A>::construct(allocator, &this->data[this->pos], value);
        this->pos++;
    }

    Iterator begin() { return Iterator(&this->data[0]); }
    Iterator end()   { return Iterator(&this->data[max_size]); }

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

        if (this->first_ptr == nullptr) this->first_ptr = p;
        else this->last_ptr = p;
    }

    Iterator begin() { return Iterator(this->first_ptr);  }
    Iterator end()   { return Iterator(this->last_ptr+1); }

private:

    A allocator;

    T* first_ptr = nullptr;
    T* last_ptr  = nullptr;

*/
