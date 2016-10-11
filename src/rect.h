/*
 * rect.h
 *
 *  Created on: Jun 4, 2016
 *      Author: aliaksei
 */

#ifndef RECT_H_
#define RECT_H_

#include "exports.h"
#include "2d/Rect.h"

using namespace mozilla::gfx;

#ifdef __cplusplus
extern "C" {
#endif

LIBRARY_API int32_t moz2d_int_rect_x(IntRect* rect);
LIBRARY_API int32_t moz2d_int_rect_y(IntRect* rect);
LIBRARY_API int32_t moz2d_Int_rect_width(IntRect* rect);
LIBRARY_API int32_t moz2d_Int_rect_height(IntRect* rect);

LIBRARY_API bool moz2d_rect_is_empty(Rect* rect);
LIBRARY_API void moz2d_rect_set_empty(Rect* rect);
LIBRARY_API bool moz2d_rect_is_finite(Rect* rect);
LIBRARY_API bool moz2d_rect_contains_rect(Rect* rect, Rect* anotherRect);
LIBRARY_API bool moz2d_rect_contains(Rect* rect, Float aX, Float aY);
LIBRARY_API bool moz2d_rect_contains_point(Rect* rect, Point *point);
LIBRARY_API void moz2d_rect_set_rect(Rect* rect, Float ax, Float aY, Float aWidth, Float aHeight);

#ifdef __cplusplus
}
#endif

#endif /* RECT_H_ */
