/** \file */

#pragma once

#include <tuple>
#include <iostream>

#include "shape.h"
#include "square.h"
#include "rectangle.h"
#include "round.h"
#include "triangle.h"

class ShapeSerializer: public ShapeVisitor
{
    std::ostream& m_data_out;

public:

    ShapeSerializer(std::ostream& data_out) : m_data_out{data_out} { }

    void visit(Square& square) override
    {
        m_data_out << "square" << std::endl;
        m_data_out << square.get_position().first << std::endl;
        m_data_out << square.get_position().second << std::endl;
        m_data_out << square.get_size() << std::endl;
    }

    void visit(Rectangle& rect) override
    {
        m_data_out << "rectangle" << std::endl;
        m_data_out << rect.get_position().first << std::endl;
        m_data_out << rect.get_position().second << std::endl;
        m_data_out << rect.get_size().first << std::endl;
        m_data_out << rect.get_size().second << std::endl;
    }

    void visit(Round& round) override
    {
        m_data_out << "round" << std::endl;
        m_data_out << round.get_position().first << std::endl;
        m_data_out << round.get_position().second << std::endl;
        m_data_out << round.get_radius() << std::endl;
    }

    void visit(Triangle& triangle) override
    {
        m_data_out << "triangle" << std::endl;
        m_data_out << triangle.get_position().first << std::endl;
        m_data_out << triangle.get_position().second << std::endl;
        m_data_out << triangle.get_apexes().first.first << std::endl;
        m_data_out << triangle.get_apexes().first.second << std::endl;
        m_data_out << triangle.get_apexes().second.first << std::endl;
        m_data_out << triangle.get_apexes().second.second << std::endl;
    }
};
