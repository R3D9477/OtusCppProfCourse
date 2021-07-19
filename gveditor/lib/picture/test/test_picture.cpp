#define BOOST_TEST_MODULE test_picture

#include "picture.h"

#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_SUITE(test_picture)

BOOST_AUTO_TEST_CASE(test_add_shapes)
{
    std::stringstream s_in;
    std::stringstream s_out;

    PictureModel pic_model;
    PictureView_Console pic_view_c{pic_model, s_out};
    PictureController_Console pic_ctrl_c{pic_model, s_in};

    s_in.str("");
    s_out.str("");
    s_in << 6 << std::endl << 7 << std::endl;       // position x,y
    s_in << 8 << std::endl;                         // size
    pic_ctrl_c.add_new_square();

    BOOST_CHECK (s_out.str() == (R"(Picture:
    Draw Sqaure 8x8 at {6,7}
)") );

    s_in.str("");
    s_out.str("");
    s_in << 30 << std::endl << 40 << std::endl;     // position x,y
    s_in << 50 << std::endl << 100 << std::endl;    // size
    pic_ctrl_c.add_new_rectangle();

    BOOST_CHECK (s_out.str() == (R"(Picture:
    Draw Sqaure 8x8 at {6,7}
    Draw Rectangle 50x100 at {30,40}
)") );

    s_in.str("");
    s_out.str("");
    s_in << 100 << std::endl << 105 << std::endl;   // position x,y
    s_in << 15 << std::endl;                        // size
    pic_ctrl_c.add_new_round();

    BOOST_CHECK (s_out.str() == (R"(Picture:
    Draw Sqaure 8x8 at {6,7}
    Draw Rectangle 50x100 at {30,40}
    Draw Round r:15 at {100,105}
)") );

    s_in.str("");
    s_out.str("");
    s_in << 295 << std::endl << 305 << std::endl;   // position x,y (apex1)
    s_in << 340 << std::endl << 345 << std::endl;   // size: apex2 x,y
    s_in << 400 << std::endl << 300 << std::endl;   // size: apex3 x,y
    pic_ctrl_c.add_new_triangle();

    BOOST_CHECK (s_out.str() == (R"(Picture:
    Draw Sqaure 8x8 at {6,7}
    Draw Rectangle 50x100 at {30,40}
    Draw Round r:15 at {100,105}
    Draw Triangle apex1:{295,305}, apex2:{340,345}, apex3:{400,300}
)") );
}

BOOST_AUTO_TEST_CASE(test_remove_shapes)
{
    std::stringstream s_in;
    std::stringstream s_out;

    PictureModel pic_model;
    PictureView_Console pic_view_c{pic_model, s_out};
    PictureController_Console pic_ctrl_c{pic_model, s_in};

    s_in << 6 << std::endl << 7 << std::endl;       // position x,y
    s_in << 8 << std::endl;                         // size
    pic_ctrl_c.add_new_square();

    s_in << 30 << std::endl << 40 << std::endl;     // position x,y
    s_in << 50 << std::endl << 100 << std::endl;    // size
    pic_ctrl_c.add_new_rectangle();

    s_in << 100 << std::endl << 105 << std::endl;   // position x,y
    s_in << 15 << std::endl;                        // size
    pic_ctrl_c.add_new_round();

    s_in << 295 << std::endl << 305 << std::endl;   // position x,y (apex1)
    s_in << 340 << std::endl << 345 << std::endl;   // size: apex2 x,y
    s_in << 400 << std::endl << 300 << std::endl;   // size: apex3 x,y
    pic_ctrl_c.add_new_triangle();

    pic_ctrl_c.remove_firt_shape();

    s_in.str("");
    s_out.str("");

    pic_ctrl_c.remove_last_shape();

    BOOST_CHECK (s_out.str() == (R"(Picture:
    Draw Rectangle 50x100 at {30,40}
    Draw Round r:15 at {100,105}
)") );
}

}
