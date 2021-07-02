/** \file */

#pragma once

#include "shape.h"

class Round : public Shape
{
    int m_radius = 0;
public:
    Round(const std::pair<int,int> def_position = {0,0}, const int def_radius = 0):
        Shape(def_position),
        m_radius{def_radius}
        { }
    void accept(ShapeVisitor& visitor) override { visitor.visit(*this); }
    void draw(std::ostream& data_out) const override
    {
        data_out << "Draw Round ";
        data_out << "r:" << m_radius;
        data_out << " at {" << m_x << "," << m_y << "}" << std::endl;
    }
    bool is_inside(const std::pair<int,int> position) const override
    {
        return (position.first >= m_x-m_radius && position.first <= m_x+m_radius)
        && (position.second >= m_y-m_radius && position.second <= m_y+m_radius);
    }
    void set_radius(int radius) { m_radius = radius; }
    int get_radius() const { return m_radius; }
};
