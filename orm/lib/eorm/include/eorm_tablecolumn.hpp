/** \file */

#pragma once

#include <cstdlib>

#include "eorm_table.hpp"
#include "eorm_defvalue.hpp"

namespace eorm {
namespace core {

template<typename T>
class TableColumn: public TableColumnBase
{
    static_assert(
        !( std::is_same<T, bool>::value
        && std::is_same<T, int>::value
        && std::is_same<T, float>::value
        && std::is_same<T, double>::value
        && std::is_same<T, std::time_t>::value
        && std::is_same<T, std::string>::value),
        "unknown data type"
    );

    const DefaultValue<T> columnDefValue{is_VALUE_REQUIRED()};

    std::vector<T> columnData;

public:

    TableColumn (const SqlName& name = {}, const TCS specs = TCS::EMPTY):
        TableColumnBase{getTableColumnType<T>(), specs, name}
    { }

    TableColumn (const TCS specs):
        TableColumnBase{getTableColumnType<T>(), specs}
    { }

    TableColumn (const SqlName& name, const TCS specs, const T& defValue):
        TableColumnBase{getTableColumnType<T>(), TCS(specs|TCS::DEFAULT), name},
        columnDefValue{defValue}
    { }

    TableColumn (const SqlName& name, const T& defValue):
        TableColumnBase{getTableColumnType<T>(), TCS::DEFAULT, name},
        columnDefValue{defValue}
    { }

    TableColumn (const TCS specs, const T& defValue):
        TableColumnBase{getTableColumnType<T>(), TCS(specs|TCS::DEFAULT), {}},
        columnDefValue{defValue}
    { }

    TableColumn (const T& defValue):
        TableColumnBase{getTableColumnType<T>(), TCS::DEFAULT, {}},
        columnDefValue{defValue}
    { }

    size_t getRowsCount() const override { return this->columnData.size(); }
    void clearRows() override { this->columnData.clear(); }

    void addRowStr(const char* rowStr) override
    {
        if (rowStr)
        {
            if constexpr (std::is_same<T,bool>::value || std::is_same<T,int>::value)
                addRowPtr(std::make_shared<T>(T(std::stoi(rowStr))));
            else if constexpr (std::is_floating_point<T>::value)
                addRowPtr(std::make_shared<T>(T(std::stod(rowStr))));
            else if constexpr (std::is_same<T,std::time_t>::value)
                addRowPtr(std::make_shared<T>(T(std::stol(rowStr))));
            else if constexpr (std::is_same<T,std::string>::value)
                addRowPtr(std::make_shared<T>(T(rowStr)));
        }
    }
    void addRowPtr(const std::shared_ptr<void> rowPtr) override
    {
        if (rowPtr)
            this->columnData.push_back(*(std::static_pointer_cast<T>(rowPtr)));
    }
    void removeLastValue() override { columnData.pop_back(); }

    T getRowValue(const size_t rowIndex) const
    {
        return (rowIndex < columnData.size()) ? columnData[rowIndex] : T();
    }

    SqlExpr getSqlDefaultValue () const override
    {
        std::ostringstream sqlBuf;

        sqlBuf << columnDefValue.getSqlValue();

        return { sqlBuf.str() };
    }

    SqlExpr getSqlRowValue(size_t rowIndex) const override
    {
        std::ostringstream sqlBuf;

        bool valueExists = false;

        if ((valueExists = rowIndex < getRowsCount()))
            sqlBuf << quoted_stringify(this->columnData[rowIndex]);

        if (!valueExists)
            if ((valueExists = this->is_NOT_NULL()))
                sqlBuf << columnDefValue.getSqlValue();

        if (!valueExists)
            sqlBuf << "NULL";

        return { sqlBuf.str() };
    }

    SqlExpr getSqlCreateTable() const override
    {
        std::ostringstream sqlBuf;

        sqlBuf << this->columnName;

        sqlBuf << " ";
        switch (this->columnType)
        {
            case TCT::INTEGER:  sqlBuf << "INTEGER";  break;
            case TCT::REAL:     sqlBuf << "REAL";     break;
            case TCT::DATETIME: sqlBuf << "DATETIME"; break;
            case TCT::TEXT:     sqlBuf << "TEXT";     break;
            default: throw "UNKNOWN DATA TYPE";
        }

        if (this->is_PRIMARY_KEY())     sqlBuf << " " << "PRIMARY KEY";
        if (this->is_AUTOINCREMENT())   sqlBuf << " " << "AUTOINCREMENT";
        if (this->is_NOT_NULL())        sqlBuf << " " << "NOT NULL";
        if (this->is_DEFAULT())         sqlBuf << " " << "DEFAULT";

        if (is_VALUE_REQUIRED())
            sqlBuf << " " << columnDefValue.getSqlValue();

        return { sqlBuf.str() };
    }

    inline TableColumnComparsionExpr operator>  (const SqlName& second) const { return TableColumnBase::operator>(second);  }
    inline TableColumnComparsionExpr operator<  (const SqlName& second) const { return TableColumnBase::operator<(second);  }
    inline TableColumnComparsionExpr operator== (const SqlName& second) const { return TableColumnBase::operator==(second); }
    inline TableColumnComparsionExpr operator!= (const SqlName& second) const { return TableColumnBase::operator!=(second); }
    inline TableColumnComparsionExpr IN         (const SqlName& second) const { return TableColumnBase::IN(second); }

    inline TableColumnComparsionExpr operator>  (const TableColumnBase& second) const { return TableColumnBase::operator>(second);  }
    inline TableColumnComparsionExpr operator<  (const TableColumnBase& second) const { return TableColumnBase::operator<(second);  }
    inline TableColumnComparsionExpr operator== (const TableColumnBase& second) const { return TableColumnBase::operator==(second); }
    inline TableColumnComparsionExpr operator!= (const TableColumnBase& second) const { return TableColumnBase::operator!=(second); }
    inline TableColumnComparsionExpr IN         (const TableColumnBase& second) const { return TableColumnBase::IN(second); }

    inline TableColumnComparsionExpr IN (const TableColumnComparsionExpr& second) const { return TableColumnBase::IN(second); }

    TableColumnComparsionExpr operator> (const T& second) const
    { return static_cast<TableColumnComparsionExpr>(*this) > static_cast<TableColumnComparsionExpr>(TableColumn{second});  }

    inline TableColumnComparsionExpr operator< (const T& second) const
    { return static_cast<TableColumnComparsionExpr>(*this) < static_cast<TableColumnComparsionExpr>(TableColumn{second});  }

    inline TableColumnComparsionExpr operator== (const T& second) const
    { return static_cast<TableColumnComparsionExpr>(*this) == static_cast<TableColumnComparsionExpr>(TableColumn{second}); }

    inline TableColumnComparsionExpr operator!= (const T& second) const
    { return static_cast<TableColumnComparsionExpr>(*this) != static_cast<TableColumnComparsionExpr>(TableColumn{second}); }
};

}
}
