/*
 * gl.cpp
 *
 *  Created on: Aug 20, 2016
 *      Author: Aliaksei Syrel
 */

#include "gl.h"

GLContext* moz2d_gl_create_wrapping_existing(void* aContext, void* aSurface) {
	return GLContextProvider::CreateWrappingExisting(aContext, aSurface).take();
}

GLContext* moz2d_gl_get_global_context() {
	return GLContextProvider::GetGlobalContext();
}

SkiaGLGlue* moz2d_gl_create_skia_gl_glue_for(GLContext* aContext) {
	return new SkiaGLGlue(aContext);
}

DrawTarget* moz2d_gl_create_skia_gl_draw_target(SkiaGLGlue* skiaGlue, int32_t width, int32_t height, SurfaceFormat aSurfaceFormat) {
	return Factory::CreateDrawTargetSkiaWithGrContext(skiaGlue->GetGrContext(), IntSize(width, height), aSurfaceFormat).take();
}

GLScreenBuffer* moz2d_gl_context_get_screen(GLContext* aContext) {
	return aContext->Screen();
}

GLuint moz2d_gl_screen_buffer_get_read_fb(GLScreenBuffer* screenBuffer) {
	return screenBuffer->GetReadFB();
}
