/** \file */

#include <cassert>
#include <iostream>

#include "matrix.h"

/**
 * @brief main is the 'entry point' of matrix application.
 * @return result of application execution, non-zero value is code of the error.
 */
int main()
{
    Matrix<int, 0> matrix;

    for (size_t i = 0; i < 10; ++i)
        matrix[i][i] = int(i);

    for (size_t i = 0; i < 10; ++i)
        matrix[9-i][i] = int(9-i);

    std::cout << "[1,1]-[8,8]:" << std::endl << "   ";

    size_t row_i = 1;
    auto it      = Matrix<int,0>::Iterator(matrix, {1,1});
    auto it_end  = Matrix<int,0>::Iterator(matrix, {9,0});
    for ( ; it != it_end ; ++it )
    {
        auto [x,y,v] = *it;

        if (x != row_i)
        {
            row_i = x;
            std::cout << std::endl << "   ";
        }

        std::cout << v << ' '; //std::cout << x << ',' << y << ':' << v << ' ';
    }
    std::cout << std::endl;

    std::cout << std::endl << "Size:" << matrix.size() << std::endl;

    std::cout << std::endl << "Elements:" << std::endl;
    for (auto c: matrix)
    {
        size_t x;
        size_t y;
        int    v;
        std::tie(x,y,v) = c;
        std::cout << "   " << x << y << v << std::endl;
    }

    std::cout << std::endl << "((matrix[100][100]=314)=0)=217: ";

    ((matrix[100][100]=314)=0)=217;
    std::cout << matrix[100][100] << std::endl;

    return 0;
}
