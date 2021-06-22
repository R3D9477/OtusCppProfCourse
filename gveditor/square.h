#pragma once

#include "shape.h"
#include "shapevisitor.h"

class Square : public Shape
{
public:
    Square() = default;
    void draw() const override {  };
    void accept(ShapeVisitor* visitor) const override { visitor->visit(this); }
};
