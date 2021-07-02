/** \file */

#pragma once

#include <memory>
#include <vector>
#include <algorithm>
#include <iostream>
#include <functional>

using PositionXY = std::pair<uint,uint>;
using SizeWH = std::pair<uint,uint>;

class GuiObserver
{
public:
    /*!
     * \brief click
     * \param xy
     * \return
     */
    virtual bool click(PositionXY xy) = 0;
};

class GuiObservable
{
public:
    virtual void subscribe(GuiObserver* obs) = 0;
    virtual void unsubscribe(GuiObserver* osb) = 0;
};

class GuiMouseArea: public GuiObservable
{
    std::vector<GuiObserver*> m_subs;
public:
    GuiMouseArea()
    {
        m_subs.reserve(10);
    }
    void subscribe(GuiObserver* obs) override
    {
        m_subs.emplace_back(obs);
    }
    void unsubscribe(GuiObserver* obs) override
    {
        auto rm_iter = std::find(m_subs.begin(), m_subs.end(), obs);
        if (rm_iter != m_subs.end())
            m_subs.erase(rm_iter);
    }
    bool click(PositionXY xy)
    {
        for (auto& s: m_subs)
            if (s->click(xy))
                return true;

        return false;
    }
};
