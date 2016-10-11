/*
 * color.h
 *
 *  Created on: Jun 4, 2016
 *      Author: aliaksei
 */

#ifndef COLOR_H_
#define COLOR_H_

#include "exports.h"
#include "2d/Types.h"

using namespace mozilla::gfx;

#ifdef __cplusplus
extern "C" {
#endif

LIBRARY_API Color* moz2d_color_create(Float red, Float green, Float blue, Float alpha);
LIBRARY_API void moz2d_color_delete(Color *color);
LIBRARY_API Float moz2d_color_get_red(Color *color);
LIBRARY_API Float moz2d_color_get_green(Color *color);
LIBRARY_API Float moz2d_color_get_blue(Color *color);
LIBRARY_API Float moz2d_color_get_alpha(Color *color);

#ifdef __cplusplus
}
#endif

#endif /* COLOR_H_ */
