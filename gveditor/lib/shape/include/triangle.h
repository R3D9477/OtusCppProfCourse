/** \file */

#pragma once

#include <algorithm>

#include "shape.h"

class Triangle : public Shape
{
    int m_x2 = 0;
    int m_y2 = 0;
    int m_x3 = 0;
    int m_y3 = 0;
public:
    Triangle(const std::pair<int,int> def_position = {0,0}, const std::pair<int,int> def_apex2 = {0,0}, const std::pair<int,int> def_apex3 = {0,0}):
        Shape(def_position),
        m_x2{def_apex2.first},
        m_y2{def_apex2.second},
        m_x3{def_apex3.first},
        m_y3{def_apex3.second}
        { }
    void accept(ShapeVisitor& visitor) override { visitor.visit(*this); }
    void draw(std::ostream& data_out) const override
    {
        data_out << "Draw Triangle ";
        data_out << "apex1:{" << m_x  << "," << m_y  << "}, ";
        data_out << "apex2:{" << m_x2 << "," << m_y2 << "}, ";
        data_out << "apex3:{" << m_x3 << "," << m_y3 << "}" << std::endl;
    }
    bool is_inside(const std::pair<int,int> position) const override
    {
        return  (position.first  >= std::min({m_x, m_x2, m_x3}) && position.first  <= std::max({m_x, m_x2, m_x3}))
            &&  (position.second >= std::min({m_y, m_y2, m_y3}) && position.second <= std::max({m_y, m_y2, m_y3}));
    }
    void set_apexes(const std::pair<int,int> a2, const std::pair<int,int> a3)
    {
        m_x2 = a2.first;
        m_y2 = a2.second;
        m_x3 = a3.first;
        m_y3 = a3.second;
    }
    std::pair<std::pair<int,int>,std::pair<int,int>> get_apexes() const
    {
        return { {m_x2,m_y2}, {m_x3,m_y3} };
    }
    std::string get_shape_name() const override { return "Triangle"; }
};
