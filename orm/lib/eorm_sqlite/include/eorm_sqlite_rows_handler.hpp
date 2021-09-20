#pragma once

#include "eorm_sqlite_database.hpp"

namespace eorm   {
namespace sqlite {

class SqliteRowsCountHandler: public SqliteRowsHandlerBase
{
    const std::function<void(const size_t)> m_handler{nullptr};

public:

    SqliteRowsCountHandler(decltype(m_handler) handler): m_handler{handler} { }

    int rowHandler(int columns, char** data, char** names) override
    {
        if ((columns == 1) && data && names && m_handler)
        {
            m_handler(std::stol(data[0]));
            return 0;
        }

        return -1;
    }
};

class SqliteRowsHandler: public SqliteRowsHandlerBase
{
    size_t rowIndex = 0;

    const std::initializer_list<Table>& m_tables;
    const std::function<void(const size_t)> m_handler{nullptr};

public:

    SqliteRowsHandler(decltype(m_tables)& tables, decltype(m_handler) handler):
        m_tables{tables}, m_handler{handler}
    {}

    int rowHandler(int columns, char** data, char** names) override
    {
        if (columns && data && names && m_handler)
        {
            std::string cname;
            char snamed = '\0';

            auto comparator = [&cname,snamed](std::shared_ptr<TableColumnBase> tbl) {
                return tbl->getSqlName(snamed) == cname;
            };

            for (int i = 0; i < columns; ++i)
            {
                cname  = std::string(names[i]);
                snamed = (cname.find('.')!=std::string::npos)?'.':'\0';

                for (auto& t: m_tables)
                {
                    auto a = std::find_if(t.begin(), t.end(), comparator);

                    if (a != t.end())
                        if (auto colptr = (*a))
                            colptr->addRowStr(data[i]);
                }
            }

            m_handler(rowIndex++);
            return 0;
        }

        return -1;
    }
};

}
}
