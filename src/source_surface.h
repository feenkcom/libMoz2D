/*
 * source_surface.h
 *
 *  Created on: Aug 3, 2016
 *      Author: Aliaksei Syrel
 */

#ifndef SOURCE_SURFACE_H_
#define SOURCE_SURFACE_H_

#include "exports.h"
#include "2d/2D.h"

using namespace mozilla::gfx;

#ifdef __cplusplus
extern "C" {
#endif

LIBRARY_API DataSourceSurface* moz2d_data_source_surface_create(SurfaceFormat aFormat, int32_t width, int32_t height, bool isZero);
LIBRARY_API uint8_t * moz2d_source_surface_map(DataSourceSurface* surface, int32_t * retStride);
LIBRARY_API void moz2d_source_surface_copy(uint8_t * aSource, int32_t aSourceStride, uint8_t * aDest, int32_t aDestStride, int32_t x, int32_t y, int32_t width, int32_t height);
LIBRARY_API void moz2d_source_surface_unmap(DataSourceSurface* surface);

#ifdef __cplusplus
}
#endif

#endif /* SOURCE_SURFACE_H_ */
