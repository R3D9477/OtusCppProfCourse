/** \file */

#pragma once

#include <cstdlib>

#include "eorm_table.hpp"

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

    std::vector<T> data;

public:

    TableColumn (const SqlName& name = {}, TCS specs = TCS::EMPTY)
    {
        this->columnType  = getTableColumnType<T>();
        this->columnName  = name;
        this->columnSpecs = specs;
    }
    TableColumn (TCS specs): TableColumn({}, specs) {}
    TableColumn (const SqlName& name, const TCS specs, const T defValue): TableColumn(name, specs)  { this->data.push_back(defValue); }
    TableColumn (const SqlName& name, const T defValue): TableColumn(name)                          { this->data.push_back(defValue); }
    TableColumn (const TCS specs, const T defValue): TableColumn({}, specs)                         { this->data.push_back(defValue); }
    TableColumn (const T defValue): TableColumn()                                                   { this->data.push_back(defValue); }

    size_t getRowsCount() const override { return this->data.size(); }
    void clearRows() override { this->data.clear(); }

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
            this->data.push_back(*(std::static_pointer_cast<T>(rowPtr)));
    }
    void removeLastValue() override { data.pop_back(); }

    T getRowValue(const size_t rowIndex = 0) const { return (rowIndex < data.size()) ? data[rowIndex] : T(); }

    SqlExpr getSqlRowValue(size_t rowIndex = 0) const override
    {
        std::ostringstream sqlBuf;

        bool valueExists = false;

        if ((valueExists = rowIndex < getRowsCount()))
        {
            if (this->columnType == TCT::TEXT) sqlBuf << "'";
            sqlBuf << stringify(this->data[rowIndex]);
            if (this->columnType == TCT::TEXT) sqlBuf << "'";
        }

        if (!valueExists)
            if ((valueExists = this->is_NOT_NULL()))
            {
                switch (this->columnType)
                {
                    case TCT::INTEGER:  sqlBuf << "0";                  break;
                    case TCT::REAL:     sqlBuf << "0";                  break;
                    case TCT::DATETIME: sqlBuf << "CURRENT_TIMESTAMP";  break;
                    case TCT::TEXT:     sqlBuf << "''";                 break;
                    default: throw "UNKNOWN DATA TYPE";
                }
            }

        if (!valueExists) sqlBuf << "NULL";

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

        if (this->is_PRIMARY_KEY())      sqlBuf << " " << "PRIMARY KEY";
        if (this->is_AUTOINCREMENT())    sqlBuf << " " << "AUTOINCREMENT";
        if (this->is_NOT_NULL())         sqlBuf << " " << "NOT NULL";
        if (this->is_DEFAULT())          sqlBuf << " " << "DEFAULT";

        if (this->is_NOT_NULL() || this->is_DEFAULT())
            sqlBuf << " " << this->getSqlRowValue();

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
    { return static_cast<TableColumnComparsionExpr>(*this) > static_cast<TableColumnComparsionExpr>(TableColumn<T>(second));  }

    inline TableColumnComparsionExpr operator< (const T& second) const
    { return static_cast<TableColumnComparsionExpr>(*this) < static_cast<TableColumnComparsionExpr>(TableColumn<T>(second));  }

    inline TableColumnComparsionExpr operator== (const T& second) const
    { return static_cast<TableColumnComparsionExpr>(*this) == static_cast<TableColumnComparsionExpr>(TableColumn<T>(second)); }

    inline TableColumnComparsionExpr operator!= (const T& second) const
    { return static_cast<TableColumnComparsionExpr>(*this) != static_cast<TableColumnComparsionExpr>(TableColumn<T>(second)); }
};

}
}
