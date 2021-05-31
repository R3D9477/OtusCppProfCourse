#pragma once

#include <any>

struct IShapePropertyType
{
    enum Types
    {
        position_x,
        position_y
    };
};

struct IShapeProperty
{
    IShapePropertyType::Types type;
    std::any value;
};

class Shape
{
    double position_x;
    double position_y;
public:
    Shape(const double x = 0, const double y = 0);
    virtual ~Shape() = 0;
    virtual void setProperty (const IShapeProperty isp);
private:
    void setBasicProperty (const IShapeProperty isp);
};

