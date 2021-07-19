/** \file */

#pragma once

#include "shape.h"

class Square : public Shape
{
    int m_width = 0;
public:
    Square(const std::pair<int,int> def_position = {0,0}, const int def_size = 0):
        Shape(def_position),
        m_width{def_size}
        { }
    void accept(ShapeVisitor& visitor) override { visitor.visit(*this); }
    void draw(std::ostream& data_out) const override
    {
        data_out << "Draw Sqaure ";
        data_out << m_width << "x" << m_width;
        data_out << " at {" << m_x << "," << m_y << "}" << std::endl;
    }
    bool is_inside(const std::pair<int,int> position) const override
    {
        return (position.first >= m_x && position.first <= m_x+m_width)
        && (position.second >= m_y && position.second <= m_y+m_width);
    }
    void set_size(int width) { m_width = width; }
    int get_size() const { return m_width; }
    std::string get_shape_name() const override { return "Square"; }
};
