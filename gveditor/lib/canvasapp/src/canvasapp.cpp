/** \file */

#include "canvasapp.h"

void CanvasApp::image_new()  { if (m_state != nullptr) m_state->image_new();  }
void CanvasApp::image_open() { if (m_state != nullptr) m_state->image_open(); }
void CanvasApp::image_save() { if (m_state != nullptr) m_state->image_save(); }


void CanvasApp::shape_add_new(std::unique_ptr<Shape>&& shape)
{
    m_canvas.add_shape(std::move(shape));
    m_canvas.select_shape_at(m_canvas.get_shapes_count()-1);

    switchTo(new GveditorAppState_Changed);
}

void CanvasApp::selected_shape_remove()
{
    if (m_canvas.shape_is_selected())
    {
        m_canvas.selected_shape_remove();
        switchTo(new GveditorAppState_Changed);
    }
}

void CanvasApp::selected_shape_set_position()
{
    if (m_canvas.shape_is_selected())
    {
        m_canvas.selected_shape_set_position();
        switchTo(new GveditorAppState_Changed);
    }
}

void CanvasApp::selected_shape_set_size()
{
    if (m_canvas.shape_is_selected())
    {
        m_canvas.selected_shape_resize();
        switchTo(new GveditorAppState_Changed);
    }
}


void CanvasApp::switchTo(CanvasAppState* state)
{
    if (m_state != nullptr)
        delete m_state;

    m_state = state;
    m_state->set_application(this);
}


void CanvasApp::request_image_name()
{
    if (on_image_name_setter != nullptr)
        m_image_name = on_image_name_setter(*this);
}

void CanvasApp::request_image_clear()
{
    m_image_name = "";
    m_canvas.clear();
}

void CanvasApp::request_image_open()
{
    if (on_image_open_handler != nullptr)
        on_image_open_handler(*this, m_image_name);
}

void CanvasApp::request_image_save()
{
    if (on_image_save_handler != nullptr)
        on_image_save_handler(*this, m_image_name);
}
