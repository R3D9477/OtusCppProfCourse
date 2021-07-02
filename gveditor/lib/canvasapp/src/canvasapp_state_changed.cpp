/** \file */

#include "canvasapp.h"

void GveditorAppState_Changed::image_new()
{
    m_app->image_save();
    m_app->image_new();
}

void GveditorAppState_Changed::image_open()
{
    m_app->image_save();
    m_app->image_open();
}

void GveditorAppState_Changed::image_save()
{
    if (m_app->get_image_name() == "")
        m_app->request_image_name();

    m_app->request_image_save();
    m_app->switchTo(new GveditorAppState_Saved);
}

void GveditorAppState_Changed::app_exit()
{
    m_app->image_save();
}
