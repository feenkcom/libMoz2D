/*
 * pattern.cpp
 *
 *  Created on: Jun 4, 2016
 *      Author: aliaksei
 */

#include "pattern.h"

Pattern* moz2d_pattern_color_create(Color *color) {
	return new ColorPattern(*color);
}

Pattern* moz2d_pattern_linear_gradient_create(Point *aBegin, Point *anEnd, GradientStops *aStops, Matrix *aMatrix) {
	return new LinearGradientPattern(*aBegin, *anEnd, aStops, *aMatrix);
}

Pattern* moz2d_pattern_radial_gradient_create(Point *innerCenter, Point *outerCenter, Float innerRadius, Float outerRadius, GradientStops *aStops, Matrix *aMatrix) {
	return new RadialGradientPattern(*innerCenter, *outerCenter, innerRadius, outerRadius, aStops, *aMatrix);
}

void moz2d_pattern_delete(Pattern* pattern) {
	delete pattern;
}
