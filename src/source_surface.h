/*
 * source_surface.h
 *
 *  Created on: Aug 3, 2016
 *      Author: Aliaksei Syrel
 */

#ifndef SOURCE_SURFACE_H_
#define SOURCE_SURFACE_H_


#include "2d/2D.h"

using namespace mozilla::gfx;

#ifdef __cplusplus
extern "C" {
#endif

DataSourceSurface* moz2d_data_source_surface_create(SurfaceFormat aFormat, int32_t width, int32_t height, bool isZero);
DataSourceSurface::MappedSurface* moz2d_source_surface_map(DataSourceSurface* surface, DataSourceSurface::MapType aMapType);

#ifdef __cplusplus
}
#endif

#endif /* SOURCE_SURFACE_H_ */
