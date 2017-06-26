/*
 * draw_target_skia.h
 *
 *  Created on: May 29, 2016
 *      Author: aliaksei
 */

#ifndef DRAW_TARGET_H_
#define DRAW_TARGET_H_

#include "exports.h"
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
LIBRARY_API DrawTarget* moz2d_draw_target_create(int32_t width, int32_t height, SurfaceFormat aFormat);

/**
 * Create a new draw target of aBackend type and aFormat pixel format
 */
LIBRARY_API DrawTarget* moz2d_draw_target_create_type(BackendType aBackend, int32_t width, int32_t height, SurfaceFormat aFormat);

/**
 * Create a new draw target for data (pixels) of aBackend type
 */
LIBRARY_API DrawTarget* moz2d_draw_target_create_for_data_type(BackendType aBackend, unsigned char* aData, int32_t width, int32_t height, int32_t aStride, SurfaceFormat aFormat, bool aUninitialized);

/**
 * Create a new draw target for data (pixels) of the type that fits best current platform.
 */
LIBRARY_API DrawTarget* moz2d_draw_target_create_for_data(unsigned char* aData, int32_t width, int32_t height, int32_t aStride, SurfaceFormat aFormat);

LIBRARY_API DrawTarget* moz2d_draw_target_create_similar(DrawTarget* drawTarget, int32_t width, int32_t height);


LIBRARY_API SourceSurface* moz2d_draw_target_create_surface_for_data (
		DrawTarget* drawTarget,
		unsigned char *aData,
        int32_t width,
        int32_t height,
        int32_t aStride,
        SurfaceFormat aFormat);

LIBRARY_API SourceSurface* moz2d_draw_target_create_surface_for_data_form (
		DrawTarget* drawTarget,
		unsigned char *aData,
        int32_t width,
        int32_t height,
        int32_t aStride,
        SurfaceFormat aFormat);

LIBRARY_API SourceSurface* moz2d_draw_target_create_surface_from_native (
        DrawTarget* drawTarget,
		NativeSurfaceType aType,
		SurfaceFormat aFormat,
		int32_t width,
		int32_t height,
		void *aSurface);

/* --------------------------------------------------- */
/* ------------------- T E S T I N G ----------------- */
/* --------------------------------------------------- */

/**
 * Return true if draw target is valid, false otherwise
 */
LIBRARY_API bool moz2d_draw_target_is_valid(DrawTarget *drawTarget);

/**
 * Return true if draw target is recording, false otherwise
 */
LIBRARY_API bool moz2d_draw_target_is_recording(DrawTarget *drawTarget);

/**
 * Return true if subpixel antialias is permitted, false otherwise
 */
LIBRARY_API bool moz2d_draw_target_get_permit_subpixel_aa(DrawTarget *drawTarget);

/* --------------------------------------------------- */
/* ----------------- A C C E S S I N G --------------- */
/* --------------------------------------------------- */

/**
 * Return a type of draw target (software, hardware, vector)
 */
LIBRARY_API DrawTargetType moz2d_draw_target_get_type (DrawTarget *drawTarget);

/**
 * Return a backend type of draw target (cairo, skia, cg, d2d1, etc)
 */
LIBRARY_API BackendType moz2d_draw_target_get_backend_type (DrawTarget *drawTarget);

/**
 * Return a surface format (pixel format) of draw target
 */
LIBRARY_API SurfaceFormat moz2d_draw_target_get_surface_format (DrawTarget *drawTarget);

/**
 * Store a size (extent) of draw target in size argument
 */
LIBRARY_API void moz2d_draw_target_get_size(DrawTarget *drawTarget, IntSize* aSize);

/**
 * Store a size (extent) of source surface in size argument
 */
LIBRARY_API void moz2d_source_surface_get_size(SourceSurface *aSourceSurface, IntSize* aSize);
/**
 * Return a surface format (pixel format)
 */
