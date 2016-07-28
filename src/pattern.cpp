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

Pattern* moz2d_pattern_color_create_rgba(Float red, Float green, Float blue, Float alpha) {
	return new ColorPattern(Color(red, green, blue, alpha));
}

Pattern* moz2d_pattern_linear_gradient_create(Point *aBegin, Point *anEnd, GradientStops *aStops, Matrix *aMatrix) {
	return new LinearGradientPattern(*aBegin, *anEnd, aStops, *aMatrix);
}

Pattern* moz2d_pattern_linear_gradient_create_flat(DrawTarget* drawTarget, Float beginX, Float beginY, Float endX, Float endY, Float * rawStops, uint32_t stopsNum, Float * rawMatrix, ExtendMode aExtendMode) {
	already_AddRefed<GradientStops> gradientStops = create_gradient_stops (drawTarget, rawStops, stopsNum, aExtendMode);

	return new LinearGradientPattern(
			Point(beginX, beginY),
			Point(endX, endY),
			gradientStops.take(),
			Matrix(rawMatrix[0],rawMatrix[1],rawMatrix[2],rawMatrix[3],rawMatrix[4],rawMatrix[5]));
}

Pattern* moz2d_pattern_radial_gradient_create(Point *innerCenter, Point *outerCenter, Float innerRadius, Float outerRadius, GradientStops *aStops, Matrix *aMatrix) {
	return new RadialGradientPattern(*innerCenter, *outerCenter, innerRadius, outerRadius, aStops, *aMatrix);
}

Pattern* moz2d_pattern_radial_gradient_create_flat(
		DrawTarget* drawTarget,
		Float innerCenterX,
		Float innerCenterY,
		Float innerRadius,
		Float outerCenterX,
		Float outerCenterY,
		Float outerRadius,
		Float * rawStops,
		uint32_t stopsNum,
		Float * rawMatrix,
		ExtendMode aExtendMode) {
	already_AddRefed<GradientStops> gradientStops = create_gradient_stops (drawTarget, rawStops, stopsNum, aExtendMode);

	return new RadialGradientPattern(
			Point(innerCenterX, innerCenterY),
			Point(outerCenterX, outerCenterY),
			innerRadius,
			outerRadius,
			gradientStops.take(),
			Matrix(rawMatrix[0],rawMatrix[1],rawMatrix[2],rawMatrix[3],rawMatrix[4],rawMatrix[5]));
}

void moz2d_pattern_delete(Pattern* pattern) {
	delete pattern;
}

already_AddRefed<GradientStops> create_gradient_stops (DrawTarget* drawTarget, Float * rawStops, uint32_t stopsNum, ExtendMode aExtendMode) {
	const int count = static_cast<int>(stopsNum);
	GradientStop *stops = new GradientStop[count];
	int index;
	for (int i = 0; i < count; i++) {
		index = i * 5;
		stops[i].offset = rawStops[index];
		stops[i].color = Color(rawStops[index + 1], rawStops[index + 2], rawStops[index + 3], rawStops[index + 4]);
	}
	already_AddRefed<GradientStops> gradientStops = drawTarget->CreateGradientStops(stops, stopsNum, aExtendMode);
	delete [] stops;
	return gradientStops;
}
