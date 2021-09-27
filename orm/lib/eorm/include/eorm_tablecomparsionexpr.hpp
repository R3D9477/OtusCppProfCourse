/** \file */

#pragma once

#include <sstream>

#include "eorm_sqlname.hpp"
#include "eorm_sqlexpr.hpp"
#include "eorm_sqlstrdata.hpp"

namespace eorm {
namespace core {

struct TableColumnComparsionExpr
{
    std::string expr;

    TableColumnComparsionExpr (const std::string& expr_ = ""):
        expr(expr_ == "" ? "" : '(' + removeSqlDelimiter(expr_) + ')')
    { }

    TableColumnComparsionExpr (const SqlName& sqlName_):
        expr(sqlName_.value)
    { }

    TableColumnComparsionExpr (const SqlExpr& nestedExpr_):
        TableColumnComparsionExpr(nestedExpr_.value)
    { }

    TableColumnComparsionExpr __getExpr(const std::string& op) const
    {
        std::ostringstream sqlBuf;
        sqlBuf << op << " " << this;
        return TableColumnComparsionExpr(sqlBuf.str());
    }

    TableColumnComparsionExpr __getExpr(const TableColumnComparsionExpr& second, const std::string& op) const
    {
        std::ostringstream sqlBuf;
        sqlBuf << this->expr << " " << op << " " << second;
        return TableColumnComparsionExpr(sqlBuf.str());
    }

    TableColumnComparsionExpr __getExpr(const std::string& second, const std::string& op) const
    {
        return __getExpr(TableColumnComparsionExpr(second), op);
    }

    inline TableColumnComparsionExpr operator>  (const TableColumnComparsionExpr& second) const { return __getExpr(second, ">");   }
    inline TableColumnComparsionExpr operator<  (const TableColumnComparsionExpr& second) const { return __getExpr(second, "<");   }
    inline TableColumnComparsionExpr operator== (const TableColumnComparsionExpr& second) const { return __getExpr(second, "=");   }
    inline TableColumnComparsionExpr operator!= (const TableColumnComparsionExpr& second) const { return __getExpr(second, "<>");  }
    inline TableColumnComparsionExpr operator&& (const TableColumnComparsionExpr& second) const { return __getExpr(second, "AND"); }
    inline TableColumnComparsionExpr operator!  () const { return __getExpr("NOT"); }

    inline TableColumnComparsionExpr IN (const TableColumnComparsionExpr& second) const { return __getExpr(second, "IN");  }

    inline bool isEmpty() const { return expr.empty(); }

    friend std::ostream& operator<< (std::ostream& os, const TableColumnComparsionExpr& expr)   { os << expr.expr; return os; }
};

typedef TableColumnComparsionExpr TCCE;

}
}
