/*
 * nsTextRunTransformationsFixed.cpp
 *
 *  Created on: Aug 1, 2016
 *      Author: Aliaksei Syrel
 */

#include "mozilla/gfx/2D.h"

/*
 * For some strange reason DrawTarget is used without
 * namespace, like it is defined in header.
 * This way we use needed namespace and include
 * nsTextRunTransformations.cpp file directly
 */
using namespace mozilla::gfx;

/*
 * UniquePtr is also used without namespace
 */
using namespace mozilla;

#include "layout/generic/nsTextRunTransformations.cpp"
