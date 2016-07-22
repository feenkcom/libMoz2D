/*
 * color.h
 *
 *  Created on: Jun 4, 2016
 *      Author: aliaksei
 */

#ifndef COLOR_H_
#define COLOR_H_

#include "2d/Types.h"

using namespace mozilla::gfx;

#ifdef __cplusplus
extern "C" {
#endif

Color* moz2d_color_create(Float red, Float green, Float blue, Float alpha);
void moz2d_color_delete(Color *color);
Float moz2d_color_get_red(Color *color);
Float moz2d_color_get_green(Color *color);
Float moz2d_color_get_blue(Color *color);
Float moz2d_color_get_alpha(Color *color);

#ifdef __cplusplus
}
#endif

#endif /* COLOR_H_ */
