#ifndef FUNCTIONAL_VIEWER_H
#define FUNCTIONAL_VIEWER_H

#include "viewer_info.h"

void move_x(s_info *data_, int shift_);
void move_y(s_info *data_, int shift_);
void move_z(s_info *data_, int shift_);

void scale(s_info *data_, float scale_);

void rotate_x(s_info *data, double turn_);
void rotate_y(s_info *data, double turn_);
void rotate_z(s_info *data, double turn_);

#endif // FUNCTIONAL_VIEWER_H
