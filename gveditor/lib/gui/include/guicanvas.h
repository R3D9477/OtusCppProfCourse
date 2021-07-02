/** \file */

#pragma once

#include <vector>
#include <memory>
#include <iostream>

#include "round.h"
#include "square.h"
#include "triangle.h"
#include "rectangle.h"
#include "shaperesizer.h"
#include "shapeserializer.h"

#include "guicomponent.h"

class GuiCanvas: public GuiComponent
{
    std::vector<std::unique_ptr<Shape>> m_shapes;
    decltype(m_shapes.end()) m_selected_shape_iter;

    std::unique_ptr<ShapeVisitor> m_shape_resizer;
    std::unique_ptr<ShapeSerializer> m_shape_serializer;

    std::istream& m_data_in;
    std::ostream& m_data_out;

public:

    GuiCanvas(
            GuiMouseArea& mouseArea,
            std::istream& data_in, std::ostream& data_out,
            PositionXY def_xy = {0,0}, SizeWH def_wh = {0,0},
            decltype(onClick) on_click_handler = nullptr
        ):
        GuiComponent(mouseArea, def_xy, def_wh, on_click_handler), m_data_in{data_in}, m_data_out{data_out}
    {
        clear();

        m_shape_resizer = std::make_unique<ShapeResizer>(m_data_in);
        m_shape_serializer = std::make_unique<ShapeSerializer>(m_data_out);
    }

    std::istream& get_data_in() { return m_data_in; }
    std::ostream& get_data_out() { return m_data_out; }

    bool call_onclick_handler(PositionXY xy) override
    {
        auto fnd_internal_pos = std::make_pair(xy.first-m_xy.first, xy.second-m_xy.second);

        for (m_selected_shape_iter = m_shapes.begin(); m_selected_shape_iter != m_shapes.end(); ++m_selected_shape_iter)
            if (m_selected_shape_iter->get()->is_inside(fnd_internal_pos))
                break;

        return GuiComponent::call_onclick_handler(xy);
    };

    void add_shape(std::unique_ptr<Shape> shape)
    {
        m_shapes.emplace_back(std::move(shape));
        m_selected_shape_iter = m_shapes.end();
    }

    void selected_shape_resize()
    {
        if (shape_is_selected())
            m_selected_shape_iter->get()->accept(*m_shape_resizer.get());
    }

    void selected_shape_set_position()
    {
        if (shape_is_selected())
        {
            uint pos_x, pos_y;
            m_data_in >> pos_x >> pos_y;
            m_selected_shape_iter->get()->set_position({pos_x, pos_y});
        }
    }

    void selected_shape_remove()
    {
        if (shape_is_selected())
        {
            m_shapes.erase(m_selected_shape_iter);
            m_selected_shape_iter = m_shapes.end();
        }
    }

    void selected_shape_draw()
    {
        if (shape_is_selected())
            m_selected_shape_iter->get()->draw(m_data_out);
    }

    bool shape_is_selected() const
    {
        return m_selected_shape_iter != m_shapes.end();
    }

    size_t get_shapes_count() const
    {
        return m_shapes.size();
    }

    void select_shape_at(const size_t shape_index)
    {
        if (shape_index < get_shapes_count())
        {
            m_selected_shape_iter = m_shapes.begin() + shape_index;
        }
    }

    void draw() const
    {
        for (auto& shape: m_shapes)
            shape->draw(m_data_out);
    }

    void save()
    {
        for (auto& shape: m_shapes)
            shape->accept(*m_shape_serializer.get());
    }

    void load()
    {
        clear();

        /*!
         *  \todo Здесь загрузка данных из потока m_data_in, парсинг создание объектов фигур
         *  ...
         */

        m_selected_shape_iter = m_shapes.end();
    }

    void clear()
    {
        m_shapes.clear();
        m_shapes.reserve(10);
        m_selected_shape_iter = m_shapes.end();
    }
};

