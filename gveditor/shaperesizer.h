#pragma once

#include "shapevisitor.h"

class ShapeResizer: ShapeVisitor
{
public:
    ShapeResizer() = default;

    void visit(const Rectangle *rect) override
    {
        //...
        //...
    }
    void visit(const Square* square) override
    {
        //...
        //...
    }
};
