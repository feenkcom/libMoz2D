/*
 * path.cpp
 *
 *  Created on: Jul 14, 2016
 *      Author: aliaksei
 */

#include "path.h"

#include <math.h>
#include <algorithm>

#define PI 				3.141592654
#define TwoPI			6.283185307
#define HalfPI			1.570796327
#define ThreeHalvesPI	4.71238898
#define SQRT2 			1.414213562

float angleOfVector(Point vector) {
	vector = vector / vector.Length();
	float arccos = acos(vector.x);

	return vector.y < 0 ? TwoPI - arccos : arccos;
}

Point calcCenter(Point start, Point end, float angle) {
	Point vector = end - start;

	Matrix matrix = Matrix::Rotation((PI - angle) / 2);

	vector = matrix * vector;

	float length = sqrt(vector.DotProduct(vector));
	float radius = (length / 2) / sin (angle / 2);

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
	float radius = sqrt(vector.DotProduct(vector));
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
			startAngle = aAntiClockwise ? PI : HalfPI;
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
			endAngle = aAntiClockwise ? HalfPI : HalfPI;
		}
		else {
			center = start + (aAntiClockwise ? Point(-radius, 0) : Point(0, -radius));
			startAngle = aAntiClockwise ? 0 : HalfPI;
			endAngle = aAntiClockwise ? HalfPI : PI;
		}
	}

	moz2d_path_sink_arc(pathSink, center.x, center.y, radius, startAngle, endAngle, aAntiClockwise, true);
}

void moz2d_path_sink_ellipse_arc_to(PathSink* pathSink, float endX, float endY, bool aAntiClockwise, bool absolute) {
//	Point start = pathSink->CurrentPoint();
//	Point end = start + Point(radius * vectorX, radius * vectorY);
//
//	Point center;
//	float startAngle;
//	float endAngle;
//	if (end.x > start.x) {
//		if (end.y > start.y) {
//			center = start + (aAntiClockwise ? Point(radius, 0) : Point(0, radius));
//			startAngle = aAntiClockwise ? PI : HalfPI;
//			endAngle = aAntiClockwise ? HalfPI : 0;
//		}
//		else {
//			center = start + (aAntiClockwise ? Point(0, -radius) : Point(radius, 0));
//			startAngle = aAntiClockwise ? HalfPI : PI;
//			endAngle = aAntiClockwise ? 0 : ThreeHalvesPI;
//		}
//	}
//	else {
//		if (end.y > start.y) {
//			center = start + (aAntiClockwise ? Point(0, radius) : Point(-radius, 0));
//			startAngle = aAntiClockwise ? PI : 0;
//			endAngle = aAntiClockwise ? HalfPI : HalfPI;
//		}
//		else {
//			center = start + (aAntiClockwise ? Point(-radius, 0) : Point(0, -radius));
//			startAngle = aAntiClockwise ? 0 : HalfPI;
//			endAngle = aAntiClockwise ? HalfPI : PI;
//		}
//	}
//
//	moz2d_path_sink_arc(pathSink, center.x, center.y, radius, startAngle, endAngle, aAntiClockwise, true);
}

void moz2d_path_sink_arc_to (PathSink* pathSink, float endX, float endY, bool aAntiClockwise, bool absolute) {
	Point start = pathSink->CurrentPoint();
	Point end = Point(endX, endY);

	Point vector = absolute ? (end - start) : end;

	moz2d_path_sink_circle_arc_to(pathSink, vector.x, sign(vector.x), sign(vector.y), aAntiClockwise);
}

void moz2d_path_sink_current_point(PathSink* pathSink, Point* point) {
	Point currentPoint = pathSink->CurrentPoint();
	point->x = currentPoint.x;
	point->y = currentPoint.y;
}

Path* moz2d_path_builder_finish(PathBuilder* pathBuilder) {
	return pathBuilder->Finish().take();
}

Path* moz2d_shape_rounded_rectangle (DrawTarget* drawTarget,
		float x, float y, float width, float height,
		float topLeftRadius, float topRightRadius, float bottomRightRadius, float bottomLeftRadius, FillRule aFillRule) {

	float min = std::min((x + width) , (y + height)) / 2;
	float tlr = std::min(topLeftRadius, min);
	float trr = std::min(topRightRadius, min);
	float brr = std::min(bottomRightRadius, min);
	float blr = std::min(bottomLeftRadius, min);

	bool absolute = true;

	PathBuilder* pathBuilder = drawTarget->CreatePathBuilder(aFillRule).take();

	moz2d_path_sink_move_to(pathBuilder, x, tlr, absolute);
	moz2d_path_sink_arc_to(pathBuilder, tlr, y, false, absolute);
	moz2d_path_sink_line_to(pathBuilder, x + width - trr, y, absolute);

	moz2d_path_sink_arc_to(pathBuilder, x + width, trr, false, absolute);
	moz2d_path_sink_line_to(pathBuilder, x + width, y + height - brr, absolute);

	moz2d_path_sink_arc_to(pathBuilder, x + width - brr, y + height, false, absolute);
	moz2d_path_sink_line_to(pathBuilder, x + blr, y + height, absolute);
	moz2d_path_sink_arc_to(pathBuilder, x, y + height - blr, false, absolute);
	moz2d_path_sink_close(pathBuilder);

	Path* path = moz2d_path_builder_finish(pathBuilder);
	delete pathBuilder;

	return path;
}

Path* moz2d_shape_ellipse (DrawTarget* drawTarget,
		float x, float y, float width, float height, FillRule aFillRule) {

	float halfWidth = width / 2;
	float halfHeight = height / 2;

	bool relative = false;

	PathBuilder* pathBuilder = drawTarget->CreatePathBuilder(aFillRule).take();

	moz2d_path_sink_move_to(pathBuilder, halfWidth, y, relative);
	moz2d_path_sink_bezier_to(pathBuilder, halfWidth * 0.55, y, halfWidth, halfHeight * 0.45, halfWidth, halfHeight, relative);
	moz2d_path_sink_bezier_to(pathBuilder, x, halfHeight * 0.55, -halfWidth * 0.45, halfHeight, -halfWidth, halfHeight, relative);
	moz2d_path_sink_bezier_to(pathBuilder, -halfWidth * 0.55, y, -halfWidth, -halfHeight * 0.45, -halfWidth, -halfHeight, relative);
	moz2d_path_sink_bezier_to(pathBuilder, x, -halfHeight * 0.55, halfWidth * 0.45, -halfHeight, halfWidth, -halfHeight, relative);
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