LIBRARY_API SurfaceFormat moz2d_source_surface_get_format(SourceSurface *aSourceSurface);
/**
 * Return a surface type
 */
LIBRARY_API SurfaceType moz2d_source_surface_get_type(SourceSurface *aSourceSurface);

LIBRARY_API void moz2d_draw_target_set_permit_subpixel_aa(DrawTarget *drawTarget, bool aPermitSubpixelAA);
LIBRARY_API SourceSurface* moz2d_draw_target_snapshot(DrawTarget *drawTarget);
LIBRARY_API void moz2d_draw_target_flush(DrawTarget *drawTarget);
LIBRARY_API uint32_t * moz2d_draw_target_get_data(DrawTarget* drawTarget);
LIBRARY_API int32_t moz2d_draw_target_get_stride(DrawTarget* drawTarget);

/**
 * Copies pixels into Form's pixel buffer
 */
LIBRARY_API void moz2d_draw_target_as_form(DrawTarget* drawTarget, uint32_t *aData);
/* --------------------------------------------------- */
/* ------------------- C L I P P I N G --------------- */
/* --------------------------------------------------- */

/**
 * Pop a clip from the DrawTarget. A pop without a corresponding push will
 * be ignored.
 */
LIBRARY_API void moz2d_draw_target_pop_clip(DrawTarget* drawTarget);

/**
 * Push an axis-aligned rectangular clip to the DrawTarget. This rectangle
 * is specified in user space.
 */
LIBRARY_API void moz2d_draw_target_push_clip_rectangle (DrawTarget* drawTarget, Float x, Float y, Float width, Float height);

/**
 * Push a clip to the DrawTarget.
 */
LIBRARY_API void moz2d_draw_target_push_clip_path (DrawTarget* drawTarget, Path* aPath);

/**
 * Stores current clipping bounds in device space in rectangle
 */
LIBRARY_API void moz2d_draw_target_clipping_bounds_global(DrawTarget* drawTarget, Rect* rectangle);

/**
 * Stores current clipping bounds in user space in rectangle
 */
LIBRARY_API void moz2d_draw_target_clipping_bounds_local(DrawTarget* drawTarget, Rect* rectangle);

LIBRARY_API bool moz2d_draw_target_clipping_is_in_local(DrawTarget* drawTarget, Float x, Float y, Float width, Float height);

LIBRARY_API bool moz2d_draw_target_clipping_is_in_global(DrawTarget* drawTarget, Float x, Float y, Float width, Float height);

/* --------------------------------------------------- */
/* ----------------------- LAYERS -------------------- */
/* --------------------------------------------------- */

LIBRARY_API void moz2d_draw_target_push_layer(DrawTarget* drawTarget, bool aOpaque, Float aOpacity, SourceSurface* aMask, Float a11, Float a12, Float a21, Float a22, Float a31, Float a32, int32_t x, int32_t y, int32_t width, int32_t height, bool aCopyBackground);
LIBRARY_API void moz2d_draw_target_pop_layer(DrawTarget* drawTarget);

/* --------------------------------------------------- */
/* -------------------- D R A W I N G ---------------- */
/* --------------------------------------------------- */

/*
 * Fill
 */
LIBRARY_API void moz2d_draw_target_fill_rect(DrawTarget* drawTarget, Rect* rect, Pattern* pattern, DrawOptions* drawOptions);
LIBRARY_API void moz2d_draw_target_fill_rect_color(DrawTarget* drawTarget, Float x, Float y, Float width, Float height, Float r, Float g, Float b, Float a, DrawOptions* drawOptions);
LIBRARY_API void moz2d_draw_target_fill_rectangle_form (DrawTarget* drawTarget, Float x, Float y, Float width, Float height, DrawOptions* drawOptions, unsigned char *aFormData, int32_t aFormWidth, int32_t aFormHeight, int32_t aStride);

