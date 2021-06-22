#pragma once

#include "shapes.h"

class ShapeVisitor
{
public:
    virtual void visit(const Rectangle* rect) = 0;
    virtual void visit(const Square* square)  = 0;
};
