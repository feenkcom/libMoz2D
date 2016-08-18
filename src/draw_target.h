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
uint32_t * moz2d_draw_target_get_data(DrawTarget* drawTarget);
int32_t moz2d_draw_target_get_stride(DrawTarget* drawTarget);
/* --------------------------------------------------- */
/* ------------------- C L I P P I N G --------------- */
/* --------------------------------------------------- */

/**
 * Pop a clip from the DrawTarget. A pop without a corresponding push will
 * be ignored.
 */
void moz2d_draw_target_pop_clip(DrawTarget* drawTarget);

/**
 * Push an axis-aligned rectangular clip to the DrawTarget. This rectangle
 * is specified in user space.
 */
void moz2d_draw_target_push_clip_rectangle (DrawTarget* drawTarget, float x, float y, float width, float height);

/**
 * Push a clip to the DrawTarget.
 */
void moz2d_draw_target_push_clip_path (DrawTarget* drawTarget, Path* aPath);

/**
 * Stores current clipping bounds in device space in rectangle
 */
void moz2d_draw_target_clipping_bounds_global(DrawTarget* drawTarget, Rect* rectangle);

/**
 * Stores current clipping bounds in user space in rectangle
 */
void moz2d_draw_target_clipping_bounds_local(DrawTarget* drawTarget, Rect* rectangle);
/* --------------------------------------------------- */
/* -------------------- D R A W I N G ---------------- */
/* --------------------------------------------------- */

/*
 * Fill
 */
void moz2d_draw_target_fill_rect(DrawTarget* drawTarget, Rect* rect, Pattern* pattern, DrawOptions* drawOptions);
void moz2d_draw_target_fill_rect_color(DrawTarget* drawTarget, float x, float y, float width, float height, float r, float g, float b, float a, DrawOptions* drawOptions);
void moz2d_draw_target_fill_path(DrawTarget* drawTarget, Path* path, Pattern* pattern, DrawOptions* drawOptions);
void moz2d_draw_target_fill_path_color (DrawTarget* drawTarget, Path* path, float r, float g, float b, float a, DrawOptions* drawOptions);
/*
 * Stroke
 */
void moz2d_draw_target_stroke_rect(DrawTarget* drawTarget, Rect* rect, Pattern* pattern, StrokeOptions* strokeOptions, DrawOptions* drawOptions);
void moz2d_draw_target_stroke_rect_color(DrawTarget* drawTarget, float x, float y, float width, float height, float r, float g, float b, float a, StrokeOptions* strokeOptions, DrawOptions* drawOptions);
void moz2d_draw_target_stroke_path(DrawTarget* drawTarget, Path* path, Pattern* pattern, StrokeOptions* strokeOptions, DrawOptions* drawOptions);
void moz2d_draw_target_stroke_path_color(DrawTarget* drawTarget, Path* path, float r, float g, float b, float a, StrokeOptions* strokeOptions, DrawOptions* drawOptions);
/*
 * Mask
 */
void moz2d_draw_target_mask_pattern(DrawTarget* drawTarget, Pattern* aSource, Pattern* aMask, DrawOptions* aOptions);
void moz2d_draw_target_mask_surface(DrawTarget* drawTarget, Pattern* aSource, SourceSurface *aMask, Float offsetX, Float offsetY, DrawOptions* aOptions);

/*
 * Filter
 */
void moz2d_draw_target_draw_filter(DrawTarget* drawTarget, FilterNode* aFilter, Rect* sourceRect, Float destX, Float destY, DrawOptions* drawOptions);

/*
 * Path
 */
PathBuilder* moz2d_draw_target_create_path_builder(DrawTarget* drawTarget, FillRule aFillRule);

/* --------------------------------------------------- */
/* ----------- T R A N S F O R M A T I O N ----------- */
/* --------------------------------------------------- */

/**
 * Return current transformation matrix as components array of 6 elements
 */
void moz2d_draw_target_transform_get(DrawTarget* drawTarget, float* array);

/**
 * Set a transform on the surface, this transform is applied at drawing time
 * to both the mask and source of the operation.
 *
 * Performance note: For some backends it is expensive to change the current
 * transform (because transforms affect a lot of the parts of the pipeline,
 * so new transform change can result in a pipeline flush).  To get around
 * this, DrawTarget implementations buffer transform changes and try to only
 * set the current transform on the backend when required.  That tracking has
 * its own performance impact though, and ideally callers would be smart
 * enough not to require it.  At a future date this method may stop this
 * doing transform buffering so, if you're a consumer, please try to be smart
 * about calling this method as little as possible.  For example, instead of
 * concatenating a translation onto the current transform then calling
 * FillRect, try to integrate the translation into FillRect's aRect
 * argument's x/y offset.
 */
void moz2d_draw_target_transform_set(DrawTarget* drawTarget, float* rawMatrix);

/**
 * Concatenate current transformation with provided one
 */
void moz2d_draw_target_transform_concatenate(DrawTarget* drawTarget, float a11, float a12, float a21, float a22, float a31, float a32);

/**
 * Push current transform on a stack
 */
void moz2d_draw_target_transform_push(DrawTarget* drawTarget);

/**
 * Pops current transform from the stack
 */
void moz2d_draw_target_transform_pop(DrawTarget* drawTarget);

#ifdef __cplusplus
}
#endif

#endif /* DRAW_TARGET_SKIA_H_ */
