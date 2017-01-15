/*
 * gl.cpp
 *
 *  Created on: Aug 20, 2016
 *      Author: Aliaksei Syrel
 */

#include "gl.h"
#ifdef USE_SKIA_GPU
#include "gfxPlatform.h"
#endif

bool moz2d_gl_use_skia_gpu() {
#ifdef USE_SKIA_GPU
    return true;
#else
    return false;
#endif
}

bool moz2d_gl_allow_opengl_canvas() {
    return gfxPlatform::GetPlatform()->AllowOpenGLCanvas();
}

bool moz2d_gl_allow_accelerated_canvas() {
    return gfxPrefs::CanvasAzureAccelerated();
}

#ifdef USE_SKIA_GPU

DrawTarget* moz2d_gl_skia_draw_target_create (int32_t aWidth, int32_t aHeight, SurfaceFormat aFormat) {
    return Factory::CreateDrawTargetSkiaWithGrContext(moz2d_gl_get_skia_gl_glue()->GetGrContext(), IntSize(aWidth, aHeight), aFormat).take();
}

DrawTarget* moz2d_gl_skia_draw_target_create_from_native_context (GrContext* aGrContext, int32_t aWidth, int32_t aHeight, int32_t aRenderWidth, int32_t aRenderHeight, SurfaceFormat aFormat) {
    return Factory::CreateDrawTargetSkiaWithNativeGrContext(aGrContext, IntSize(aWidth, aHeight), IntSize(aRenderWidth, aRenderHeight), aFormat).take();
}

SkiaGLGlue* moz2d_gl_get_skia_gl_glue() {
    return gfxPlatform::GetPlatform()->GetSkiaGLGlue();
}

GLContext* moz2d_gl_get_gl_context() {
    SkiaGLGlue* aSkiaGlue = moz2d_gl_get_skia_gl_glue();
    if (!aSkiaGlue)
        return nullptr;
    return aSkiaGlue->GetGLContext();
}

GrContext* moz2d_gl_get_gr_context () {
    SkiaGLGlue* aSkiaGlue = moz2d_gl_get_skia_gl_glue();
    if (!aSkiaGlue)
        return nullptr;
    return aSkiaGlue->GetGrContext();
}

GrContext* moz2d_gl_create_gr_context_native (GrBackend aGrBackend) {
    const GrGLInterface* aGrInterface = GrGLCreateNativeInterface();
    GrContext* aGrContext = GrContext::Create( aGrBackend, (GrBackendContext) aGrInterface );

    if (!aGrContext) {
        if(aGrInterface) {
            delete aGrInterface;
        }
        //aGrInterface.release();
        return nullptr;
    }
    return aGrContext;
}

#endif