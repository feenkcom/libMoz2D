/*
 * pattern.h
 *
 *  Created on: Jun 4, 2016
 *      Author: aliaksei
 */

#ifndef PATTERN_H_
#define PATTERN_H_

#include "2d/2D.h"

using namespace mozilla::gfx;

#ifdef __cplusplus
extern "C" {
#endif

Pattern* moz2d_pattern_color_create(Color *color);
Pattern* moz2d_pattern_linear_gradient_create(Point *aBegin, Point *anEnd, GradientStops *aStops, Matrix *aMatrix);
Pattern* moz2d_pattern_radial_gradient_create(Point *innerCenter, Point *outerCenter, Float innerRadius, Float outerRadius, GradientStops *aStops, Matrix *aMatrix);
void moz2d_pattern_delete(Pattern* pattern);

#ifdef __cplusplus
}
#endif

#endif /* PATTERN_H_ */