LIBRARY_API void moz2d_draw_target_fill_path(DrawTarget* drawTarget, Path* path, Pattern* pattern, DrawOptions* drawOptions);
LIBRARY_API void moz2d_draw_target_fill_path_color (DrawTarget* drawTarget, Path* path, Float r, Float g, Float b, Float a, DrawOptions* drawOptions);
LIBRARY_API void moz2d_draw_target_fill_path_form (DrawTarget* drawTarget, Path* path, DrawOptions* drawOptions, unsigned char *aFormData, int32_t aFormWidth, int32_t aFormHeight, int32_t aStride);

/*
 * Stroke
 */
LIBRARY_API void moz2d_draw_target_stroke_rect(DrawTarget* drawTarget, Rect* rect, Pattern* pattern, StrokeOptions* strokeOptions, DrawOptions* drawOptions);
LIBRARY_API void moz2d_draw_target_stroke_rect_color(DrawTarget* drawTarget, Float x, Float y, Float width, Float height, Float r, Float g, Float b, Float a, StrokeOptions* strokeOptions, DrawOptions* drawOptions);
LIBRARY_API void moz2d_draw_target_stroke_path(DrawTarget* drawTarget, Path* path, Pattern* pattern, StrokeOptions* strokeOptions, DrawOptions* drawOptions);
LIBRARY_API void moz2d_draw_target_stroke_path_color(DrawTarget* drawTarget, Path* path, Float r, Float g, Float b, Float a, StrokeOptions* strokeOptions, DrawOptions* drawOptions);
LIBRARY_API void moz2d_draw_target_stroke_line(DrawTarget* drawTarget, Float fromX, Float fromY, Float toX, Float toY, Pattern* pattern, StrokeOptions* strokeOptions, DrawOptions* drawOptions);
LIBRARY_API void moz2d_draw_target_stroke_line_color(DrawTarget* drawTarget, Float fromX, Float fromY, Float toX, Float toY, Float r, Float g, Float b, Float a, StrokeOptions* strokeOptions, DrawOptions* drawOptions);
/*
 * Mask
 */
LIBRARY_API void moz2d_draw_target_mask_pattern(DrawTarget* drawTarget, Pattern* aSource, Pattern* aMask, DrawOptions* aOptions);
LIBRARY_API void moz2d_draw_target_mask_surface(DrawTarget* drawTarget, Pattern* aSource, SourceSurface *aMask, Float offsetX, Float offsetY, DrawOptions* aOptions);

/*
 * Filter
 */
LIBRARY_API void moz2d_draw_target_draw_filter(DrawTarget* drawTarget, FilterNode* aFilter, Rect* sourceRect, Float destX, Float destY, DrawOptions* drawOptions);

/*
 * Path
 */
LIBRARY_API PathBuilder* moz2d_draw_target_create_path_builder(DrawTarget* drawTarget, FillRule aFillRule);

/* --------------------------------------------------- */
/* ----------- T R A N S F O R M A T I O N ----------- */
/* --------------------------------------------------- */

/**
 * Return current transformation matrix as components array of 6 elements
 */
LIBRARY_API void moz2d_draw_target_transform_get(DrawTarget* drawTarget, Float* array);

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
LIBRARY_API void moz2d_draw_target_transform_set(DrawTarget* drawTarget, Float* rawMatrix);

/**
 * Concatenate current transformation with provided one
 */
LIBRARY_API void moz2d_draw_target_transform_concatenate(DrawTarget* drawTarget, Float a11, Float a12, Float a21, Float a22, Float a31, Float a32);

/**
 * Push current transform on a stack
 */
LIBRARY_API void moz2d_draw_target_transform_push(DrawTarget* drawTarget);

/**
 * Pops current transform from the stack
 */
LIBRARY_API void moz2d_draw_target_transform_pop(DrawTarget* drawTarget);

#ifdef __cplusplus
}
#endif

#endif /* DRAW_TARGET_SKIA_H_ */
