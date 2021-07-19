/** \file */

#include "picture.h"

/*!
 * \brief on_new_picture_file
 * \param ctrl
 */
void on_new_picture_file(PictureController_File& ctrl)
{
    ctrl.new_file();
}

/*!
 * \brief on_save_picture_file
 * \param ctrl
 */
void on_save_picture_file(PictureController_File& ctrl)
{
    ctrl.save_to_file();
}

/*!
 * \brief on_load_picture_file
 * \param ctrl
 */
void on_load_picture_file(PictureController_File& ctrl)
{
    ctrl.load_from_file();
}

/*!
 * \brief on_add_square
 * \param ctrl
 */
void on_add_square(PictureController_Console& ctrl)
{
    ctrl.add_new_square();
}

/*!
 * \brief on_add_rectangle
 * \param ctrl
 */
void on_add_rectangle(PictureController_Console& ctrl)
{
    ctrl.add_new_rectangle();
}

/*!
 * \brief on_add_round
 * \param ctrl
 */
void on_add_round(PictureController_Console& ctrl)
{
    ctrl.add_new_round();
}

/*!
 * \brief on_add_triangle
 * \param ctrl
 */
void on_add_triangle(PictureController_Console& ctrl)
{
    ctrl.add_new_triangle();
}

/*!
 * \brief on_del_first_shape
 * \param ctrl
 */
void on_del_first_shape(PictureController_Console& ctrl)
{
    ctrl.remove_firt_shape();
}

/*!
 * \brief on_del_last_shape
 * \param ctrl
 */
void on_del_last_shape(PictureController_Console& ctrl)
{
    ctrl.remove_last_shape();
}

/**
 * @brief main точка входа в программу GVEditor
 * @return ноль в случае успешного завершения, ненулевой результат - код ошибки
 */
int main()
{
    PictureModel pic_model;

    PictureView_Console pic_view_c{pic_model};

    PictureController_File pic_ctrl_f{pic_model};
    PictureController_Console pic_ctrl_c{pic_model};

    on_new_picture_file(pic_ctrl_f);

    on_add_square(pic_ctrl_c);
    on_add_rectangle(pic_ctrl_c);
    on_add_round(pic_ctrl_c);
    on_add_triangle(pic_ctrl_c);

    on_del_first_shape(pic_ctrl_c);
    on_del_last_shape(pic_ctrl_c);

    on_save_picture_file(pic_ctrl_f);
    on_load_picture_file(pic_ctrl_f);

    return 0;
}
