/** \file */

#pragma once

#include <list>
#include <stack>
#include <tuple>
#include <vector>
#include <numeric>
#include <ostream>
#include <algorithm>

template<typename T, T DEFAULT_VALUE>
/**
 * @brief Matrix - класс бесконечной многомерной матрицы с поддержкой forward iterator и доступом к элементам по индексу
 */
class Matrix
{
    size_t index = 0;
    T value = DEFAULT_VALUE;
    Matrix* parent = nullptr;
    std::list<Matrix> columns = {};

    bool cells_changed = false;
    std::vector<std::tuple<size_t,size_t,Matrix&>> cells_buf = {};

public:

    Matrix (size_t index_ = 0, Matrix* parent_ = nullptr) : index{index_},parent{parent_} { }
    Matrix (T value_) : value{value_} { }

    Matrix& operator= (T value_)
    {
        this->value = value_;

        if (value_ == DEFAULT_VALUE && this->parent != nullptr)
        {
            this->parent->cells_changed = true;
            //this->parent->columns.remove(*this);
        }

        return *this;
    }

    std::stack<size_t> getFullIndex () const
    {
        std::stack<size_t> indexes;

        const Matrix* parent_ = this;

        while (parent_ != nullptr)
        {
            if (parent_->parent != nullptr)
                indexes.push(parent_->index);
            parent_ = parent_->parent;
        }

        return indexes;
    }

    T getValue () const { return this->value; }

    Matrix& operator[] (size_t column_index)
    {
        Matrix* retmatrix = nullptr;

        for (auto columns_it = columns.rbegin(); columns_it != columns.rend(); ++columns_it)
            if (columns_it->index == column_index)
            {
                retmatrix = &(*columns_it);
                break;
            }
            else if (columns_it->index < column_index)
            {
                break;
            }

        if (retmatrix == nullptr)
        {
            auto it = std::find_if(columns.begin(), columns.end(), [&column_index](auto&a){ return a.index > column_index; });

            if (it == columns.end())
            {
                columns.push_back({column_index});
                retmatrix = &(*columns.rbegin());
            }
            else
            {
                it = columns.insert(it, {column_index});
                retmatrix = &(*it);
            }

            retmatrix->parent = this;

            cells_changed = true;
        }

        return *retmatrix;
    }

    bool operator== (const Matrix& matrix) const
    {
        bool result = true;

        const Matrix* a = this;
        const Matrix* b = &matrix;

        while (a != nullptr && b != nullptr)
        {
            if (a == nullptr && b == nullptr) return true;
            if (a == nullptr && b != nullptr) return false;
            if (a != nullptr && b == nullptr) return false;

            if (!(result = (a->parent == b->parent) && (a->index == b->index)))
                return false;

            a = a->parent;
            b = b->parent;
        }

        return result;
    }

    bool operator== (const T& value_) const
    {
        return this->value == value_;
    }

    bool operator!= (const Matrix& matrix) const { return !(*this == matrix); }

    friend std::ostream& operator<< (std::ostream& os, Matrix& matrix)
    {
        return os << matrix.value;
    }

    auto begin()
    {
        this->refresh_cells_buffer();
        return this->cells_buf.begin();
    }

    auto end()
    {
        this->refresh_cells_buffer();
        return this->cells_buf.end();
    }

    auto find (Matrix& matrix)
    {
        return std::find(this->begin(), this->end(), matrix.get_cell());
    }

    size_t size ()
    {
        this->refresh_cells_buffer();
        return this->cells_buf.size();
    }

private:

    std::tuple<size_t,size_t,Matrix&> get_cell()
    {
        return { this->parent == nullptr ? 0 : this->parent->index, this->index, *this };
    }

    void refresh_cells_buffer()
    {
        this->cells_buf.clear();

        auto fill_childs_recursively = [](auto& matrix, auto& vec)
        {
            auto recursive_lambda = [](auto& matrix, auto& vec, const auto& ff) -> void
            {
                if (matrix.columns.empty())
                {
                    if (matrix.value != DEFAULT_VALUE)
                        vec.push_back(matrix.get_cell());
                }
                else for (auto& c: matrix.columns) ff(c,vec,ff);
            };
            recursive_lambda(matrix, vec, recursive_lambda);
        };
        fill_childs_recursively(*this, this->cells_buf);

        cells_changed = false;
    }
};
