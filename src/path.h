/*
 * path.h
 *
 *  Created on: Jul 14, 2016
 *      Author: aliaksei
 */

#ifndef PATH_H_
#define PATH_H_

#include "2d/2D.h"

using namespace mozilla::gfx;

#ifdef __cplusplus
extern "C" {
#endif

/** Move the current point in the path, any figure currently being drawn will
 * be considered closed during fill operations, however when stroking the
 * closing line segment will not be drawn.
 */
void moz2d_path_sink_move_to(PathSink* pathSink, float x, float y, bool absolute);

/** Add a line segment to the current figure */
void moz2d_path_sink_line_to(PathSink* pathSink, float x, float y, bool absolute);

/** Add a cubic bezier curve to the current figure */
void moz2d_path_sink_bezier_to(PathSink* pathSink, float x1, float y1, float x2, float y2, float x3, float y3, bool absolute);

/** Add a quadratic bezier curve to the current figure */
void moz2d_path_sink_quadratic_bezier_to(PathSink* pathSink, float x1, float y1, float x2, float y2, bool absolute);

/** Close the current figure, this will essentially generate a line segment
 * from the current point to the starting point for the current figure
 */
void moz2d_path_sink_close(PathSink* pathSink);

/** Add an arc to the current figure */
void moz2d_path_sink_arc(PathSink* pathSink, float originX, float originY, float aRadius, float aStartAngle, float anEndAngle, bool aAntiClockwise, bool absolute);

/**
 * Add an arc to the current figure
 */
void moz2d_path_sink_arc_to_angle(PathSink* pathSink, float endX, float endY, float angle, bool aAntiClockwise, bool absolute);

/**
 * Add an arc to the current figure, assuming angle is Pi / 2
 */
void moz2d_path_sink_arc_to (PathSink* pathSink, float endX, float endY, bool aAntiClockwise, bool absolute);

void moz2d_path_sink_circle_arc_to(PathSink* pathSink, float radius, int8_t vectorX, int8_t vectorY, bool aAntiClockwise);
void moz2d_path_sink_ellipse_arc_to(PathSink* pathSink, float endX, float endY, bool aAntiClockwise, bool absolute);

/** Point the current subpath is at - or where the next subpath will start
 * if there is no active subpath.
 */
void moz2d_path_sink_current_point(PathSink* pathSink, Point* point);

/** Finish writing to the path and return a Path object that can be used for
 * drawing. Future use of the builder results in a crash!
 */
Path* moz2d_path_builder_finish(PathBuilder* pathBuilder);

Path* moz2d_shape_rounded_rectangle(DrawTarget* drawTarget, float x, float y, float width, float height, float topLeftRadius, float topRightRadius, float bottomRightRadius, float bottomLeftRadius, FillRule aFillRule);
Path* moz2d_shape_ellipse (DrawTarget* drawTarget, float x, float y, float width, float height, FillRule aFillRule);
Path* moz2d_shape_circle (DrawTarget* drawTarget, float x, float y, float radius, FillRule aFillRule);

void moz2d_path_get_stroked_bounds(Path* path, StrokeOptions* aStrokeOptions, Rect* retBounds);

#ifdef __cplusplus
}
#endif

#endif /* PATH_H_ */
