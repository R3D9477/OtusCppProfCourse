#pragma once

#include "shape.h"
#include "shapevisitor.h"

class Rectangle : public Shape
{
public:
    Rectangle() = default;
    void draw() const override {  };
    void accept(ShapeVisitor* visitor) const override { visitor->visit(this); }
};

