/** \file */

#pragma once

#include "guicomponent.h"

class GuiButton: public GuiComponent
{
public:
    GuiButton(
            GuiMouseArea& mouseArea,
            PositionXY def_xy = {0,0}, SizeWH def_wh = {0,0},
            decltype(onClick) on_click_handler = nullptr
        ):
        GuiComponent(mouseArea, def_xy, def_wh, on_click_handler) { }
};
