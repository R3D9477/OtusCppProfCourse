/** \file */

#pragma once

#include <tuple>
#include <iostream>

#include "shape.h"
#include "square.h"
#include "rectangle.h"
#include "round.h"
#include "triangle.h"

class ShapeResizer: public ShapeVisitor
{
    std::istream& m_data_in;

public:

    ShapeResizer(std::istream& data_in) : m_data_in{data_in} { }

    void visit(Square& square) override
    {
        int size_buf;
        m_data_in >> size_buf;
        square.set_size(size_buf);
    }

    void visit(Rectangle& rect) override
    {
        std::pair<int,int> size_buf;
        m_data_in >> size_buf.first;
        m_data_in >> size_buf.second;
        rect.set_size(size_buf);
    }

    void visit(Round& round) override
    {
        int size_buf;
        m_data_in >> size_buf;
        round.set_radius(size_buf);
    }

    void visit(Triangle& triangle) override
    {
        std::pair<int,int> a2_buf;
        std::pair<int,int> a3_buf;
        m_data_in >> a2_buf.first;
        m_data_in >> a2_buf.second;
        m_data_in >> a3_buf.first;
        m_data_in >> a3_buf.second;
        triangle.set_apexes(a2_buf, a3_buf);
    }
};
