/*
 * structs.cpp
 *
 *  Created on: Jul 22, 2016
 *      Author: aliaksei
 */

#include "structs.h"

Point_t moz2d_Point_to_t(Point point){
	Point_t point_t;
	point_t.x = point.x;
	point_t.y = point.y;
	return point_t;
}