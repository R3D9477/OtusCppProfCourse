#pragma once

#include <ctime>
#include <memory>
#include <vector>
#include <cassert>
#include <sstream>

#include "eorm_sqlname.hpp"
#include "eorm_sqlexpr.hpp"
#include "eorm_tablecomparsionexpr.hpp"
#include "eorm_tablecolumntype.hpp"
#include "eorm_tablecolumnspecs.hpp"

namespace eorm {

struct Table
{
    struct TableColumnBase
    {
        std::shared_ptr<Table>   table;

        SqlName             columnName;
        TableColumnType     columnType;
        TableColumnSpecs    columnSpecs;

        virtual SqlExpr getSqlName (const char delimiter = '.') const
        {
            std::ostringstream sqlBuf;

            if (this->table != nullptr)
                sqlBuf << this->table->tableName << delimiter;
            sqlBuf << this->columnName;

            return { sqlBuf.str() };
        }
        virtual SqlExpr getSqlRowValue (size_t rowIndex = 0) const { std::ignore = rowIndex; return {}; }
        virtual SqlExpr getSqlCreateTable () const { return {}; }

        virtual size_t getRowsCount () const { return 0; }
        virtual void clearRows () {}
        virtual void addRowPtr (const std::shared_ptr<void> rowPtr) { std::ignore = rowPtr; }

        virtual bool is_PRIMARY_KEY     () const { return eorm::is_PRIMARY_KEY(this->columnSpecs);   }
        virtual bool is_AUTOINCREMENT   () const { return eorm::is_AUTOINCREMENT(this->columnSpecs); }
        virtual bool is_NOT_NULL        () const { return eorm::is_NOT_NULL(this->columnSpecs);      }
        virtual bool is_DEFAULT         () const { return eorm::is_DEFAULT(this->columnSpecs);       }

        operator TableColumnComparsionExpr() const
        {
            return
                this->columnName.value == ""
                ? TableColumnComparsionExpr(this->getSqlRowValue())
                : TableColumnComparsionExpr(this->getSqlName());
        }

        inline TableColumnComparsionExpr operator> (const SqlName& second) const
        { return static_cast<TableColumnComparsionExpr>(*this) > TableColumnComparsionExpr(second);   }

        inline TableColumnComparsionExpr operator< (const SqlName& second) const
        { return static_cast<TableColumnComparsionExpr>(*this) < TableColumnComparsionExpr(second);   }

        inline TableColumnComparsionExpr operator== (const SqlName& second) const
        { return static_cast<TableColumnComparsionExpr>(*this) == TableColumnComparsionExpr(second);  }

        inline TableColumnComparsionExpr operator!= (const SqlName& second) const
        { return static_cast<TableColumnComparsionExpr>(*this) != TableColumnComparsionExpr(second);  }

        inline TableColumnComparsionExpr IN (const SqlName& second) const
        { return static_cast<TableColumnComparsionExpr>(*this).IN(TableColumnComparsionExpr(second)); }

        inline TableColumnComparsionExpr operator> (const TableColumnBase& second) const
        { return static_cast<TableColumnComparsionExpr>(*this) > static_cast<TableColumnComparsionExpr>(second);   }

        inline TableColumnComparsionExpr operator< (const TableColumnBase& second) const
        { return static_cast<TableColumnComparsionExpr>(*this) < static_cast<TableColumnComparsionExpr>(second);   }

        inline TableColumnComparsionExpr operator== (const TableColumnBase& second) const
        { return static_cast<TableColumnComparsionExpr>(*this) == static_cast<TableColumnComparsionExpr>(second);  }

        inline TableColumnComparsionExpr operator!= (const TableColumnBase& second) const
        { return static_cast<TableColumnComparsionExpr>(*this) != static_cast<TableColumnComparsionExpr>(second);  }

        inline TableColumnComparsionExpr IN (const TableColumnBase& second) const
        { return static_cast<TableColumnComparsionExpr>(*this).IN(static_cast<TableColumnComparsionExpr>(second)); }

        inline TableColumnComparsionExpr IN (const TableColumnComparsionExpr& second) const
        { return static_cast<TableColumnComparsionExpr>(*this).IN(second); }
    };

    struct TableRowSort
    {
        enum TableRowSortType
        {
            DEFAULT,
            ASC,
            DESC
        };

        Table::TableColumnBase& column;
        TableRowSortType sortType;
    };

