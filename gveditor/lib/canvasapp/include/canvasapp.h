/** \file */

#pragma once

#include <iostream>

#include "guicanvas.h"
#include "guibutton.h"

class CanvasApp;

class CanvasAppState
{
protected:
    CanvasApp* m_app{nullptr};
public:
    CanvasAppState() = default;
    virtual ~CanvasAppState() { }

    void set_application(CanvasApp* app) { m_app = app; }

    virtual void image_new()  { }
    virtual void image_save() { }
    virtual void image_open() { }
    virtual void app_exit()   { }
};

class GveditorAppState_Saved: public CanvasAppState
{
public:
    void image_new()  override;
    void image_open() override;
};

class GveditorAppState_Changed: public CanvasAppState
{
public:
    void image_new()  override;
    void image_open() override;
    void image_save() override;
    void app_exit()   override;
};

class CanvasApp
{
    std::istream& m_data_in;
    std::ostream& m_data_out;

    GuiMouseArea m_mouseArea;
    GuiCanvas m_canvas;

    CanvasAppState* m_state{nullptr};

    std::string m_image_name;

public:

    std::function<std::string(CanvasApp&)>        on_image_name_setter{nullptr};
    std::function<void(CanvasApp&,std::string&)>  on_image_open_handler{nullptr};
    std::function<void(CanvasApp&,std::string&)>  on_image_save_handler{nullptr};

    CanvasApp(
            std::istream& data_in, std::ostream& data_out,
            const SizeWH window_size,
            decltype(on_image_name_setter)  def_image_name_setter  = nullptr,
            decltype(on_image_open_handler) def_image_open_handler = nullptr,
            decltype(on_image_save_handler) def_image_save_handler = nullptr
        ):
        m_data_in{data_in}, m_data_out{data_out},
        m_mouseArea{}, m_canvas{m_mouseArea, m_data_in, m_data_out, {0,0}, window_size},
        on_image_name_setter{def_image_name_setter},
        on_image_open_handler{def_image_open_handler},
        on_image_save_handler{def_image_save_handler}
    {
        switchTo(new GveditorAppState_Saved);
    }

    void image_new();
    void image_open();
    void image_save();

    void shape_add_new(std::unique_ptr<Shape>&& shape);
    void shape_add_new_rectangle() { shape_add_new(std::make_unique<Rectangle>()); }
    void shape_add_new_square() { shape_add_new(std::make_unique<Square>()); }
    void shape_add_new_round() { shape_add_new(std::make_unique<Round>()); }
    void shape_add_new_triangle() { shape_add_new(std::make_unique<Triangle>()); }

    void selected_shape_remove();
    void selected_shape_set_position();
    void selected_shape_set_size();

    const std::string& get_image_name () const { return m_image_name; }
    const GuiCanvas& get_canvas () const { return m_canvas; }
    GuiMouseArea& get_mouse_area() { return m_mouseArea; }

private:

    friend class GveditorAppState_Saved;
    friend class GveditorAppState_Changed;

    void request_image_name();
    void request_image_clear();
    void request_image_open();
    void request_image_save();

    void switchTo(CanvasAppState* state);
};
