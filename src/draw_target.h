/*
 * draw_target_skia.h
 *
 *  Created on: May 29, 2016
 *      Author: aliaksei
 */

#ifndef DRAW_TARGET_H_
#define DRAW_TARGET_H_

#include "2d/2D.h"

typedef void* CDrawTarget;
typedef void* CSourceSurface;
typedef void* CPattern;
typedef void* CColorPattern;

using namespace mozilla::gfx;

#ifdef __cplusplus
extern "C" {
#endif

DrawTarget* moz2d_draw_target_create(BackendType aBackend, int32_t width, int32_t height, SurfaceFormat aFormat);
DrawTarget* moz2d_draw_target_create_for_data(BackendType aBackend, unsigned char* aData, int32_t width, int32_t height, int32_t aStride, SurfaceFormat aFormat, bool aUninitialized);

void moz2d_draw_target_delete(DrawTarget *drawTarget);
bool moz2d_draw_target_is_valid(DrawTarget *drawTarget);
DrawTargetType moz2d_draw_target_get_type (DrawTarget *drawTarget);
BackendType moz2d_draw_target_get_backend_type (DrawTarget *drawTarget);
bool moz2d_draw_target_is_recording(DrawTarget *drawTarget);
bool moz2d_draw_target_get_permit_subpixel_aa(DrawTarget *drawTarget);
void moz2d_draw_target_set_permit_subpixel_aa(DrawTarget *drawTarget, bool aPermitSubpixelAA);
SourceSurface* moz2d_draw_target_snapshot(DrawTarget *drawTarget);
IntSize* moz2d_draw_target_get_size(DrawTarget *drawTarget);
void moz2d_draw_target_flush(DrawTarget *drawTarget);

/*
 * Fill
 */
void moz2d_draw_target_fill_rect(DrawTarget* drawTarget, Rect* rect, Pattern* pattern, DrawOptions* drawOptions);
void moz2d_draw_target_fill_path(DrawTarget* drawTarget, Path* path, Pattern* pattern, DrawOptions* drawOptions);
/*
 * Stroke
 */
void moz2d_draw_target_stroke_rect(DrawTarget* drawTarget, Rect* rect, Pattern* pattern, StrokeOptions* strokeOptions, DrawOptions* drawOptions);
void moz2d_draw_target_stroke_path(DrawTarget* drawTarget, Path* path, Pattern* pattern, StrokeOptions* strokeOptions, DrawOptions* drawOptions);
/*
 * Mask
 */
void moz2d_draw_target_mask_pattern(DrawTarget* drawTarget, Pattern* aSource, Pattern* aMask, DrawOptions* aOptions);
void moz2d_draw_target_mask_surface(DrawTarget* drawTarget, Pattern* aSource, SourceSurface *aMask, Float offsetX, Float offsetY, DrawOptions* aOptions);

void moz2d_draw_target_draw_filter(DrawTarget* drawTarget, FilterNode* aFilter, Rect* sourceRect, Float destX, Float destY, DrawOptions* drawOptions);
PathBuilder* moz2d_draw_target_create_path_builder(DrawTarget* drawTarget, FillRule aFillRule);



//
uint32_t * moz2d_draw_target_get_data(DrawTarget* drawTarget);

//int32_t moz2d_draw_target_stride(CDrawTarget drawTarget);
uint32_t moz2d_draw_target_get_pixel (CDrawTarget drawTarget, int32_t x, int32_t y);
//CSourceSurface moz2d_draw_target_get_surface(CDrawTarget drawTarget);
//
//void moz2d_draw_target_fill_rect(CDrawTarget drawTarget, Float x, Float y, Float width, Float height, CPattern pattern);
//void moz2d_draw_target_fill_rect_color(CDrawTarget drawTarget,
//		Float x, Float y, Float width, Float height,
//		Float red, Float green, Float blue, Float alpha);
//void moz2d_draw_target_draw_filter(CDrawTarget drawTarget, CFilterNode filter, Float x, Float y, Float width, Float height, Float destX, Float destY);
//
//CColorPattern moz2d_color_pattern_create(CColor color);
//void moz2d_color_pattern_delete(CColorPattern pattern);

#ifdef __cplusplus
}
#endif

#endif /* DRAW_TARGET_SKIA_H_ */
