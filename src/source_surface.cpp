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

uint8_t * moz2d_source_surface_map(DataSourceSurface* surface, int32_t * retStride) {
	DataSourceSurface::MappedSurface mapping;
	if (!surface->Map(DataSourceSurface::MapType::READ, &mapping)) {
		gfxCriticalError() << "moz2d_source_surface_map failed to map surface";
		if (retStride) *retStride = 0;
		return nullptr;
	}
	if (retStride) *retStride = mapping.mStride;
	return mapping.mData;
}

void moz2d_source_surface_unmap(DataSourceSurface* surface) {
	surface->Unmap();
}

void moz2d_source_surface_copy(uint8_t * aSource, int32_t aSourceStride, uint8_t * aDest, int32_t aDestStride, int32_t x, int32_t y, int32_t width, int32_t height) {
	uint32_t *sourceData = (uint32_t*) aSource;
	uint32_t *destData = (uint32_t*) aDest;
	int32_t sourceWidth = aSourceStride / 4;
	int32_t destWidth = aDestStride / 4;
	int32_t sourceOffset = y * sourceWidth + x;
	int32_t destOffset = y * destWidth + x;

	uint32_t bufferSize = width * 4;

	for (int32_t i = 0; i < height; ++i) {
		memcpy(destData + destOffset, sourceData + sourceOffset, bufferSize);
		sourceOffset += sourceWidth;
		destOffset += destWidth;
	}
}
