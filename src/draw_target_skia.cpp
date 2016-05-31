/*
 * draw_target_skia.cpp
 *
 *  Created on: May 29, 2016
 *      Author: aliaksei
 */

#include "2d/Factory.cpp"
#include "2d/2D.h"
#include "draw_target_skia.h"

using namespace mozilla::gfx;

CDrawTargeSkia draw_target_skia_create() {
	return reinterpret_cast<void*>(Factory::CreateDrawTarget(BackendType::SKIA, IntSizeTyped<UnknownUnits>(20,20), SurfaceFormat::X8R8G8B8).take());
}
