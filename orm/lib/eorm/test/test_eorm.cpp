#define BOOST_TEST_MODULE test_eorm_core

#include <sstream>

#include <boost/test/unit_test.hpp>

#include "eorm.h"

using namespace eorm;

struct Table1Info: Table
{
    TableColumn<int>          ID = { SqlName("ID"), TCS(TCS::PRIMARY_KEY|TCS::AUTOINCREMENT) };
    TableColumn<double>       C1 = { SqlName("C1"), TCS::DEFAULT, 3.14 };
    TableColumn<std::time_t>  C2 = { SqlName("C2"), TCS(TCS::DEFAULT|TCS::NOT_NULL) };
    TableColumn<std::string>  C3 = { TCS::DEFAULT };
    TableColumn<float>        C4 = { 5.65f };
    TableColumn<float>        C5;

    Table1Info(): Table(SqlName("Table1")) { registerColumns(ID, C1, C2, C3, C4, C5); }
};

struct Table2Info: Table
{
    TableColumn<int>          ID = { SqlName("ID"), TCS(TCS::PRIMARY_KEY|TCS::AUTOINCREMENT) };
    TableColumn<double>       C1 = { SqlName("C1"), TCS::DEFAULT, 3.14 };
    TableColumn<std::time_t>  C2 = { SqlName("C2"), TCS(TCS::DEFAULT|TCS::NOT_NULL) };
    TableColumn<std::string>  C3 = { TCS::DEFAULT };
    TableColumn<float>        C4 = { 5.65f };
    TableColumn<float>        C5;

    Table2Info(): Table(SqlName("Table2")) { registerColumns(ID, C1, C2, C3, C4, C5); }
};

struct Table3Info: Table
{
    TableColumn<int>          ID = { SqlName("ID"), TCS(TCS::PRIMARY_KEY|TCS::AUTOINCREMENT) };
    TableColumn<double>       C1 = { SqlName("C1"), TCS::DEFAULT, 3.14 };
    TableColumn<std::time_t>  C2 = { SqlName("C2"), TCS(TCS::DEFAULT|TCS::NOT_NULL) };
    TableColumn<std::string>  C3 = { TCS::DEFAULT };
    TableColumn<float>        C4 = { 5.65f };
    TableColumn<float>        C5;

    Table3Info(): Table(SqlName("Table3")) { registerColumns(ID, C1, C2, C3, C4, C5); }
};

struct Table4Info: Table
{
    TableColumn<int>          ID = { SqlName("ID"), TCS(TCS::PRIMARY_KEY|TCS::AUTOINCREMENT) };
    TableColumn<double>       C1 = { SqlName("C1"), TCS::DEFAULT, 3.14 };
    TableColumn<std::time_t>  C2 = { SqlName("C2"), TCS(TCS::DEFAULT|TCS::NOT_NULL) };
    TableColumn<std::string>  C3 = { TCS::DEFAULT };
    TableColumn<float>        C4 = { 5.65f };
    TableColumn<float>        C5;

    Table4Info(): Table(SqlName("Table4")) { registerColumns(ID, C1, C2, C3, C4, C5); }
};

struct Table5Info: Table
{
    TableColumn<int>          ID = { SqlName("ID"), TCS(TCS::PRIMARY_KEY|TCS::AUTOINCREMENT) };
    TableColumn<double>       C1 = { SqlName("C1"), TCS::DEFAULT, 3.14 };
    TableColumn<std::time_t>  C2 = { SqlName("C2"), TCS(TCS::DEFAULT|TCS::NOT_NULL) };
    TableColumn<std::string>  C3 = { TCS::DEFAULT };
    TableColumn<float>        C4 = { 5.65f };
    TableColumn<float>        C5;

    Table5Info(): Table(SqlName("Table5")) { registerColumns(ID, C1, C2, C3, C4, C5); }
};

struct Table6Info: Table
{
    TableColumn<int>          ID = { SqlName("ID"), TCS(TCS::PRIMARY_KEY|TCS::AUTOINCREMENT) };
    TableColumn<double>       C1 = { SqlName("C1"), TCS::DEFAULT, 3.14 };
    TableColumn<std::time_t>  C2 = { SqlName("C2"), TCS(TCS::DEFAULT|TCS::NOT_NULL) };
    TableColumn<std::string>  C3 = { TCS::DEFAULT };
    TableColumn<float>        C4 = { 5.65f };
    TableColumn<float>        C5;

    Table6Info(): Table(SqlName("Table6")) { registerColumns(ID, C1, C2, C3, C4, C5); }
};

