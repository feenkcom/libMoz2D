/*
 * structs.h
 *
 *  Created on: Jul 22, 2016
 *      Author: aliaksei
 */

#ifndef STRUCTS_H_
#define STRUCTS_H_

#include "2d/Point.h"

using namespace mozilla::gfx;

struct Point_t {
	float x;
	float y;
};

#ifdef __cplusplus
extern "C" {
#endif

Point_t moz2d_Point_to_t (Point point);

#ifdef __cplusplus
}
#endif

#endif /* STRUCTS_H_ */
