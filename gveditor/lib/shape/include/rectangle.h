/** \file */

#pragma once

#include "shape.h"

class Rectangle : public Shape
{
    int m_width  = 0;
    int m_height = 0;
public:
    Rectangle(const std::pair<int,int> def_position = {0,0}, const std::pair<int,int> def_size = {0,0}):
        Shape(def_position),
        m_width{def_size.first},
        m_height{def_size.second}
        { }
    void accept(ShapeVisitor& visitor) override { visitor.visit(*this); }
    void draw(std::ostream& data_out) const override
    {
        data_out << "Draw Rectangle ";
        data_out << m_width << "x" << m_height;
        data_out << " at {" << m_x << "," << m_y << "}" << std::endl;
    }
    bool is_inside(const std::pair<int,int> position) const override
    {
        return (position.first >= m_x && position.first <= m_x+m_width)
        && (position.second >= m_y && position.second <= m_y+m_height);
    }
    void set_size(std::pair<int,int> size) { m_width = size.first; m_height = size.second; }
    std::pair<int,int> get_size() const { return {m_width,m_height}; }
    std::string get_shape_name() const override { return "Rectangle"; }
};

