/*
 * path.cpp
 *
 *  Created on: Jul 14, 2016
 *      Author: aliaksei
 */

#include "path.h"

#include <math.h>
#define PI 3.14159265

float angleOfVector(Point vector) {
	vector = vector / vector.Length();
	float arccos = acos(vector.x);

	return vector.y < 0 ? PI * 2 - arccos : arccos;
}

Point calcCenter(Point start, Point end, float angle) {
	Point vector = end - start;

	Matrix matrix = Matrix::Rotation((PI - angle) / 2);

	vector = matrix * vector;

	float length = sqrt(vector.DotProduct(vector));
	float radius = (length / 2) / sin (angle / 2);

	return vector * (radius / length) + start;
}

void moz2d_path_sink_move_to(PathSink* pathSink, float x, float y) {
	pathSink->MoveTo(Point(x,y));
}

void moz2d_path_sink_line_to(PathSink* pathSink, float x, float y) {
	pathSink->LineTo(Point(x,y));
}

void moz2d_path_sink_bezier_to(PathSink* pathSink, float x1, float y1, float x2, float y2, float x3, float y3) {
	pathSink->BezierTo(Point(x1,y1), Point(x2,y2), Point(x3,y3));
}

void moz2d_path_sink_quadratic_bezier_to(PathSink* pathSink, float x1, float y1, float x2, float y2) {
	pathSink->QuadraticBezierTo(Point(x1,y1), Point(x2,y2));
}

void moz2d_path_sink_close(PathSink* pathSink) {
	pathSink->Close();
}

void moz2d_path_sink_arc(PathSink* pathSink, float originX, float originY, float aRadius, float aStartAngle, float anEndAngle, bool anAntiClockwise) {
		pathSink->Arc(Point(originX, originY), aRadius, aStartAngle,anEndAngle, anAntiClockwise);
}

void moz2d_path_sink_arc_to_angle(PathSink* pathSink, Float endX, Float endY, Float angle, bool aAntiClockwise) {
	if (angle == 0) {
		return moz2d_path_sink_line_to(pathSink, endX, endY);
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

	moz2d_path_sink_arc(pathSink, center.x, center.y, radius, startAngle, endAngle, aAntiClockwise);
}

void moz2d_path_sink_arc_to (PathSink* pathSink, float endX, float endY, bool aAntiClockwise) {
	Point start = pathSink->CurrentPoint();
	Point end = Point(endX, endY);

	Point center = (end - start) / 2;
	float radius = center.Length();
	center += start;

	moz2d_path_sink_arc(pathSink, center.x, center.y, radius, 0, PI, aAntiClockwise);
}


void moz2d_path_sink_current_point(PathSink* pathSink, Point* point) {
	Point currentPoint = pathSink->CurrentPoint();
	point->x = currentPoint.x;
	point->y = currentPoint.y;
}

Path* moz2d_path_builder_finish(PathBuilder* pathBuilder) {
	return pathBuilder->Finish().take();
}
