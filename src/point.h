/*
 * point.h
 *
 *  Created on: Jul 24, 2016
 *      Author: aliaksei
 */

#ifndef POINT_H_
#define POINT_H_

#include "exports.h"
#include "2d/Point.h"

using namespace mozilla::gfx;

#ifdef __cplusplus
extern "C" {
#endif

LIBRARY_API Float moz2d_size_width (Size *size);
LIBRARY_API Float moz2d_size_height (Size *size);


#ifdef __cplusplus
}
#endif



#endif /* POINT_H_ */