BOOST_AUTO_TEST_SUITE(test_eorm_core)

BOOST_AUTO_TEST_CASE(test_generate_sql_create_and_drop_table)
{
    Table1Info Table1;

    std::stringstream ValidSql(R"(CREATE TABLE Table1 (
ID INTEGER PRIMARY KEY AUTOINCREMENT,
C1 REAL DEFAULT 3.140000,
C2 DATETIME NOT NULL DEFAULT CURRENT_TIMESTAMP,
COLUMN_3 TEXT DEFAULT NULL,
COLUMN_4 REAL,
COLUMN_5 REAL
);)");

    auto ResultSql = Table1.getSqlTableCreate();

    BOOST_CHECK (ValidSql.str() == ResultSql.str());

    ValidSql.str("DROP TABLE Table1;");

    ResultSql = Table1.getSqlTableDrop();

    BOOST_CHECK (ValidSql.str() == ResultSql.str());
}

BOOST_AUTO_TEST_CASE(test_generate_sql_insert_values)
{
    Table1Info Table1;

    Table1.clearRows();
    Table1.addRow(0, 1.23, std::time_t(1631730839), "XXX", 3.14f, 31.4f);
    Table1.addRow(1, 45.6, std::time_t(1631730839), "YYY", 56.7f, 8.88f);

    std::stringstream ValidSql(R"(INSERT INTO Table1 (
Table1.C1,
Table1.C2,
Table1.COLUMN_3,
Table1.COLUMN_4,
Table1.COLUMN_5
)
VALUES (
1.230000,
1631730839,
'XXX',
3.140000,
31.400000
);
INSERT INTO Table1 (
Table1.C1,
Table1.C2,
Table1.COLUMN_3,
Table1.COLUMN_4,
Table1.COLUMN_5
)
VALUES (
45.600000,
1631730839,
'YYY',
56.700001,
8.880000
);)");

    auto ResultSql = Table1.getSqlRowsInsert();

    BOOST_CHECK (ValidSql.str() == ResultSql.str());
}

BOOST_AUTO_TEST_CASE(test_generate_sql_update)
{
    Table1Info Table1;

    Table1.clearRows();
    Table1.addRow(0, 45.6, std::time_t(1631730839), "YYY", 56.7f, 8.88f);

    std::stringstream ValidSql(R"(UPDATE Table1 SET
Table1.C1=45.600000,
Table1.C2=1631730839,
Table1.COLUMN_3='YYY',
Table1.COLUMN_4=56.700001,
Table1.COLUMN_5=8.880000
WHERE ((Table1.ID) = (0));)");

    auto ResultSql = Table1.getSqlRowsUpdate( Table1.ID == 0 );

    BOOST_CHECK ( ValidSql.str() == ResultSql.str() );

    Table1.clearRows();
    Table1.addRow(1, 1.23, std::time_t(1631730839), "XXX", 3.14f, 31.4f);

    ValidSql.str(R"(UPDATE Table1 SET
Table1.C1=1.230000,
Table1.C2=1631730839,
Table1.COLUMN_3='XXX',
Table1.COLUMN_4=3.140000,
Table1.COLUMN_5=31.400000
WHERE ((Table1.ID) = (1));)");

    ResultSql = Table1.getSqlRowsUpdate( Table1.ID == 1 );

    BOOST_CHECK ( ValidSql.str() == ResultSql.str() );
}

BOOST_AUTO_TEST_CASE(test_generate_sql_delete_all)
{
    Table1Info Table1;

    std::stringstream ValidSql(R"(DELETE FROM Table1;)");

    auto ResultSql = Table1.getSqlRowsDelete();

    BOOST_CHECK (ValidSql.str() == ResultSql.str());
}

BOOST_AUTO_TEST_CASE(test_generate_sql_delete_where)
{
    Table1Info Table1;

    std::stringstream ValidSql(R"(DELETE FROM Table1
WHERE ((Table1.ID) < (100));)");

    auto ResultSql = Table1.getSqlRowsDelete( Table1.ID < 100 );

    BOOST_CHECK (ValidSql.str() == ResultSql.str());
}

BOOST_AUTO_TEST_CASE(test_generate_sql_select_all)
{
    Table1Info Table1;

    std::stringstream ValidSql(R"(SELECT
Table1.*
FROM Table1;)");

    auto ResultSql = Table1.getSqlRowsSelect();

    BOOST_CHECK (ValidSql.str() == ResultSql.str());
}

