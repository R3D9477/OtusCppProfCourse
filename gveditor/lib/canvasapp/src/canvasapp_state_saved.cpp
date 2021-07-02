/** \file */

#include "canvasapp.h"

void GveditorAppState_Saved::image_new()
{
    m_app->request_image_clear();
}

void GveditorAppState_Saved::image_open()
{
    m_app->request_image_name();
    m_app->request_image_open();
}
