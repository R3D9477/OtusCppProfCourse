#include "shape.h"

Shape::Shape (const double x, const double y)
{
    setBasicProperty({ IShapePropertyType::position_x, x });
    setBasicProperty({ IShapePropertyType::position_y, y });
}

void Shape::setBasicProperty(const IShapeProperty isp)
{
    switch (isp.type)
    {
        case IShapePropertyType::position_x:
            this->position_x = std::any_cast<double>(isp.value);
            break;
        case IShapePropertyType::position_y:
            this->position_y = std::any_cast<double>(isp.value);
            break;
    }
}

void Shape::setProperty(const IShapeProperty isp)
{
    setBasicProperty(isp);
}
