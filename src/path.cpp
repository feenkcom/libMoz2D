/*
 * path.cpp
 *
 *  Created on: Jul 14, 2016
 *      Author: aliaksei
 */

#include "path.h"

#include <math.h>
#include <algorithm>
#include <limits>

#define PI 				3.141592654f
#define TwoPI			6.283185307f
#define HalfPI			1.570796327f
#define ThreeHalvesPI	4.71238898f
#define SQRT2 			1.414213562f

float angleOfVector(Point vector) {
	vector = vector / vector.Length();
	float arccos = (float) acos(vector.x);

	return vector.y < 0 ? TwoPI - arccos : arccos;
}

Point calcCenter(Point start, Point end, float angle) {
	Point vector = end - start;

	Matrix matrix = Matrix::Rotation((PI - angle) / 2);

	vector = matrix.TransformPoint(vector);

	float length = (float) sqrt(vector.DotProduct(vector));
	float radius = (float) ((length / 2) / sin (angle / 2));

	return vector * (radius / length) + start;
}

template <typename T> int sign(T val) {
    return (T(0) < val) - (val < T(0));
}

void moz2d_path_sink_move_to(PathSink* pathSink, float x, float y, bool absolute) {
	if (!absolute)
		pathSink->MoveTo(Point(x,y) + pathSink->CurrentPoint());
	else
		pathSink->MoveTo(Point(x,y));
}

void moz2d_path_sink_line_to(PathSink* pathSink, float x, float y, bool absolute) {

	if (!absolute)
		pathSink->LineTo(Point(x,y) + pathSink->CurrentPoint());
	else
		pathSink->LineTo(Point(x,y));
}

void moz2d_path_sink_bezier_to(PathSink* pathSink, float x1, float y1, float x2, float y2, float x3, float y3, bool absolute) {
	if (!absolute) {
		Point current = pathSink->CurrentPoint();
		x1 = x1 + current.x;
		y1 = y1 + current.y;
		x2 = x2 + current.x;
		y2 = y2 + current.y;
		x3 = x3 + current.x;
		y3 = y3 + current.y;
	}

	pathSink->BezierTo(Point(x1,y1), Point(x2,y2), Point(x3,y3));
}

void moz2d_path_sink_quadratic_bezier_to(PathSink* pathSink, float x1, float y1, float x2, float y2, bool absolute) {
	if (!absolute) {
		Point current = pathSink->CurrentPoint();
		x1 = x1 + current.x;
		y1 = y1 + current.y;
		x2 = x2 + current.x;
		y2 = y2 + current.y;
	}
	pathSink->QuadraticBezierTo(Point(x1,y1), Point(x2,y2));
}

void moz2d_path_sink_close(PathSink* pathSink) {
	pathSink->Close();
}

void moz2d_path_sink_arc(PathSink* pathSink, float originX, float originY, float aRadius, float aStartAngle, float anEndAngle, bool anAntiClockwise, bool absolute) {
	if (!absolute) {
		Point current = pathSink->CurrentPoint();
		originX = originX + current.x;
		originY = originY + current.y;
	}
	pathSink->Arc(Point(originX, originY), aRadius, aStartAngle,anEndAngle, anAntiClockwise);
}

void moz2d_path_sink_arc_to_angle(PathSink* pathSink, Float endX, Float endY, Float angle, bool aAntiClockwise, bool absolute) {
	if (angle == 0) {
		return moz2d_path_sink_line_to(pathSink, endX, endY, absolute);
	}

	Point start = pathSink->CurrentPoint();
	Point end = Point(endX, endY);

	if (start == end)
		return;

	float cwAngle = aAntiClockwise ? -angle : angle;
	Point center = calcCenter(start, end, cwAngle);
	Point vector = start - center;
	float radius = (float) sqrt(vector.DotProduct(vector));
	float startAngle = angleOfVector(vector);
	float endAngle = angleOfVector(end - center);

	moz2d_path_sink_arc(pathSink, center.x, center.y, radius, startAngle, endAngle, aAntiClockwise, absolute);
}

