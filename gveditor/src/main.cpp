/** \file */

#include "rectangle.h"
#include "square.h"
#include "round.h"
#include "triangle.h"

#include "guimousearea.h"
#include "guibutton.h"
#include "guicanvas.h"

#include "canvasapp.h"


/*!
 * \brief on_image_name реализация сеттера наименования рисунка
 * \param app ссылка область рисования
 * \return наименование риснка
 */
std::string on_image_name(CanvasApp& app)
{
    std::ignore = app;
    std::string image_name = "";

    std::cout << "Enter image name:";
    std::cin >> image_name;

    return image_name;
}

/*!
 * \brief on_image_open обработчик события области рисования при открытии рисунка
 * \param app ссылка область рисования
 * \param image_name наименование рисунка
 */
void on_image_open(CanvasApp& app, std::string& image_name)
{
    std::ignore = app;

    std::cout << "Open " << image_name << ", deserialize and load shapes..." << std::endl;
    /*!
     *  \todo Здесь загрузка из файла, десериализация и создание объектов фигур
     *  ...
     *  app.shape_add_new(...)
     *  ...
     */
}

/*!
 * \brief on_image_save обработчик события области рисования при сохраненни рисунка
 * \param app ссылка область рисования
 * \param image_name наименование рисунка
 */
void on_image_save(CanvasApp& app, std::string& image_name)
{
    std::ignore = app;

    std::cout << "Serialize shapes and  write to " << image_name << "..." << std::endl;
    /*!
     *  \todo Здесь запись в файл из потока данных, подключенного в canvas
     *  ...
     *  app.get_image_name() >> ...
     *  ...
     */
}


/*!
 * \brief on_new_image_click обработчик события щелчка по кнопке "Новый рисунок"
 * \param app ссылка область рисования
 */
void on_btn_image_new_click(CanvasApp& app)
{
    std::cout << "'New image' has been clicked!" << std::endl;
    app.image_new();
}

/*!
 * \brief on_open_image_click обработчик события щелчка по кнопке "Открыть рисунок"
 * \param app ссылка область рисования
 */
void on_btn_image_open_click(CanvasApp& app)
{
    std::cout << "'Open image' has been clicked!" << std::endl;
    app.image_open();
}

/*!
 * \brief on_save_image_click обработчик события щелчка по кнопке "Сохранить рисунок"
 * \param app ссылка область рисования
 */
void on_btn_image_save_click(CanvasApp& app)
{
    std::cout << "'Save image' has been clicked!" << std::endl;
    app.image_save();
}

/*!
 * \brief on_exit_click обработчик события щелчка по кнопке "Выйти из приложения"
 * \param app ссылка область рисования
 */
void on_btn_exit_click(CanvasApp& app)
{
    std::cout << "'Exit' has been clicked!" << std::endl;
    app.image_save();
    //exit(0);
}


/*!
 * \brief set_selected_shape_geometry функция установки позиции и размеров выделенной фигуры
 * \param app ссылка область рисования
 * \param size_description описание полей размера фигуры
 */
void set_selected_shape_geometry(CanvasApp& app, std::string size_description)
{
    std::cout << "Set position (x,y):" << std::endl;
    app.selected_shape_set_position();

    std::cout << "Set size (" << size_description << "):" << std::endl;
    app.selected_shape_set_size();
}

/*!
 * \brief on_btn_shape_add_rectangle_click обработчик события щелчка по кнопке "Добавить новый прямоугольник"
 * \param app ссылка область рисования
 */
void on_btn_shape_add_rectangle_click(CanvasApp& app)
{
    std::cout << "'Add new rectangle' has been clicked!" << std::endl;
    app.shape_add_new(std::make_unique<Rectangle>());
    set_selected_shape_geometry(app, "width,height");
}

/*!
 * \brief on_btn_shape_add_square_click обработчик события щелчка по кнопке "Добавить новый квадрат"
 * \param app ссылка область рисования
 */
void on_btn_shape_add_square_click(CanvasApp& app)
{
    std::cout << "'Add new square' has been clicked!" << std::endl;
    app.shape_add_new(std::make_unique<Square>());
    set_selected_shape_geometry(app, "width");
}

/*!
 * \brief on_btn_shape_add_round_click обработчик события щелчка по кнопке "Добавить новый круг"
 * \param app ссылка область рисования
 */
void on_btn_shape_add_round_click(CanvasApp& app)
{
    std::cout << "'Add new round' has been clicked!" << std::endl;
    app.shape_add_new(std::make_unique<Round>());
    set_selected_shape_geometry(app, "radius");
}

/*!
 * \brief on_btn_shape_add_triangle_click обработчик события щелчка по кнопке "Добавить новый треугольник"
 * \param app ссылка область рисования
 */
void on_btn_shape_add_triangle_click(CanvasApp& app)
{
    std::cout << "'Add new triangle' has been clicked!" << std::endl;
    app.shape_add_new(std::make_unique<Triangle>());
    set_selected_shape_geometry(app, "apex2_x,apex2_y,apex3_x,apex3_y");
}


/*!
 * \brief on_shape_set_pos обработчик события щелчка по кнопке "Изменить позицию фигуры"
 * \param app ссылка область рисования
 */
