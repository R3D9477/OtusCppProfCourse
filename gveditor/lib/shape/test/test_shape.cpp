#define BOOST_TEST_MODULE test_shape

#include <vector>
#include <memory>

#include "shape.h"
#include "rectangle.h"
#include "square.h"
#include "round.h"
#include "triangle.h"
#include "shapeserializer.h"

#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_SUITE(test_shape)

BOOST_AUTO_TEST_CASE(test_shape_serialization)
{
    std::vector<std::unique_ptr<Shape>> shapes;

    shapes.emplace_back(std::make_unique<Rectangle>(std::pair<int,int>{0,0}, std::pair<int,int>{50,10}));
    shapes.emplace_back(std::make_unique<Square>(std::pair<int,int>{50,50}, 25));
    shapes.emplace_back(std::make_unique<Round>(std::pair<int,int>{100,100}, 70));
    shapes.emplace_back(std::make_unique<Triangle>(std::pair<int,int>{200,200}, std::pair<int,int>{210,220}, std::pair<int,int>{300,200}));

    std::stringstream data_out;
    auto shape_serializer = std::make_unique<ShapeSerializer>(data_out);

    data_out.str("");
    shapes[0].get()->accept(*shape_serializer.get());

    BOOST_CHECK( data_out.str() == std::string(R"(rectangle
0
0
50
10
)"));

    data_out.str("");
    shapes[1].get()->accept(*shape_serializer.get());

    BOOST_CHECK( data_out.str() == std::string(R"(square
50
50
25
)"));

    data_out.str("");
    shapes[2].get()->accept(*shape_serializer.get());

    BOOST_CHECK( data_out.str() == std::string(R"(round
100
100
70
)"));


    data_out.str("");
    shapes[3].get()->accept(*shape_serializer.get());

    BOOST_CHECK( data_out.str() == std::string(R"(triangle
200
200
210
220
300
200
)"));
}

}
