/** \file */

#pragma once

#include <iostream>
#include <algorithm>
#include <iterator>
#include <queue>

/*!
 * \brief print_bulk функция вывода пачки команд
 * \param out поток вывода
 * \param bulk пачка команд
 */
void print_bulk(std::ostream& out, const std::vector<std::string>& bulk);

/*!
 * \brief get_bulks рекурсивная фунция ввода пачек команд
 * \param in поток вввода команд
 * \param out поток вывода пачек
 * \param N количество команд в пачке
 * \param bulks общий буфер пачек команд
 * \param is_dynamic_bulk признак динамической ввода динамической пачки команд
 */
void get_bulks(
    std::istream& in,
    const int N,
    std::queue<std::vector<std::string>>& bulks,
    bool is_dynamic_bulk = false
);