void moz2d_path_sink_circle_arc_to(PathSink* pathSink, float radius, int8_t vectorX, int8_t vectorY, bool aAntiClockwise) {
	Point start = pathSink->CurrentPoint();
	Point end = start + Point(radius * vectorX, radius * vectorY);

	Point center;
	float startAngle;
	float endAngle;
	if (end.x > start.x) {
		if (end.y > start.y) {
			center = start + (aAntiClockwise ? Point(radius, 0) : Point(0, radius));
			startAngle = aAntiClockwise ? PI : ThreeHalvesPI;
			endAngle = aAntiClockwise ? HalfPI : 0;
		}
		else {
			center = start + (aAntiClockwise ? Point(0, -radius) : Point(radius, 0));
			startAngle = aAntiClockwise ? HalfPI : PI;
			endAngle = aAntiClockwise ? 0 : ThreeHalvesPI;
		}
	}
	else {
		if (end.y > start.y) {
			center = start + (aAntiClockwise ? Point(0, radius) : Point(-radius, 0));
			startAngle = aAntiClockwise ? PI : 0;
			endAngle = aAntiClockwise ? HalfPI : PI; // TODO
		}
		else {
			center = start + (aAntiClockwise ? Point(-radius, 0) : Point(0, -radius));
			startAngle = aAntiClockwise ? 0 : HalfPI;
			endAngle = aAntiClockwise ? HalfPI : PI;
		}
	}

	moz2d_path_sink_arc(pathSink, center.x, center.y, radius, startAngle, endAngle, aAntiClockwise, true);
}

void moz2d_path_sink_ellipse_arc_to(PathSink* pathSink, float endX, float endY, bool aAntiClockwise, bool isAbsolute) {
	Point start = pathSink->CurrentPoint(); // absolute value
	Point end = isAbsolute ? Point(endX, endY) : start + Point(endX, endY);
	Point vector = end - start;

	float halfWidth = std::abs(vector.x);
	float halfHeight = std::abs(vector.y);
	bool absolute = false;

	if (end.x > start.x) {
		if (end.y > start.y) {
			if (aAntiClockwise) {
				moz2d_path_sink_bezier_to(pathSink, 0, halfHeight * 0.55f, halfWidth * 0.45f, halfHeight, halfWidth, halfHeight, absolute);
			}
			else {
				moz2d_path_sink_bezier_to(pathSink, halfWidth * 0.55f, 0, halfWidth, halfHeight * 0.45f, halfWidth, halfHeight, absolute);
			}
		}
		else {
			if (aAntiClockwise) {
				moz2d_path_sink_bezier_to(pathSink, halfWidth * 0.55f, 0, halfWidth, -halfHeight * 0.45f, halfWidth, -halfHeight, absolute);
			}
			else {
				moz2d_path_sink_bezier_to(pathSink, 0, -halfHeight * 0.55f, halfWidth * 0.45f, -halfHeight, halfWidth, -halfHeight, absolute);
			}
		}
	}
	else {
		if (end.y > start.y) {
			if (aAntiClockwise) {
				moz2d_path_sink_bezier_to(pathSink, -halfWidth * 0.55f, 0, -halfWidth , halfHeight * 0.45f, -halfWidth, halfHeight, absolute);
			}
			else {
				moz2d_path_sink_bezier_to(pathSink, 0, halfHeight * 0.55f, -halfWidth * 0.45f, halfHeight, -halfWidth, halfHeight, absolute);
			}
		}
		else {
			if (aAntiClockwise) {
				moz2d_path_sink_bezier_to(pathSink, 0, -halfHeight * 0.55f, -halfWidth * 0.45f, -halfHeight,  -halfWidth, -halfHeight, absolute);
			}
			else {
				moz2d_path_sink_bezier_to(pathSink, -halfWidth * 0.55f, 0, -halfWidth, -halfHeight * 0.45f, -halfWidth, -halfHeight, absolute);
			}
		}
	}
}


MOZ_ALWAYS_INLINE bool floatEquals(float a, float b) {
	return fabs(a - b) < std::numeric_limits<float>::epsilon();
}

MOZ_ALWAYS_INLINE bool absEquals(float a, float b) {
	return floatEquals(fabs(a), fabs(b));
}


void moz2d_path_sink_arc_to (PathSink* pathSink, float endX, float endY, bool aAntiClockwise, bool absolute) {
	// if endX = endY we get a quarter of circle and can simplify creation

	bool optimize = !absolute && absEquals(endX, endY);
	if (!optimize && absolute) {
		Point current = pathSink->CurrentPoint();
		optimize = absEquals(endX - current.x, endY - current.y);
	}

	// if we can optimize, create a circled arc instead
	if (optimize) {
		Point start = pathSink->CurrentPoint();
		Point vector = absolute ? Point(endX, endY) - start : Point(endX, endY);
		moz2d_path_sink_circle_arc_to(pathSink, vector.x, sign(vector.x), sign(vector.y), aAntiClockwise);
	}
	// otherwise we need to create an ellipse arc
	else {
		moz2d_path_sink_ellipse_arc_to(pathSink, endX, endY, aAntiClockwise, absolute);
	}
}

