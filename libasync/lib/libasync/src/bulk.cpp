/** \file */

#include "bulk.h"

void print_bulk(std::ostream& out, const std::vector<std::string>& bulk)
{
    out << "bulk:";
    bool comma = false;
    std::for_each(bulk.begin(), bulk.end(), [&out,&comma](auto& cmd){ out << (comma?", ":(comma=true," ")) << cmd; });
    out << std::endl;
}

void get_bulks(std::istream& in, const int N, std::queue<std::vector<std::string>>& bulks, bool is_dynamic_bulk)
{
    int cmd_i = 0;
    std::vector<std::string> bulk;

    auto emplace_bulk = [&cmd_i, &bulks, &bulk]()
    {
        cmd_i = 0;
        bulks.emplace(std::move(bulk));
    };

    std::ignore = N;
    std::ignore = is_dynamic_bulk;

    for ( std::string cmd; std::getline(in, cmd) && !cmd.empty(); )
    {
        if (cmd == "{")
        {
            if (!is_dynamic_bulk)
            {
                emplace_bulk();
                get_bulks(in, N, bulks, true);
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

        bulk.emplace_back(std::move(cmd));

        if (!is_dynamic_bulk)
            ++cmd_i;

        if (cmd_i >= N)
            emplace_bulk();
    }

    if (bulk.size() > 0)
        emplace_bulk();
}