    struct TableJoin
    {
        enum TableJoinType
        {
            INNER,
            OUTER,
            LEFT,
            RIGHT
        };

        TableJoinType joinType;
        SqlName tableName;
        TableColumnComparsionExpr joinCondition;

        TableJoin(
            const TableJoinType joinType_,
            const SqlName& tableName_,
            const TableColumnComparsionExpr& joinCondition_ = {}):
            joinType{joinType_},
            tableName{tableName_},
            joinCondition{joinCondition_}
        { }

        TableJoin(
            const TableJoinType joinType_,
            const Table& table_,
            const TableColumnComparsionExpr& joinCondition_ = {}):
            TableJoin{joinType_,table_.tableName,joinCondition_}
        { }

        friend std::ostream& operator<< (std::ostream& os, const TableJoin& tableJoin)
        {
            switch (tableJoin.joinType)
            {
                case INNER: os << "INNER" ; break;
                case OUTER: os << "OUTER" ; break;
                case LEFT:  os << "LEFT"  ; break;
                case RIGHT: os << "RIGHT" ; break;
                default: return os;
            }
            os << " JOIN " << tableJoin.tableName;
            if (!tableJoin.joinCondition.isEmpty())
                os << " ON " << tableJoin.joinCondition;
            return os;
        }
    };

    SqlName tableName;
    std::vector<std::shared_ptr<TableColumnBase>> tableColumns;

    template<typename... T>
    Table(const SqlName& name) : tableName(name) { }

    template <typename... T>
    void registerColumns(T&&... columns) { (void)std::initializer_list<int>{(registerColumn((std::forward<T>(columns))), void(), 0)...}; }

    template <typename T>
    void registerColumn(T& tc)
    {
        if (tc.columnName == "")
            tc.columnName = "COLUMN_" + std::to_string(tableColumns.size());
        tc.table = std::shared_ptr<Table>(this,[](Table*){});
        tableColumns.push_back(std::shared_ptr<T>(&tc,[](T*){}));
    }

    SqlExpr getSqlTableCreate() const
    {
        std::ostringstream sqlBuf;

        sqlBuf << "CREATE TABLE " << this->tableName << " (";

        for (auto& c: tableColumns)
            sqlBuf << std::endl << c->getSqlCreateTable() << ',';

        sqlBuf.seekp(-1, sqlBuf.cur);
        sqlBuf << std::endl << ");";

        return { sqlBuf.str() };
    }

    SqlExpr getSqlTableDrop() const
    {
        std::ostringstream sqlBuf;
        sqlBuf << "DROP TABLE " << this->tableName << ';';
        return { sqlBuf.str() };
    }

    SqlExpr getSqlRowsInsert(const bool insertValues = true) const
    {
        std::ostringstream sqlBuf, sqlBufValues;

        for (size_t rowIndex = 0; rowIndex < this->getMaxRowsCount(); ++rowIndex)
        {
            sqlBuf << "INSERT INTO " << this->tableName << " (";
            sqlBufValues << "VALUES (";

            for (auto& c: tableColumns)
                if (!c->is_AUTOINCREMENT())
                {
                    sqlBuf << std::endl << c->getSqlName() << ',';
                    sqlBufValues << std::endl;
                    if (insertValues) sqlBufValues << c->getSqlRowValue(rowIndex);
                    else sqlBufValues << ":" << c->getSqlName('_') << '_' << rowIndex;
                    sqlBufValues << ',';
                }

            sqlBuf.seekp(-1, sqlBuf.cur);
            sqlBuf << std::endl << ')';

            sqlBufValues.seekp(-1, sqlBuf.cur);
            sqlBufValues << std::endl << ')';

            sqlBuf << std::endl << sqlBufValues.str() << ';' << std::endl;

            sqlBufValues.str("");
            sqlBufValues.clear();
        }

        sqlBuf.seekp(-1, sqlBuf.cur);
        sqlBuf << '\0';

        return { sqlBuf.str() };
    }