void moz2d_path_sink_current_point(PathSink* pathSink, Point* point) {
	Point currentPoint = pathSink->CurrentPoint();
	point->x = currentPoint.x;
	point->y = currentPoint.y;
}

Path* moz2d_path_builder_finish(PathBuilder* pathBuilder) {
	return pathBuilder->Finish().take();
}

Path* moz2d_shape_circle (DrawTarget* drawTarget, float x, float y, float radius, FillRule aFillRule) {

	PathBuilder* pathBuilder = drawTarget->CreatePathBuilder(aFillRule).take();

	moz2d_path_sink_arc(pathBuilder, x, y, radius, 0, TwoPI, false, true);
	moz2d_path_sink_close(pathBuilder);

	Path* path = moz2d_path_builder_finish(pathBuilder);
	delete pathBuilder;

	return path;
}

Path* moz2d_shape_rounded_rectangle (DrawTarget* drawTarget,
		float x, float y, float width, float height,
		float topLeftRadius, float topRightRadius, float bottomRightRadius, float bottomLeftRadius, FillRule aFillRule) {

	float min = std::min(width , height) / 2;
	float tlr = std::min(topLeftRadius, min);
	float trr = std::min(topRightRadius, min);
	float brr = std::min(bottomRightRadius, min);
	float blr = std::min(bottomLeftRadius, min);
	float right = x + width;
	float bottom = y + height;

	bool absolute = true;

	PathBuilder* pathBuilder = drawTarget->CreatePathBuilder(aFillRule).take();

	moz2d_path_sink_move_to(pathBuilder, x, y + tlr, absolute);
	moz2d_path_sink_arc_to(pathBuilder, x + tlr, y, false, absolute);

	moz2d_path_sink_line_to(pathBuilder, right - trr, y, absolute);
	moz2d_path_sink_arc_to(pathBuilder, right, y + trr, false, absolute);

	moz2d_path_sink_line_to(pathBuilder, right, bottom - brr, absolute);
	moz2d_path_sink_arc_to(pathBuilder, right - brr, bottom, false, absolute);

	moz2d_path_sink_line_to(pathBuilder, x + blr, bottom, absolute);
	moz2d_path_sink_arc_to(pathBuilder, x, bottom - blr, false, absolute);
	moz2d_path_sink_close(pathBuilder);

	Path* path = moz2d_path_builder_finish(pathBuilder);
	delete pathBuilder;

	return path;
}

Path* moz2d_shape_ellipse (DrawTarget* drawTarget,
		float x, float y, float width, float height, FillRule aFillRule) {

	// if ellipse degrades to circle we create a circle
	if (floatEquals(width, height)) {
		float radius = width / 2;
		float centerX = x + radius;
		float centerY = y + radius;

		return moz2d_shape_circle (drawTarget, centerX, centerY, radius, aFillRule);
	}

	float halfWidth = width / 2;
	float halfHeight = height / 2;
	float halfWidth055 = halfWidth * 0.55f;
	float halfHeight055 = halfHeight * 0.55f;
	float halfWidth045 = halfWidth * 0.45f;
	float halfHeight045 = halfHeight * 0.45f;

	bool absolute = false;

	PathBuilder* pathBuilder = drawTarget->CreatePathBuilder(aFillRule).take();

	moz2d_path_sink_move_to(pathBuilder, x, y, absolute);
	moz2d_path_sink_move_to(pathBuilder, halfWidth, 0, absolute);
	moz2d_path_sink_bezier_to(pathBuilder, halfWidth055, 0, halfWidth, halfHeight045, halfWidth, halfHeight, absolute);
	moz2d_path_sink_bezier_to(pathBuilder, 0, halfHeight055, -halfWidth045, halfHeight, -halfWidth, halfHeight, absolute);
	moz2d_path_sink_bezier_to(pathBuilder, -halfWidth055, 0, -halfWidth, -halfHeight045, -halfWidth, -halfHeight, absolute);
	moz2d_path_sink_bezier_to(pathBuilder, 0, -halfHeight055, halfWidth045, -halfHeight, halfWidth, -halfHeight, absolute);
	moz2d_path_sink_close(pathBuilder);

	Path* path = moz2d_path_builder_finish(pathBuilder);
	delete pathBuilder;

	return path;
}

void moz2d_path_get_stroked_bounds(Path* path, StrokeOptions* aStrokeOptions, Rect* retBounds) {
	Rect bounds = path->GetStrokedBounds(*aStrokeOptions);
	retBounds->x = bounds.x;
	retBounds->y = bounds.y;
	retBounds->width = bounds.width;
	retBounds->height = bounds.height;
}
