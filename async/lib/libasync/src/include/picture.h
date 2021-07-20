/** \file */

#pragma once

#include <vector>
#include <memory>
#include <sstream>
#include <iostream>

#include "round.h"
#include "square.h"
#include "triangle.h"
#include "rectangle.h"
#include "shaperesizer.h"
#include "shapeserializer.h"

class Observer
{
public:
    virtual ~Observer() = default;
    virtual void draw() = 0;
};

class Observable
{
    std::vector<Observer*> m_observers;
public:

    void add_observer(Observer* observer)
    {
        m_observers.emplace_back(std::move(observer));
    }
    void notify_draw()
    {
        for (auto& obs: m_observers)
            obs->draw();
    }
};

class PictureModel: public Observable
{
    std::vector<std::unique_ptr<Shape>> m_shapes;
public:

    PictureModel() { clear(); }

    size_t get_shapes_count() const { return m_shapes.size(); }

    void add_shape(std::unique_ptr<Shape>&& shape)
    {
        m_shapes.emplace_back(std::move(shape));
        notify_draw();
    }

    void remove_shape(decltype(m_shapes.begin())&& shape_iter)
    {
        if (shape_iter != m_shapes.end())
        {
            m_shapes.erase(std::move(shape_iter));
            notify_draw();
        }
    }

    void remove_shape(decltype (m_shapes.rbegin())&& shape_iter)
    {
        if (shape_iter != m_shapes.rend())
            remove_shape((++shape_iter).base());
    }

    void clear()
    {
        m_shapes.clear();
        m_shapes.reserve(10);
        notify_draw();
    }

    auto begin()   { return m_shapes.begin();  }
    auto end()     { return m_shapes.end();    }
    auto rbegin()  { return m_shapes.rbegin(); }
    auto rend()    { return m_shapes.rend();   }
    auto cbegin()  const { return m_shapes.cbegin();  }
    auto cend()    const { return m_shapes.cend();    }
    auto crbegin() const { return m_shapes.crbegin(); }
    auto crend()   const { return m_shapes.crend();   }
};

class PictureView_Console: public Observer
{
    PictureModel& m_model;
    std::ostream& m_out;
public:

    PictureView_Console(PictureModel& model, std::ostream& out = std::cout): m_model{model}, m_out{out}
    {
        m_model.add_observer(this);
    }

    ~PictureView_Console() = default;

    virtual void draw()
    {
        system("clear");
        m_out << "Picture:" << std::endl;
        for (const auto& shape: m_model)
        {
            m_out << "    ";
            shape.get()->draw(m_out);
        }
    }
};

class PictureController_Console
{
    PictureModel& m_model;
    std::unique_ptr<ShapeVisitor> m_shape_resizer;
    std::istream& m_in;
public:
    PictureController_Console(PictureModel& model, std::istream& in = std::cin): m_model{model}, m_in{in}
    {
        m_shape_resizer = std::make_unique<ShapeResizer>(m_in);
    }
    void add_new_square()       { add_new_shape(std::make_unique<Square>());    }
    void add_new_rectangle()    { add_new_shape(std::make_unique<Rectangle>()); }
    void add_new_round()        { add_new_shape(std::make_unique<Round>());     }
    void add_new_triangle()     { add_new_shape(std::make_unique<Triangle>());  }
    void remove_firt_shape()    { m_model.remove_shape(m_model.begin());      }
    void remove_last_shape()    { m_model.remove_shape(m_model.rbegin());     }
private:
    void add_new_shape(std::unique_ptr<Shape>&& new_shape)
    {
        std::cout << std::endl << std::endl;
        std::cout << "Set position of new " << new_shape->get_shape_name() << ':' << std::endl;
        uint pos_x, pos_y;
        m_in >> pos_x >> pos_y;
        new_shape.get()->set_position({pos_x, pos_y});

        std::cout << "Set size of new " << new_shape->get_shape_name() << ':' << std::endl;
        new_shape.get()->accept(*m_shape_resizer.get());

        m_model.add_shape(std::move(new_shape));
    }
};

class PictureController_File
{
    PictureModel& m_model;
    std::string m_file_name;
    std::unique_ptr<ShapeVisitor> m_shape_serializer;
    std::stringstream m_shapre_sbuff;
public:
    PictureController_File(PictureModel& model): m_model{model}
    {
        m_shape_serializer = std::make_unique<ShapeSerializer>(m_shapre_sbuff);
    }
    void save_to_file()
    {
        if (m_file_name.empty())
        {
            std::cout << "Choose file to save: ";
            std::cin >> m_file_name;

            m_shapre_sbuff.str("");
            for (const auto& shape: m_model)
                shape.get()->accept(*m_shape_serializer.get());

            /*!
             * ...
             * open file stream
             * ...
             * write m_shapre_sbuff to file
             * ...
             * close file stream
             * ...
             */
        }
    }
    void load_from_file()
    {
        m_model.clear();

        std::cout << "Choose file to open: ";
        std::cin >> m_file_name;

        /*!
         * ...
         * deserialize objects...
         * ...
         * add shapes through "m_model.add_shape"
         *      ...
         *      auto new_shape = make_unique<...>(....);
         *      m_model.add_shape(std::move(new_shape));
         *      ...
         * ...
         */
    }
    void new_file()
    {
        m_file_name.clear();
        m_model.clear();
    }
};
