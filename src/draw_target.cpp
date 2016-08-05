/*
 * draw_target.cpp
 *
 *  Created on: May 29, 2016
 *      Author: aliaksei
 */

#include "draw_target.h"
#include "2d/Filters.h"

using namespace mozilla::gfx;

DrawTarget* moz2d_draw_target_create(BackendType aBackend, int32_t width, int32_t height, SurfaceFormat aFormat) {
	return Factory::CreateDrawTarget(aBackend, IntSize(width, height), aFormat).take();
}

DrawTarget* moz2d_draw_target_create_for_data(BackendType aBackend, unsigned char* aData, int32_t width, int32_t height, int32_t aStride, SurfaceFormat aFormat, bool aUninitialized) {
	return Factory::CreateDrawTargetForData(aBackend, aData, IntSize(width, height), aStride, aFormat, aUninitialized).take();
}

void moz2d_draw_target_delete(DrawTarget *drawTarget) {
	delete drawTarget;
}

SourceSurface* moz2d_draw_target_snapshot(DrawTarget *drawTarget) {
	return drawTarget->Snapshot().take();
}

void moz2d_draw_target_flush(DrawTarget *drawTarget) {
	drawTarget->Flush();
}

bool moz2d_draw_target_get_permit_subpixel_aa(DrawTarget *drawTarget) {
	return drawTarget->GetPermitSubpixelAA();
}

void moz2d_draw_target_set_permit_subpixel_aa(DrawTarget *drawTarget, bool aPermitSubpixelAA) {
	drawTarget->SetPermitSubpixelAA(aPermitSubpixelAA);
}

/*
 * Fill
 */
void moz2d_draw_target_fill_rect(DrawTarget* drawTarget, Rect* rect, Pattern* pattern, DrawOptions* drawOptions) {
	drawTarget->FillRect(*rect, *pattern, *drawOptions);
}

void moz2d_draw_target_fill_path(DrawTarget* drawTarget, Path* path, Pattern* pattern, DrawOptions* drawOptions) {
	drawTarget->Fill(path, *pattern, *drawOptions);
}

/*
 * Stroke
 */
void moz2d_draw_target_stroke_rect(DrawTarget* drawTarget, Rect* rect, Pattern* pattern, StrokeOptions* strokeOptions, DrawOptions* drawOptions) {
	drawTarget->StrokeRect(*rect, *pattern, *strokeOptions, *drawOptions);
}

void moz2d_draw_target_stroke_path(DrawTarget* drawTarget, Path* path, Pattern* pattern, StrokeOptions* strokeOptions, DrawOptions* drawOptions) {
	drawTarget->Stroke(path, *pattern, *strokeOptions, *drawOptions);
}

/*
 * Mask
 */
void moz2d_draw_target_mask_pattern(DrawTarget* drawTarget, Pattern* aSource, Pattern* aMask, DrawOptions* aOptions) {
	drawTarget->Mask(*aSource, *aMask, *aOptions);
}

void moz2d_draw_target_mask_surface(DrawTarget* drawTarget, Pattern* aSource, SourceSurface *aMask, Float offsetX, Float offsetY, DrawOptions* aOptions) {
	drawTarget->MaskSurface(*aSource, aMask, Point(offsetX, offsetY), *aOptions);
}

void moz2d_draw_target_draw_filter(DrawTarget* drawTarget, FilterNode* aFilter, Rect* sourceRect, Float destX, Float destY, DrawOptions* drawOptions) {
	drawTarget->DrawFilter(aFilter, *sourceRect, Point(destX, destY), *drawOptions);
}

PathBuilder* moz2d_draw_target_create_path_builder(DrawTarget* drawTarget, FillRule aFillRule) {
	return drawTarget->CreatePathBuilder(aFillRule).take();
}

//
//DrawTargetType moz2d_draw_target_type (CDrawTarget drawTarget) {
//	return reinterpret_cast<DrawTarget*>(drawTarget)->GetType();
//}
//
//bool moz2d_draw_target_is_valid(CDrawTarget drawTarget) {
//	return reinterpret_cast<DrawTarget*>(drawTarget)->IsValid();
//}
//
//CSourceSurface moz2d_draw_target_get_surface(CDrawTarget drawTarget) {
//	already_AddRefed<SourceSurface> snapshot = reinterpret_cast<DrawTarget*>(drawTarget)->Snapshot();
//	return reinterpret_cast<void*>(snapshot.take());
//}
//

uint32_t * moz2d_draw_target_get_data(DrawTarget* drawTarget) {
	RefPtr<SourceSurface> snapshot = drawTarget->Snapshot();
	RefPtr<DataSourceSurface> mDataSnapshot = snapshot->GetDataSurface();
	return (uint32_t *) mDataSnapshot->GetData();
}

//
//
//void moz2d_draw_target_draw_filter(CDrawTarget drawTarget, CFilterNode filter, Float x, Float y, Float width, Float height, Float destX, Float destY) {
//	FilterNode* tmpFilter = reinterpret_cast<FilterNode*>(filter);
//	reinterpret_cast<DrawTarget*>(drawTarget)->DrawFilter(
//				tmpFilter,
//				Rect(x,y,width,height),
//				Point(destX, destY),
//				DrawOptions());
//}
//
//int32_t moz2d_draw_target_stride(CDrawTarget drawTarget){
//	RefPtr<SourceSurface> snapshot = reinterpret_cast<DrawTarget*>(drawTarget)->Snapshot();
//	RefPtr<DataSourceSurface> mDataSnapshot = snapshot->GetDataSurface();
//	return mDataSnapshot->Stride();
//}
//

uint32_t moz2d_draw_target_get_pixel (CDrawTarget drawTarget, int32_t x, int32_t y) {
	RefPtr<SourceSurface> snapshot = reinterpret_cast<DrawTarget*>(drawTarget)->Snapshot();
	RefPtr<DataSourceSurface> mDataSnapshot = snapshot->GetDataSurface();
	int32_t stride = mDataSnapshot->Stride();
	uint32_t* data = (uint32_t*) mDataSnapshot->GetData();
	return data[ y * ( stride / 4 ) + x ];
}
//
//
//CColorPattern moz2d_color_pattern_create(CColor color) {
//	Color* tmpColor = reinterpret_cast<Color*>(color);
//	return reinterpret_cast<void*>(new ColorPattern(*tmpColor));
//}
//
//void moz2d_color_pattern_delete(CColorPattern pattern) {
//	delete reinterpret_cast<ColorPattern*>(pattern);
//}
