//
// Created by Aliaksei Syrel on 10.01.17.
//

#ifndef MOZ2D_TEST_ENUMS_H
#define MOZ2D_TEST_ENUMS_H

#include "exports.h"
#include "macros.h"
#include "2d/2D.h"

#define PREFIX(x) PREFIX1(x)
#define PREFIX1(x) PREFIX2(x)
#define PREFIX2(x) moz2d_test_##x

#define FUNC_EACH_IMP(enumName, enumValueName) FUNC_EACH_IMP1(enumName, enumValueName)
#define FUNC_EACH_IMP1(enumName, enumValueName) FUNC_EACH_IMP2(enumName, enumValueName)
#define FUNC_EACH_IMP2(enumName, enumValueName) enumName##_##enumValueName

#define FUNC_EACH(enumName, enumValueName) PREFIX(FUNC_EACH_IMP(enumName,enumValueName))
#define FUNC(enumName) PREFIX(enumName)

#define MAP_NAME(enumName) MAP_NAME1(enumName)
#define MAP_NAME1(enumName) MAP_NAME2(enumName)
#define MAP_NAME2(enumName) Map_##enumName

#define MAP_STRUCT_NAME(enumName) MAP_STRUCT_NAME1(enumName)
#define MAP_STRUCT_NAME1(enumName) MAP_STRUCT_NAME2(enumName)
#define MAP_STRUCT_NAME2(enumName) Map##enumName

#define TEST_FOR_EACH(enumName, enumValueName) LIBRARY_API enumName FUNC_EACH(enumName, enumValueName) () { return enumName::enumValueName; }

#define MAP_STRUCT(enumName)\
struct MAP_STRUCT_NAME(enumName) {\
    const char *name;\
    enumName (*func)(void);\
};

#define CALL_MAP(enumName)\
LIBRARY_API bool FUNC(enumName) (const char *name, enumName *result) {\
    unsigned long i;\
    for (i = 0; i < (sizeof(MAP_NAME(enumName)) / sizeof(MAP_NAME(enumName)[0])); i++) {\
        if (!strcmp(MAP_NAME(enumName)[i].name, name) && MAP_NAME(enumName)[i].func) {\
            *result = MAP_NAME(enumName)[i].func();\
            return true;\
        }\
    }\
    return false;\
}

#define MAP_EACH(enumName, enumValueName) { STRINGIZE(enumValueName), FUNC_EACH(enumName, enumValueName) },

#define MAP(enumName, ...) const static MAP_STRUCT_NAME(enumName) MAP_NAME(enumName) [] = { FOR_EACH( MAP_EACH, enumName, __VA_ARGS__) };

#define TEST(enumName, ...) FOR_EACH(TEST_FOR_EACH, enumName, __VA_ARGS__) MAP_STRUCT(enumName) MAP(enumName, __VA_ARGS__) CALL_MAP(enumName)

#ifdef __cplusplus
extern "C" {
#endif

using namespace mozilla::gfx;

TEST(SurfaceType,
     DATA,
     D2D1_BITMAP,
     D2D1_DRAWTARGET,
     CAIRO,
     CAIRO_IMAGE,
     COREGRAPHICS_IMAGE,
     COREGRAPHICS_CGCONTEXT,
     SKIA,
     DUAL_DT,
     D2D1_1_IMAGE,
     RECORDING,
     TILED)

TEST(SurfaceFormat,
     B8G8R8A8,
     B8G8R8X8,
     R8G8B8A8,
     R8G8B8X8,
     A8R8G8B8,
     X8R8G8B8,
     R8G8B8,
     B8G8R8,
     R5G6B5_UINT16,
     A8,
     YUV,
     NV12,
     YUV422,
     HSV,
     Lab,
     Depth,
     UNKNOWN,
     A8R8G8B8_UINT32,
     X8R8G8B8_UINT32)

TEST(FilterType,
     BLEND,
     TRANSFORM,
     MORPHOLOGY,
     COLOR_MATRIX,
     FLOOD,
     TILE,
     TABLE_TRANSFER,
     DISCRETE_TRANSFER,
     LINEAR_TRANSFER,
     GAMMA_TRANSFER,
     CONVOLVE_MATRIX,
     DISPLACEMENT_MAP,
     TURBULENCE,
     ARITHMETIC_COMBINE,
     COMPOSITE,
     DIRECTIONAL_BLUR,
     GAUSSIAN_BLUR,
     POINT_DIFFUSE,
     POINT_SPECULAR,
     SPOT_DIFFUSE,
     SPOT_SPECULAR,
     DISTANT_DIFFUSE,
     DISTANT_SPECULAR,
     CROP,
     PREMULTIPLY,
     UNPREMULTIPLY)

TEST(DrawTargetType,
     SOFTWARE_RASTER,
     HARDWARE_RASTER,
     VECTOR)

TEST(BackendType,
     NONE,
     DIRECT2D,
     CAIRO,
     SKIA,
     RECORDING,
     DIRECT2D1_1,
     BACKEND_LAST)

TEST(FontType,
     DWRITE,
     GDI,
     MAC,
     SKIA,
     CAIRO,
     COREGRAPHICS,
     FONTCONFIG)

TEST(CompositionOp,
     OP_OVER,
     OP_ADD,
     OP_ATOP,
     OP_OUT,
     OP_IN,
     OP_SOURCE,
     OP_DEST_IN,
     OP_DEST_OUT,
     OP_DEST_OVER,
     OP_DEST_ATOP,
     OP_XOR,
     OP_MULTIPLY,
     OP_SCREEN,
     OP_OVERLAY,
     OP_DARKEN,
     OP_LIGHTEN,
     OP_COLOR_DODGE,
     OP_COLOR_BURN,
     OP_HARD_LIGHT,
     OP_SOFT_LIGHT,
     OP_DIFFERENCE,
     OP_EXCLUSION,
     OP_HUE,
     OP_SATURATION,
     OP_COLOR,
     OP_LUMINOSITY,
     OP_CLEAR,
     OP_COUNT)

TEST(ExtendMode,
     CLAMP,
     REPEAT,
     REPEAT_X,
     REPEAT_Y,
     REFLECT)

TEST(FillRule,
     FILL_WINDING,
     FILL_EVEN_ODD)

TEST(AntialiasMode,
     NONE,
     GRAY,
     SUBPIXEL,
     DEFAULT)

TEST(SamplingFilter,
     GOOD,
     LINEAR,
     POINT,
     SENTINEL)

TEST(PatternType,
     COLOR,
     SURFACE,
     LINEAR_GRADIENT,
     RADIAL_GRADIENT)

TEST(JoinStyle,
     BEVEL,
     ROUND,
     MITER,
     MITER_OR_BEVEL)

TEST(CapStyle,
     BUTT,
     ROUND,
     SQUARE)

TEST(NativeSurfaceType,
     D3D10_TEXTURE,
     CAIRO_CONTEXT,
     CGCONTEXT,
     CGCONTEXT_ACCELERATED,
     OPENGL_TEXTURE
)

#ifdef __cplusplus
}
#endif

#endif //MOZ2D_TEST_ENUMS_H
