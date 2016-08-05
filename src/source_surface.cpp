/*
 * source_surface.cpp
 *
 *  Created on: Aug 3, 2016
 *      Author: Aliaksei Syrel
 */

#include "source_surface.h"
#include "2d/Logging.h"

DataSourceSurface* moz2d_data_source_surface_create(SurfaceFormat aFormat, int32_t width, int32_t height, bool isZero) {
	return Factory::CreateDataSourceSurface(IntSize(width, height), aFormat,  isZero).take();
}

DataSourceSurface::MappedSurface* moz2d_source_surface_map(DataSourceSurface* surface, DataSourceSurface::MapType aMapType) {
	DataSourceSurface::MappedSurface mapping;
	if (!surface->Map(DataSourceSurface::MapType::WRITE, &mapping)) {
		gfxCriticalError() << "moz2d_source_surface_map failed to map surface";
		return nullptr;
	}
	return &mapping;
}
