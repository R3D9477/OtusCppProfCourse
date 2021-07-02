/** \file */

#pragma once

#include "guimousearea.h"

class GuiComponent: public GuiObserver
{
protected:

    GuiMouseArea& m_mouse_area;
    PositionXY m_xy;
    SizeWH m_wh;

public:

    /*!
     * \brief onClick
     * \param self объект вызвавший событие
     * \param clicked_xy глобальные коондинаты мыши в MouseArea в момент щелчка
     */
    std::function<void(GuiComponent& self, PositionXY clicked_xy)> onClick;

    GuiComponent(
            GuiMouseArea& mouseArea,
            PositionXY def_xy = {0,0}, SizeWH def_wh = {0,0},
            decltype(onClick) on_click_handler = nullptr
        ):
        m_mouse_area{mouseArea}, m_xy{def_xy}, m_wh{def_wh}, onClick{on_click_handler}
    {
        m_mouse_area.subscribe(this);
    }
    ~GuiComponent()
    {
        m_mouse_area.unsubscribe(this);
    }
    bool click(PositionXY xy) override
    {
        return
               (xy.first  >= m_xy.first  && xy.first  <= m_xy.first+m_wh.first)
            && (xy.second >= m_xy.second && xy.second <= m_xy.second+m_wh.second)
            ? call_onclick_handler(xy) : false;
    }
    virtual void set_position(PositionXY xy)
    {
        m_xy = xy;
    }
    virtual void set_size(SizeWH wh)
    {
        m_wh = wh;
    }
protected:
    virtual bool call_onclick_handler(PositionXY xy)
    {
        if (onClick != nullptr)
        {
            onClick(*this, xy);
            return true;
        }

        return false;
    }
};
