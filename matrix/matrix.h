/** \file */

#pragma once

#include <tuple>
#include <vector>
#include <numeric>
#include <iostream>

template<typename T, T DEFAULT_VALUE, size_t DEFAULT_SIZE = 10>
/**
 * @brief Matrix - класс бесконечной матрицы с поддержкой forward iterator и доступом к элементам по индексу
 */
class Matrix
{
    /**
     * @brief MatrixRow - класс бесконечной строки матрицы с поддержкой доступа к элементам по индексу
     */
    class MatrixRow
    {
    private:

        /**
         * @brief columns - буфер колонок (элементов) строки матрицы
         */
        std::vector<T> columns;

        MatrixRow() { columns.reserve(DEFAULT_SIZE); }

    public:

        friend class Matrix;
        friend class Iterator;

        /**
         * @brief size - размер строки матрицы
         * @return количество заполненных элементов строки матрицы
         */
        size_t size () const
        {
            return std::accumulate(this->columns.begin(), this->columns.end(), 0, [](size_t a, auto& b){ return a + (b == DEFAULT_VALUE ? 0 : 1); });
        };

        /**
         * @brief operator [] - доступ к ссылке на ячейку элемента в строке матрицы через индекс
         * (динамически добавляет элементы строки матрицы, если индекс выходит за пределы)
         * @param column_index - индекс колонки в строке матрицы
         * @return ссылка на ячейку элемента в строке матрицы
         */
        T& operator[] (size_t column_index)
        {
            auto column_number = column_index+1;

            if ( column_number > this->columns.size() )
            {
                auto len = ( column_number - this->columns.size() );
                for ( size_t i = 0 ; i < len ; ++i )
                    this->columns.push_back(DEFAULT_VALUE);
            }

            return this->columns[column_index];
        }
    };

    /**
     * @brief rows - буфер строк матрицы
     */
    std::vector<MatrixRow> rows;

public:

    Matrix() { rows.reserve(DEFAULT_SIZE); }

    /**
     * @brief size - размер матрицы
     * @return количество заполненных элементов матрицы
     */
    size_t size () const
    {
        return std::accumulate(this->rows.begin(), this->rows.end(), 0, [](size_t a, auto& b){ return a+b.size(); });
    };

    /**
     * @brief operator [] - доступ к ссылке на ячейку элемента в строке матрицы через через индекс
     * (динамически добавляет элементы строки матрицы, если индекс выходит за пределы)
     * @param row_index - индекс строки матрицы
     * @return ссылка на строку матрицы
     */
    MatrixRow& operator[] (size_t row_index)
    {
        auto row_number = row_index+1;

        if ( row_number > this->rows.size() )
        {
            auto len = ( row_number - this->rows.size() );
            for ( size_t i = 0 ; i < len ; ++i )
                this->rows.push_back(MatrixRow{});
        }

        return this->rows[row_index];
    }

    /**
     * @brief The Iterator struct - тип итератора матрицы с поддержкой прямого перебора от начала к концу
     */
    struct Iterator
    {
        /**
         * @brief Iterator - конструктор итератора матрицы
         * @param matrix_ - ссылка экземпляр матрицы
         * @param rowcol_index - пара [ индекс_строки, индекс_колонки ] соответствующей ячейки матрицы
         */
        Iterator(Matrix& matrix_, std::pair<size_t,size_t> rowcol_index) : matrix(matrix_)
        {
            std::tie(this->row_index, this->col_index) = rowcol_index;
        }

        /**
         * @brief operator* - оператор непрямого обращения к значению итератора
         * @return кортеж [ индекес_строки, индекс_колонки, ссылка_на_ячейку_матрицы ]
         */
        std::tuple<size_t,size_t,T&> operator*() const
        {
            return { row_index, col_index, matrix[row_index][col_index] };
        }

        /**
         * @brief operator++ - постфиксный оператор перехода на следующий заполненный элемент матрицы
         * @return ссылка на итератор следующего заполненного элемента матрицы
         */
        Iterator& operator++()
        {
            ++col_index;

            while ( row_index < matrix.rows.size() )
            {
                for ( ; col_index < matrix.rows[row_index].columns.size() ; ++col_index )
                    if (matrix.rows[row_index][col_index] != DEFAULT_VALUE)
                        return *this;

                ++row_index;
                col_index = 0;
            }

            row_index = matrix.rows.size();
            col_index = 0;

            return *this;
        }

        friend bool operator!= (const Iterator& a, const Iterator& b)
        {
            return (a.row_index != b.row_index) || (a.col_index != b.col_index);
        }

    private:

        size_t row_index;
        size_t col_index;

        Matrix& matrix;
    };

    /**
     * @brief begin - метод получения начального итератора для прямого перебора матрицы
     * @return итератор начального заполненного элемента матрицы
     * (соответсвующего начальному заполненному элементу матрицы)
     */
    Iterator begin()
    {
        auto it = Iterator(*this, {0,0});

        auto [x,y,v] = *it;
        if (v == DEFAULT_VALUE) ++it;

        return (it);
    }

    /**
     * @brief end - метод получения конечного итератора для прямого перебора матрицы
     * @return итератор конечного элемента матрицы
     * (нулевой элемент строки, следующей за последней, содержащей хотя бы один заполненный элемент матрицы)
     */
    Iterator end()
    {
        return Iterator(*this, { this->rows.size(),0 });
    }
};
