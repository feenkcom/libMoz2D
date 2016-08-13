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

/* --------------------------------------------------- */
/* ----------------- C R E A T I O N ----------------- */
/* --------------------------------------------------- */

/**
 * Create a new draw target of a type that fits best for current platform and aFormat pixel format
 */
DrawTarget* moz2d_draw_target_create(int32_t width, int32_t height, SurfaceFormat aFormat);

/**
 * Create a new draw target of aBackend type and aFormat pixel format
 */
DrawTarget* moz2d_draw_target_create_type(BackendType aBackend, int32_t width, int32_t height, SurfaceFormat aFormat);

/**
 * Create a new draw target for data (pixels) of aBackend type
 */
DrawTarget* moz2d_draw_target_create_for_data_type(BackendType aBackend, unsigned char* aData, int32_t width, int32_t height, int32_t aStride, SurfaceFormat aFormat, bool aUninitialized);

/**
 * Create a new draw target for data (pixels) of the type that fits best current platform.
 */
DrawTarget* moz2d_draw_target_create_for_data(unsigned char* aData, int32_t width, int32_t height, int32_t aStride, SurfaceFormat aFormat);

/**
 * Deletes draw target and frees memory
 */
void moz2d_draw_target_delete(DrawTarget *drawTarget);

/* --------------------------------------------------- */
/* ------------------- T E S T I N G ----------------- */
/* --------------------------------------------------- */

/**
 * Return true if draw target is valid, false otherwise
 */
bool moz2d_draw_target_is_valid(DrawTarget *drawTarget);

/**
 * Return true if draw target is recording, false otherwise
 */
bool moz2d_draw_target_is_recording(DrawTarget *drawTarget);

/**
 * Return true if subpixel antialias is permitted, false otherwise
 */
bool moz2d_draw_target_get_permit_subpixel_aa(DrawTarget *drawTarget);

/* --------------------------------------------------- */
/* ----------------- A C C E S S I N G --------------- */
/* --------------------------------------------------- */

/**
 * Return a type of draw target (software, hardware, vector)
 */
DrawTargetType moz2d_draw_target_get_type (DrawTarget *drawTarget);

/**
 * Return a backend type of draw target (cairo, skia, cg, d2d1, etc)
 */
BackendType moz2d_draw_target_get_backend_type (DrawTarget *drawTarget);

/**
 * Return a surface format (pixel format) of draw target
 */
SurfaceFormat moz2d_draw_target_get_surface_format (DrawTarget *drawTarget);

/**
 * Store a size (extent) of draw target in size argument
 */
void moz2d_draw_target_get_size(DrawTarget *drawTarget, IntSize* aSize);


void moz2d_draw_target_set_permit_subpixel_aa(DrawTarget *drawTarget, bool aPermitSubpixelAA);
SourceSurface* moz2d_draw_target_snapshot(DrawTarget *drawTarget);
void moz2d_draw_target_flush(DrawTarget *drawTarget);

/* --------------------------------------------------- */
/* -------------------- D R A W I N G ---------------- */
/* --------------------------------------------------- */

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
