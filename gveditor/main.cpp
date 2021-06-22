/** \file */

#include <vector>
#include <memory>

#include "rectangle.h"
#include "square.h"
#include "shaperesizer.h"

/**
 * @brief main is the 'entry point' of GVEditor application.
 * @return result of application execution, non-zero value is code of the error.
 */
int main()
{
    std::vector<std::unique_ptr<Shape>> shapes;
    shapes.emplace_back(std::make_unique<Rectangle>());
    shapes.emplace_back(std::make_unique<Square>());

    std::unique_ptr<ShapeVisitor> resizer = std::make_unique<ShapeResizer>();

    shapes[0]->accept(resizer.get());
    shapes[1]->accept(resizer.get());

    return 0;
}
