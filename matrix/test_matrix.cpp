/** \file */

#define BOOST_TEST_MODULE test_matrix

#include <boost/test/unit_test.hpp>

#include <array>
#include <tuple>

#include "matrix2.h"

#define DEFAULT_VALUE 0

BOOST_AUTO_TEST_SUITE(test_matrix)

/*!
 *  тест метода size() для пустой матрицы
 */
BOOST_AUTO_TEST_CASE(test_matrix_init_size)
{
    Matrix<int,DEFAULT_VALUE> matrix;

    BOOST_CHECK(matrix.size() == 0);
}

/*!
 *  тест двумерной матрицы с пустыми элементами
 */
BOOST_AUTO_TEST_CASE(test_matrix_empty_elements)
{
    Matrix<int,DEFAULT_VALUE> matrix;

    BOOST_CHECK(matrix[0][0] == DEFAULT_VALUE);
    BOOST_CHECK(matrix[1][1] == DEFAULT_VALUE);
    BOOST_CHECK(matrix[2][2] == DEFAULT_VALUE);

    BOOST_CHECK(matrix[100][100] == DEFAULT_VALUE);
}

/*!
 *  тест двумерной матрицы с 3 заполненными элементами
 */
BOOST_AUTO_TEST_CASE(test_matrix_3_elements)
{
    Matrix<int,DEFAULT_VALUE> matrix;

    matrix[0][0] = 1;
    matrix[1][1] = 2;
    matrix[2][2] = 3;

    BOOST_CHECK(matrix.size() == 3);

    BOOST_CHECK(matrix[0][0] == 1);
    BOOST_CHECK(matrix[1][1] == 2);
    BOOST_CHECK(matrix[2][2] == 3);
}

/*!
 *  тест очистки двумерной матрицы
 */
BOOST_AUTO_TEST_CASE(test_matrix_clear_elements)
{
    Matrix<int,DEFAULT_VALUE> matrix;

    matrix[0][0] = 1;
    matrix[1][1] = 2;
    matrix[2][2] = 3;

    BOOST_CHECK(matrix.size() == 3);

    BOOST_CHECK(matrix[0][0] == 1);
    BOOST_CHECK(matrix[1][1] == 2);
    BOOST_CHECK(matrix[2][2] == 3);

    matrix[0][0] = DEFAULT_VALUE;
    matrix[1][1] = DEFAULT_VALUE;

    BOOST_CHECK(matrix.size() == 1);

    BOOST_CHECK(matrix[2][2] == 3);
}

/*!
 *  тест итерации двумерной матрицы
 */
BOOST_AUTO_TEST_CASE(test_matrix_fwd_iteration)
{
    std::array<std::tuple<size_t,size_t,int>, 6> valid_result = { {
        { 0, 0, 1 },
        { 1, 1, 2 },
        { 2, 2, 3 },
        { 100, 0,  100 },
        { 200, 10, 200 },
        { 300, 20, 300 }
    } };

    Matrix<int,DEFAULT_VALUE> matrix;

    for (auto [x,y,v]: valid_result)
        matrix[x][y] = v;

    BOOST_CHECK(matrix.size() == 6);

    size_t res_index = 0;
    for (auto [x,y,v]: matrix)
    {
        auto [valid_x, valid_y, valid_v ] = valid_result[res_index];

        BOOST_CHECK(x == valid_x);
        BOOST_CHECK(y == valid_y);
        BOOST_CHECK(v == valid_v);

        ++res_index;
    }
}

/*!
 *  тест каннонической реализации оператор присваивания
 */
BOOST_AUTO_TEST_CASE(test_matrix_cannonical_assignment)
{
    Matrix<int,DEFAULT_VALUE> matrix;

    matrix[100][100]=314;

    BOOST_CHECK(matrix.size() == 1);
    BOOST_CHECK(matrix[100][100] == 314);

    (matrix[100][100]=314)=0;

    BOOST_CHECK(matrix.size() == 0);
    BOOST_CHECK(matrix[100][100] == 0);

    ((matrix[100][100]=314)=0)=217;

    BOOST_CHECK(matrix.size() == 1);
    BOOST_CHECK(matrix[100][100] == 217);
}

/*!
 *  текст заполнения и очистки ячеек многомерной матрицы
 */
BOOST_AUTO_TEST_CASE(test_matrix_N_matrix)
{
    Matrix<int,DEFAULT_VALUE> matrix;

    matrix[1]=10;
    BOOST_CHECK(matrix.size() == 1);
    BOOST_CHECK(matrix[1] != DEFAULT_VALUE);
    BOOST_CHECK(matrix[1] == 10);

    matrix[2][2]=200;
    BOOST_CHECK(matrix.size() == 2);
    BOOST_CHECK(matrix[2][2] != DEFAULT_VALUE);
    BOOST_CHECK(matrix[2][2] == 200);

    matrix[3][3][3]=3000;
    BOOST_CHECK(matrix.size() == 3);
    BOOST_CHECK(matrix[3][3][3] != DEFAULT_VALUE);
    BOOST_CHECK(matrix[3][3][3] == 3000);

    matrix[100][100][100][100]=1000000;
    BOOST_CHECK(matrix.size() == 4);
    BOOST_CHECK(matrix[100][100][100][100] != DEFAULT_VALUE);
    BOOST_CHECK(matrix[100][100][100][100] == 1000000);

    matrix[100][100][100][100]=DEFAULT_VALUE;
    BOOST_CHECK(matrix.size() == 3);
    BOOST_CHECK(matrix[100][100][100][100] == DEFAULT_VALUE);
    BOOST_CHECK(matrix[100][100][100][100] != 1000000);

    matrix[3][3][3]=DEFAULT_VALUE;
    BOOST_CHECK(matrix.size() == 2);
    BOOST_CHECK(matrix[3][3][3] == DEFAULT_VALUE);
    BOOST_CHECK(matrix[3][3][3] != 3000);

    matrix[2][2]=DEFAULT_VALUE;
    BOOST_CHECK(matrix.size() == 1);
    BOOST_CHECK(matrix[2][2] == DEFAULT_VALUE);
    BOOST_CHECK(matrix[2][2] != 200);

    matrix[1]=DEFAULT_VALUE;
    BOOST_CHECK(matrix.size() == 0);
    BOOST_CHECK(matrix[1] == DEFAULT_VALUE);
    BOOST_CHECK(matrix[1] != 10);
}

}