    SqlExpr getSqlRowsUpdate(
        const TableColumnComparsionExpr& condition   = {},
        const size_t count  = 0,
        const size_t offset = 0) const
    {
        std::ostringstream sqlBuf;

        for (size_t rowIndex = 0; rowIndex < this->getMaxRowsCount(); ++rowIndex)
        {
            sqlBuf << "UPDATE " << this->tableName << " SET";

            for (auto& c: tableColumns)
                if (!c->is_AUTOINCREMENT())
                    sqlBuf << std::endl << c->getSqlName() << '=' << c->getSqlRowValue(rowIndex) << ',';

            sqlBuf.seekp(-1, sqlBuf.cur);
            sqlBuf<<'\0';

            if (condition.expr != "")
                sqlBuf << std::endl << "WHERE " << condition;

            if (count > 0)
                sqlBuf << std::endl << "LIMIT " << count;

            if (offset > 0)
                sqlBuf << std::endl << "OFFSET " << offset;

            sqlBuf << ';' << std::endl;
        }

        sqlBuf.seekp(-1, sqlBuf.cur);
        sqlBuf << '\0';

        return { sqlBuf.str() };
    }

    SqlExpr getSqlRowsSelect(
        const std::vector<std::reference_wrapper<TableColumnBase>>& sc = {},
        const std::initializer_list<TableJoin>& tableJoins             = {},
        const TableColumnComparsionExpr& condition    = {},
        const std::vector<TableRowSort>& sort = {},
        const size_t count  = 0,
        const size_t offset = 0) const
    {
        std::ostringstream sqlBuf;

        sqlBuf << "SELECT";

        if (sc.size() > 0)
        {
            for (auto& c: sc)
                sqlBuf << std::endl << c.get().getSqlName() << ',';
            sqlBuf.seekp(-1, sqlBuf.cur);
        }
        else sqlBuf << std::endl << this->tableName << ".*";

        sqlBuf << std::endl << "FROM " << this->tableName;

        for (auto& join: tableJoins)
            sqlBuf << std::endl << join;

        if (condition.expr != "")
            sqlBuf << std::endl << "WHERE " << condition;

        if (sort.size() > 0)
        {
            sqlBuf << std::endl << "ORDER BY";
            for (auto& s: sort)
            {
                sqlBuf << " " << s.column.getSqlName();
                switch (s.sortType)
                {
                    case TableRowSort::TableRowSortType::ASC:
                        sqlBuf << " ASC";
                        break;
                    case TableRowSort::TableRowSortType::DESC:
                        sqlBuf << " DESC";
                        break;
                    default:
                        break;
                }
                sqlBuf << ',';
            }
            sqlBuf.seekp(-1, sqlBuf.cur);
            sqlBuf << '\0';
        }

        if (count > 0)
            sqlBuf << std::endl << "LIMIT " << count;

        if (offset > 0)
            sqlBuf << std::endl << "OFFSET " << offset;

        sqlBuf << ';';

        return { sqlBuf.str() };
    }

    SqlExpr getSqlRowsDelete(
        const TableColumnComparsionExpr& condition   = {},
        const size_t count  = 0,
        const size_t offset = 0) const
    {
        std::ostringstream sqlBuf;

        sqlBuf << "DELETE FROM " << this->tableName;

        if (condition.expr != "")
            sqlBuf << std::endl << "WHERE " << condition;

        if (count > 0)
            sqlBuf << std::endl << "LIMIT " << count;

        if (offset > 0)
            sqlBuf << std::endl << "OFFSET " << offset;

        sqlBuf << ';';

        return { sqlBuf.str() };
    }

    void clearRows()
    {
        for (auto& cl: this->tableColumns)
            cl->clearRows();
    }

    size_t getMaxRowsCount() const
    {
        size_t rows = 0;
        for (auto& cl: this->tableColumns)
            if (rows < cl->getRowsCount())
                rows = cl->getRowsCount();
        return rows;
    }

    template<typename... T>
    void addRow(T&&... t)
    {
        assert(this->tableColumns.size() == sizeof...(T));

        size_t colBuf = 0;
        (void)std::initializer_list<int>{(addRow(colBuf++, std::forward<T>(t)), void(), 0)...};
    }

    template <typename T>
    void addRow(size_t colIndex, const T val)
    {
        assert(colIndex < this->tableColumns.size());

        auto buf = stdstr_or_type(val);

        assert(getTableColumnType<typeof(buf)>() == this->tableColumns[colIndex]->columnType);

        this->tableColumns[colIndex]->addRowPtr(std::make_shared<typeof(buf)>(buf));
    }
};

typedef Table::TableRowSort                     TRS;
typedef Table::TableJoin::TableJoinType         TJT;
typedef Table::TableRowSort::TableRowSortType   TRST;

}