BOOST_AUTO_TEST_CASE(test_generate_sql_select_all_specific_columns)
{
    Table1Info Table1;

    std::stringstream ValidSql(R"(SELECT
Table1.ID,
Table1.C2,
Table1.COLUMN_3,
Table1.COLUMN_5
FROM Table1;)");

    auto ResultSql = Table1.getSqlRowsSelect({ Table1.ID, Table1.C2, Table1.C3, Table1.C5 });

    BOOST_CHECK (ValidSql.str() == ResultSql.str());
}

BOOST_AUTO_TEST_CASE(test_generate_sql_select_where)
{
    Table1Info Table1;

    std::stringstream ValidSql(R"(SELECT
Table1.*
FROM Table1
WHERE ((Table1.ID) > (100));)");

    auto ResultSql = Table1.getSqlRowsSelect( {}, {}, ( Table1.ID > 100 ) );

    BOOST_CHECK (ValidSql.str() == ResultSql.str());
}

BOOST_AUTO_TEST_CASE(test_generate_sql_select_where_and_order)
{
    Table1Info Table1;

    std::stringstream ValidSql(R"(SELECT
Table1.*
FROM Table1
WHERE ((Table1.ID) > (100))
ORDER BY Table1.ID ASC, Table1.C1, Table1.COLUMN_5 DESC;)");

    auto ResultSql = Table1.getSqlRowsSelect( {}, {},
        ( Table1.ID > 100 ),
        {
            { Table1.ID, TRST::ASC },
            { Table1.C1, TRST::DEFAULT },
            { Table1.C5, TRST::DESC }
        }
    );

    BOOST_CHECK (ValidSql.str() == ResultSql.str());
}

BOOST_AUTO_TEST_CASE(test_generate_sql_select_all_limit_and_offset)
{
    Table1Info Table1;

    std::stringstream ValidSql(R"(SELECT
Table1.*
FROM Table1
LIMIT 300
OFFSET 25;)");

    auto ResultSql = Table1.getSqlRowsSelect( {}, {}, {}, {}, 300, 25 );

    BOOST_CHECK (ValidSql.str() == ResultSql.str());
}

BOOST_AUTO_TEST_CASE(test_generate_sql_select_join)
{
    Table1Info Table1;
    Table2Info Table2;

    std::stringstream ValidSql(R"(SELECT
Table1.*
FROM Table1
INNER JOIN Table1 ON ((Table1.C1) = (Table2.C1));)");

    auto ResultSql = Table1.getSqlRowsSelect( {},
        {
            { TJT::INNER, Table1, { Table1.C1 == Table2.C1 } }
        }
    );

    BOOST_CHECK (ValidSql.str() == ResultSql.str());
}

BOOST_AUTO_TEST_CASE(test_generate_sql_select_complex_condition)
{
    Table1Info Table1;
    Table2Info Table2;
    Table3Info Table3;
    Table4Info Table4;
    Table5Info Table5;
    Table6Info Table6;

    std::stringstream ValidSql(R"(SELECT
Table1.ID,
Table1.C1,
Table1.C2
FROM Table1
INNER JOIN Table2 ON ((Table1.ID) = (Table2.ID))
OUTER JOIN Table3 ON ((Table1.C2) = (Table2.C2))
LEFT JOIN Table4
RIGHT JOIN Table5
WHERE (((((Table1.C1) = (3.140000)) AND ((Table1.C2) > (Table1.COLUMN_3))) AND ((Table1.COLUMN_3) = MyCustomColumn)) AND ((Table1.COLUMN_4) IN (SELECT
Table6.*
FROM Table6)))
ORDER BY Table1.C1, Table1.C2 ASC, Table1.COLUMN_3 DESC
LIMIT 300
OFFSET 25;)");

    auto ResultSql = Table1.getSqlRowsSelect(
        {
            Table1.ID,
            Table1.C1,
            Table1.C2
        },
        {
            { TJT::INNER, Table2, { Table1.ID == Table2.ID } },
            { TJT::OUTER, Table3, { Table1.C2 == Table2.C2 } },
            { TJT::LEFT,  Table4 },
            { TJT::RIGHT, Table5 },
        },
               Table1.C1 ==  3.14
            && Table1.C2 >   Table1.C3
            && Table1.C3 ==  SqlName("MyCustomColumn")
            && Table1.C4.IN( Table6.getSqlRowsSelect() )
        ,
        {
            { Table1.C1, TRST::DEFAULT },
            { Table1.C2, TRST::ASC     },
            { Table1.C3, TRST::DESC    }
        },
        300,
        25
    );

    BOOST_CHECK (ValidSql.str() == ResultSql.str());
}

}

