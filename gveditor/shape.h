#pragma once

#include "shapevisitor.h"

class Shape
{
public:
    virtual void draw() const = 0;
    virtual void accept(ShapeVisitor* visitor) const = 0;
};