void on_btn_shape_set_position_click(CanvasApp& app)
{
    std::cout << "'Set shape position' has been clicked!" << std::endl;
    app.selected_shape_set_position();
}

/*!
 * \brief on_shape_set_size обработчик события щелчка по кнопке "Изменить размер фигуры"
 * \param app ссылка область рисования
 */
void on_btn_shape_set_size_click(CanvasApp& app)
{
    std::cout << "'Set shape size' has been clicked!" << std::endl;
    app.selected_shape_set_size();
}

/*!
 * \brief on_shape_set_size обработчик события щелчка по кнопке "Удалить фигуру"
 * \param app ссылка область рисования
 */
void on_btn_shape_remove_click(CanvasApp& app)
{
    std::cout << "'Remove shape' has been clicked!" << std::endl;
    app.selected_shape_remove();
}


/**
 * @brief main точка входа в программу GVEditor
 * @return ноль в случае успешного завершения, ненулевой результат - код ошибки
 */
int main()
{
    CanvasApp app{std::cin, std::cout, {500,500}, on_image_name, on_image_open, on_image_save};


    GuiButton btn_newimg(app.get_mouse_area(), {10,10},  {100,30}, [&app](GuiComponent& self, PositionXY clicked_xy){
        std::ignore = self;
        std::ignore = clicked_xy;
        on_btn_image_new_click(app);
    });

    GuiButton btn_openimg(app.get_mouse_area(), {140,10}, {100,30}, [&app](GuiComponent& self, PositionXY clicked_xy){
        std::ignore = self;
        std::ignore = clicked_xy;
        on_btn_image_open_click(app);
    });

    GuiButton btn_saveimg(app.get_mouse_area(), {270,10}, {100,30}, [&app](GuiComponent& self, PositionXY clicked_xy){
        std::ignore = self;
        std::ignore = clicked_xy;
        on_btn_image_save_click(app);
    });

    GuiButton btn_exit(app.get_mouse_area(), {400,10}, {100,30}, [&app](GuiComponent& self, PositionXY clicked_xy){
        std::ignore = self;
        std::ignore = clicked_xy;
        on_btn_exit_click(app);
    });


    GuiButton btn_add_new_rectangle(app.get_mouse_area(), {10,50}, {100,30}, [&app](GuiComponent& self, PositionXY clicked_xy){
        std::ignore = self;
        std::ignore = clicked_xy;
        on_btn_shape_add_rectangle_click(app);
    });

    GuiButton btn_add_new_square(app.get_mouse_area(), {140,50}, {100,30}, [&app](GuiComponent& self, PositionXY clicked_xy){
        std::ignore = self;
        std::ignore = clicked_xy;
        on_btn_shape_add_square_click(app);
    });

    GuiButton btn_add_new_round(app.get_mouse_area(), {270,50}, {100,30}, [&app](GuiComponent& self, PositionXY clicked_xy){
        std::ignore = self;
        std::ignore = clicked_xy;
        on_btn_shape_add_round_click(app);
    });

    GuiButton btn_add_new_triangle(app.get_mouse_area(), {400,50}, {100,30}, [&app](GuiComponent& self, PositionXY clicked_xy){
        std::ignore = self;
        std::ignore = clicked_xy;
        on_btn_shape_add_triangle_click(app);
    });


    GuiButton btn_shape_set_pos(app.get_mouse_area(), {10,80}, {100,30}, [&app](GuiComponent& self, PositionXY clicked_xy){
        std::ignore = self;
        std::ignore = clicked_xy;
        on_btn_shape_set_position_click(app);
    });

    GuiButton btn_shape_set_size(app.get_mouse_area(), {140,80}, {100,30}, [&app](GuiComponent& self, PositionXY clicked_xy){
        std::ignore = self;
        std::ignore = clicked_xy;
        on_btn_shape_set_size_click(app);
    });

    GuiButton btn_shape_remove(app.get_mouse_area(), {270,80}, {100,30}, [&app](GuiComponent& self, PositionXY clicked_xy){
        std::ignore = self;
        std::ignore = clicked_xy;
        on_btn_shape_remove_click(app);
    });


    app.get_mouse_area().click({20,  20});  // new
    app.get_mouse_area().click({150, 20});  // open

    app.get_mouse_area().click({20,  60});  // add new rectangle
    app.get_mouse_area().click({150, 60});  // add new square
    app.get_mouse_area().click({280, 60});  // add new round
    app.get_mouse_area().click({410, 60});  // add new triangle

    app.get_mouse_area().click({20,  110}); // not found
    app.get_mouse_area().click({110, 210}); // square
    app.get_mouse_area().click({110, 410}); // round
    app.get_mouse_area().click({310, 230}); // triangle
    app.get_mouse_area().click({310, 410}); // rectangle

    app.get_canvas().draw();

    std::cout << std::endl << "Remove last shape..." << std::endl << std::endl;
    app.get_mouse_area().click({280, 90});  // remove shape
    app.get_canvas().draw();

    std::cout << std::endl;
    app.get_mouse_area().click({280, 20});  // save
    app.get_mouse_area().click({410, 20});  // exit

    return 0;
}
