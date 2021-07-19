/** \file */

#pragma once

#include <tuple>
#include <iostream>

class Rectangle;
class Square;
class Round;
class Triangle;

class ShapeVisitor
{
public:
    virtual void visit(Rectangle& rect) = 0;
    virtual void visit(Square& square) = 0;
    virtual void visit(Round& round) = 0;
    virtual void visit(Triangle& square) = 0;
};

class Shape
{
protected:
    int m_x = 0;
    int m_y = 0;
    Shape(const std::pair<int,int> def_position = {0,0}): m_x{def_position.first}, m_y{def_position.second} { }
public:
    virtual void set_position(const std::pair<int,int> position) { m_x = position.first; m_y = position.second; }
    virtual std::pair<int,int> get_position() const { return {m_x,m_y}; }
    virtual void accept(ShapeVisitor& visitor) = 0;
    virtual void draw(std::ostream& data_out) const = 0;
    virtual bool is_inside(const std::pair<int,int> position) const = 0;
    virtual std::string get_shape_name() const = 0;
};
