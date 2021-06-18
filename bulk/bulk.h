/** \file */

#pragma once

#include <cassert>
#include <iostream>

#include <algorithm>
#include <iterator>
#include <vector>
#include <fstream>

#include <chrono>
using namespace std::chrono;

/*!
 * \brief print_bulk функция вывода пачки команд
 * \param out поток вывода
 * \param bulk пачка команд
 */
void print_bulk(std::ostream& out, const std::vector<std::string>& bulk)
{
    out << "bulk:";
    std::for_each(bulk.begin(), bulk.end(), [&out](auto& cmd){
        out << ' ' << cmd << ',';
    });
    out.seekp(-1, std::ios_base::end);
    out << std::endl;
}

/*!
 * \brief get_bulks рекурсивная фунция ввода пачек команд
 * \param in поток вввода команд
 * \param out поток вывода пачек
 * \param N количество команд в пачке
 * \param bulks общий буфер пачек команд
 * \param is_dynamic_bulk признак динамической ввода динамической пачки команд
 */
void get_bulks(std::istream& in, std::ostream& out, const int N, std::vector<std::vector<std::string>>& bulks, bool is_dynamic_bulk = false)
{
    int cmd_i = 0;
    std::ofstream bulk_f;
    std::vector<std::string> bulk;

    auto emplace_bulk = [&out, &cmd_i, &bulks, &bulk, &bulk_f]()
    {
        cmd_i = 0;
        bulk_f.close();
        print_bulk(out, bulk);
        bulks.emplace_back(std::move(bulk));
    };

    for ( std::string cmd; std::getline(in, cmd); )
    {
        if (cmd == "") break;

        if (cmd == "{")
        {
            if (!is_dynamic_bulk)
            {
                emplace_bulk();
                get_bulks(in, out, N, bulks, true);
            }
            continue;
        }
        else if (cmd == "}")
        {
            if (is_dynamic_bulk)
            {
                emplace_bulk();
                break;
            }
            continue;
        }

        if (!bulk_f.is_open())
            bulk_f.open("bulk" + std::to_string(duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count()) + ".log");
        bulk_f << cmd << std::endl;

        bulk.emplace_back(std::move(cmd));

        if (!is_dynamic_bulk) ++cmd_i;

        if (cmd_i >= N) emplace_bulk();
    }

    if (bulk.size() > 0)  emplace_bulk();
    if (bulk_f.is_open()) bulk_f.close();
}
