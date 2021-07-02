#define BOOST_TEST_MODULE test_gui

#include "guimousearea.h"
#include "guibutton.h"
#include "guicanvas.h"

#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_SUITE(test_gui)

BOOST_AUTO_TEST_CASE(test_button_click)
{
    GuiMouseArea mouse_area;

    bool gui_component_found{false}, button1_clicked{false}, button2_clicked{false};
    PositionXY button1_clicked_xy{0,0}, button2_clicked_xy{0,0};

    auto reset_checking_flags = [&](){
        gui_component_found = false;
        button1_clicked     = false;
        button2_clicked     = false;
        button1_clicked_xy  = {0,0};
        button2_clicked_xy  = {0,0};
    };

    GuiButton btn_newimg(mouse_area, {10,10}, {100,30}, [&button1_clicked,&button1_clicked_xy](auto& self, auto clicked_xy){
        std::ignore = self;
        button1_clicked = true;
        button1_clicked_xy = clicked_xy;
    });

    GuiButton btn_openimg(mouse_area, {140,10}, {100,30}, [&button2_clicked,&button2_clicked_xy](auto& self, auto clicked_xy){
        std::ignore = self;
        button2_clicked = true;
        button2_clicked_xy = clicked_xy;
    });

    reset_checking_flags();
    gui_component_found = mouse_area.click({5,5});

    BOOST_CHECK(!gui_component_found);                          // gui component wasn't found
    BOOST_CHECK(!button1_clicked);                              // button 1 hasn't been clicked
    BOOST_CHECK((button1_clicked_xy == PositionXY{0,0}));
    BOOST_CHECK(!button2_clicked);                              // button 2 hasn't been clicked
    BOOST_CHECK((button2_clicked_xy == PositionXY{0,0}));

    reset_checking_flags();
    gui_component_found = mouse_area.click({20,20});

    BOOST_CHECK(gui_component_found);                           // gui component was found
    BOOST_CHECK(button1_clicked);                               // button 1 has been clicked
    BOOST_CHECK((button1_clicked_xy == PositionXY{20,20}));
    BOOST_CHECK(!button2_clicked);                              // button 2 hasn't been clicked
    BOOST_CHECK((button2_clicked_xy == PositionXY{0,0}));

    reset_checking_flags();
    gui_component_found = mouse_area.click({150,25});

    BOOST_CHECK(gui_component_found);                           // gui component was found
    BOOST_CHECK(!button1_clicked);                              // button 1 hasn't been clicked
    BOOST_CHECK((button1_clicked_xy == PositionXY{0,0}));
    BOOST_CHECK(button2_clicked);                               // button 2 has been clicked
    BOOST_CHECK((button2_clicked_xy == PositionXY{150,25}));
}

BOOST_AUTO_TEST_CASE(test_canvas_add_shape)
{
    GuiMouseArea mouse_area;

    std::stringstream data_out;

    GuiCanvas canvas(mouse_area, std::cin, data_out, {10,10}, {500,500});

    BOOST_CHECK(canvas.get_shapes_count() == 0);

    canvas.add_shape(std::make_unique<Rectangle>(std::pair<int,int>{5,5}, std::pair<int,int>{100,50}));
    canvas.add_shape(std::make_unique<Round>(std::pair<int,int>{300,300}, 40));

    BOOST_CHECK(canvas.get_shapes_count() == 2);

    std::string valid_data = R"(Draw Rectangle 100x50 at {5,5}
Draw Round r:40 at {300,300}
)";

    canvas.draw();

    BOOST_CHECK(data_out.str() == valid_data);
}

BOOST_AUTO_TEST_CASE(test_canvas_click_and_select_shape)
{
    GuiMouseArea mouse_area;

    bool gui_component_found{false}, shape_is_found{false};
    PositionXY gui_component_clicked_xy{0,0};

    auto reset_checking_flags = [&](){
        gui_component_found         = false;
        gui_component_clicked_xy    = {0,0};
    };

    std::stringstream data_out;

    GuiCanvas canvas(mouse_area, std::cin, data_out, {10,10}, {500,500}, [&gui_component_clicked_xy](auto& self, auto clicked_xy){
        std::ignore = self;
        gui_component_clicked_xy = clicked_xy;
    });

    reset_checking_flags();
    gui_component_found = mouse_area.click({5,5});

    BOOST_CHECK(!gui_component_found);                          // gui component wasn't found
    BOOST_CHECK((gui_component_clicked_xy == PositionXY{0,0}));
    BOOST_CHECK(!canvas.shape_is_selected());                   // shape wasn't found

    reset_checking_flags();
    gui_component_found = mouse_area.click({20,20});

    BOOST_CHECK(gui_component_found);                          // gui component was found
    BOOST_CHECK(!shape_is_found);                              // shape wasn't found
    BOOST_CHECK((gui_component_clicked_xy == PositionXY{20,20}));

    BOOST_CHECK(canvas.get_shapes_count() == 0);

    canvas.add_shape(std::make_unique<Rectangle>(std::pair<int,int>{5,5}, std::pair<int,int>{100,50}));
    canvas.add_shape(std::make_unique<Round>(std::pair<int,int>{300,300}, 40));

    reset_checking_flags();
    gui_component_found = mouse_area.click({30,50});

    BOOST_CHECK(gui_component_found);                           // gui component was found (Canvas)
    BOOST_CHECK((gui_component_clicked_xy == PositionXY{30,50}));
    BOOST_CHECK(canvas.shape_is_selected());                    // shape was found (Rectangle)

    reset_checking_flags();
    gui_component_found = mouse_area.click({100,100});

    BOOST_CHECK(gui_component_found);                           // gui component was found (Canvas)
    BOOST_CHECK((gui_component_clicked_xy == PositionXY{100,100}));
    BOOST_CHECK(!canvas.shape_is_selected());                   // shape wasn't found

    reset_checking_flags();
    gui_component_found = mouse_area.click({350,350});

    BOOST_CHECK(gui_component_found);                           // gui component was found (Canvas)
    BOOST_CHECK((gui_component_clicked_xy == PositionXY{350,350}));
    BOOST_CHECK(canvas.shape_is_selected());                    // shape was found (Round)
}

BOOST_AUTO_TEST_CASE(test_canvas_click_and_remove_select_shape)
{
    GuiMouseArea mouse_area;

    std::stringstream data_out;

    GuiCanvas canvas(mouse_area, std::cin, data_out, {10,10}, {500,500});

    canvas.add_shape(std::make_unique<Rectangle>(std::pair<int,int>{5,5}, std::pair<int,int>{100,50}));
    canvas.add_shape(std::make_unique<Round>(std::pair<int,int>{300,300}, 40));

    BOOST_CHECK(canvas.get_shapes_count() == 2);

    std::string valid_data = R"(Draw Rectangle 100x50 at {5,5}
Draw Round r:40 at {300,300}
)";

    canvas.draw();

    BOOST_CHECK(data_out.str() == valid_data);

    mouse_area.click({350,350});
    BOOST_CHECK(canvas.shape_is_selected());                    // shape was found (Round)

    canvas.selected_shape_remove();

    mouse_area.click({350,350});
    BOOST_CHECK(!canvas.shape_is_selected());                   // shape wasn't found (Round has been removed)

    BOOST_CHECK(canvas.get_shapes_count() == 1);

    valid_data = R"(Draw Rectangle 100x50 at {5,5}
)";

    data_out.str("");
    canvas.draw();

    BOOST_CHECK(data_out.str() == valid_data);
}

}
