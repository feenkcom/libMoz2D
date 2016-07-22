/*
 * path.cpp
 *
 *  Created on: Jul 14, 2016
 *      Author: aliaksei
 */

#include "path.h"

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

void moz2d_path_sink_current_point(PathSink* pathSink, Point* point) {
	Point currentPoint = pathSink->CurrentPoint();
	point->x = currentPoint.x;
	point->y = currentPoint.y;
}

Path* moz2d_path_builder_finish(PathBuilder* pathBuilder) {
	return pathBuilder->Finish().take();
}
