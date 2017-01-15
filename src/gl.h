/*
 * gl.h
 *
 *  Created on: Aug 20, 2016
 *      Author: Aliaksei Syrel
 */

#ifndef GL_H_
#define GL_H_

#include "exports.h"
#include "2d/2D.h"

#ifdef USE_SKIA_GPU
#  include "skia/include/gpu/GrContext.h"
#  include "skia/include/gpu/gl/GrGLInterface.h"
#  include "SkiaGLGlue.h"
#  include "GLContext.h"
using namespace mozilla::gl;
# endif

using namespace mozilla::gfx;

#ifdef __cplusplus
extern "C" {
#endif

LIBRARY_API bool moz2d_gl_use_skia_gpu();
LIBRARY_API bool moz2d_gl_allow_opengl_canvas();
LIBRARY_API bool moz2d_gl_allow_accelerated_canvas();

#ifdef USE_SKIA_GPU
LIBRARY_API DrawTarget* moz2d_gl_skia_draw_target_create (int32_t aWidth, int32_t aHeight, SurfaceFormat aFormat);
LIBRARY_API DrawTarget* moz2d_gl_skia_draw_target_create_from_native_context (GrContext* aGrContext, int32_t aWidth, int32_t aHeight, int32_t aRenderWidth, int32_t aRenderHeight, SurfaceFormat aFormat);

LIBRARY_API SkiaGLGlue* moz2d_gl_get_skia_gl_glue();
LIBRARY_API GLContext* moz2d_gl_get_gl_context();
LIBRARY_API GrContext* moz2d_gl_get_gr_context();
LIBRARY_API GrContext* moz2d_gl_create_gr_context_native (GrBackend aGrBackend);
#endif

#ifdef __cplusplus
}
#endif

#endif /* GL_H_ */
