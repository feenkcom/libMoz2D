//
// Created by Aliaksei Syrel on 10.01.17.
//

#ifndef MOZ2D_TEST_ENUMS_H
#define MOZ2D_TEST_ENUMS_H

#include "exports.h"
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

#define CALL_FUNCTION_NAME(enumName) CALL_FUNCTION_NAME1(enumName)
#define CALL_FUNCTION_NAME1(enumName) CALL_FUNCTION_NAME2(enumName)
#define CALL_FUNCTION_NAME2(enumName) call_function_##enumName

#define STRINGIZE(arg)  STRINGIZE1(arg)
#define STRINGIZE1(arg) STRINGIZE2(arg)
#define STRINGIZE2(arg) #arg

#define CONCATENATE(arg1, arg2)   CONCATENATE1(arg1, arg2)
#define CONCATENATE1(arg1, arg2)  CONCATENATE2(arg1, arg2)
#define CONCATENATE2(arg1, arg2)  arg1##arg2

#define FOR_EACH_1(what, x, y, ...)\
  what(x, y)
#define FOR_EACH_2(what, x, y, ...)\
  what(x, y)\
  FOR_EACH_1(what, x, __VA_ARGS__)
#define FOR_EACH_3(what, x, y, ...)\
  what(x, y)\
  FOR_EACH_2(what, x, __VA_ARGS__)
//#define FOR_EACH_4(what, x, y, ...)\
//  what(x, y)\
//  FOR_EACH_3(what, x, __VA_ARGS__)
//#define FOR_EACH_5(what, x, y, ...)\
//  what(x, y)\
// FOR_EACH_4(what, x, __VA_ARGS__)
//#define FOR_EACH_6(what, x, y, ...)\
//  what(x, y)\
//  FOR_EACH_5(what, x, __VA_ARGS__)
//#define FOR_EACH_7(what, x, y, ...)\
//  what(x, y)\
//  FOR_EACH_6(what, x, __VA_ARGS__)
//#define FOR_EACH_8(what, x, y, ...)\
//  what(x, y)\
//  FOR_EACH_7(what, x, __VA_ARGS__)
//#define FOR_EACH_9(what, x, y, ...)\
//  what(x, y)\
//  FOR_EACH_8(what, x, __VA_ARGS__)
//#define FOR_EACH_10(what, x, y, ...)\
//  what(x, y)\
//  FOR_EACH_9(what, x, __VA_ARGS__)
//#define FOR_EACH_11(what, x, y, ...)\
//  what(x, y)\
//  FOR_EACH_10(what, x, __VA_ARGS__)
//#define FOR_EACH_12(what, x, y, ...)\
//  what(x, y)\
//  FOR_EACH_11(what, x, __VA_ARGS__)
//#define FOR_EACH_13(what, x, y, ...)\
//  what(x, y)\
//  FOR_EACH_12(what, x, __VA_ARGS__)
//#define FOR_EACH_14(what, x, y, ...)\
//  what(x, y)\
//  FOR_EACH_13(what, x, __VA_ARGS__)
//#define FOR_EACH_15(what, x, y, ...)\
//  what(x, y)\
//  FOR_EACH_14(what, x, __VA_ARGS__)
//#define FOR_EACH_16(what, x, y, ...)\
//  what(x, y)\
//  FOR_EACH_15(what, x, __VA_ARGS__)
//#define FOR_EACH_17(what, x, y, ...)\
//  what(x, y)\
//  FOR_EACH_16(what, x, __VA_ARGS__)
//#define FOR_EACH_18(what, x, y, ...)\
//  what(x, y)\
//  FOR_EACH_17(what, x, __VA_ARGS__)
//#define FOR_EACH_19(what, x, y, ...)\
//  what(x, y)\
//  FOR_EACH_18(what, x, __VA_ARGS__)
//#define FOR_EACH_20(what, x, y, ...)\
//  what(x, y)\
//  FOR_EACH_19(what, x, __VA_ARGS__)
//#define FOR_EACH_21(what, x, y, ...)\
//  what(x, y)\
//  FOR_EACH_20(what, x, __VA_ARGS__)
//#define FOR_EACH_22(what, x, y, ...)\
//  what(x, y)\
//  FOR_EACH_21(what, x, __VA_ARGS__)
//#define FOR_EACH_23(what, x, y, ...)\
//  what(x, y)\
//  FOR_EACH_22(what, x, __VA_ARGS__)
//#define FOR_EACH_24(what, x, y, ...)\
//  what(x, y)\
//  FOR_EACH_23(what, x, __VA_ARGS__)
//#define FOR_EACH_25(what, x, y, ...)\
//  what(x, y)\
//  FOR_EACH_24(what, x, __VA_ARGS__)
//#define FOR_EACH_26(what, x, y, ...)\
//  what(x, y)\
//  FOR_EACH_25(what, x, __VA_ARGS__)
//#define FOR_EACH_27(what, x, y, ...)\
//  what(x, y)\
//  FOR_EACH_26(what, x, __VA_ARGS__)
//#define FOR_EACH_28(what, x, y, ...)\
//  what(x, y)\
//  FOR_EACH_27(what, x, __VA_ARGS__)
//#define FOR_EACH_29(what, x, y, ...)\
//  what(x, y)\
//  FOR_EACH_28(what, x, __VA_ARGS__)
//#define FOR_EACH_30(what, x, y, ...)\
//  what(x, y)\
//  FOR_EACH_29(what, x, __VA_ARGS__)

/*
* Count the number of arguments passed to FOR_EACH, very carefully
* tiptoeing around an MSVC bug where it improperly expands __VA_ARGS__ as a
* single token in argument lists.  See these URLs for details:
*
*   http://connect.microsoft.com/VisualStudio/feedback/details/380090/variadic-macro-replacement
*   http://cplusplus.co.il/2010/07/17/variadic-macro-to-count-number-of-arguments/#comment-644
*/
#define COUNT_FOR_EACH_ARGS_IMPL2( _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _20, _21, _22, _23, _24, _25, _26, _27, _28, _29, _30, count, ...) \
   count
#define COUNT_FOR_EACH_ARGS_IMPL(args) \
   COUNT_FOR_EACH_ARGS_IMPL2 args
#define COUNT_FOR_EACH_ARGS(...) \
   COUNT_FOR_EACH_ARGS_IMPL((__VA_ARGS__, 30, 29, 28, 27, 26, 25, 24, 23, 22, 21, 20, 19, 18, 17, 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0))
/* Pick the right helper macro to invoke. */
#define FOR_EACH_CHOOSE_HELPER2(count) FOR_EACH_##count
#define FOR_EACH_CHOOSE_HELPER1(count) FOR_EACH_CHOOSE_HELPER2(count)
#define FOR_EACH_CHOOSE_HELPER(count) FOR_EACH_CHOOSE_HELPER1(count)
/* The actual macro. */
#define FOR_EACH_GLUE(x, y) x y
#define FOR_EACH(what, x, ...) FOR_EACH_GLUE(FOR_EACH_CHOOSE_HELPER(COUNT_FOR_EACH_ARGS( __VA_ARGS__)), (what, x, __VA_ARGS__))


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

#define FOO(x,y) const int y = x;\


FOR_EACH(FOO, 0, b, c, d)

//TEST(SurfaceType,
//     DATA,
//     D2D1_BITMAP,
//     D2D1_DRAWTARGET,
//     CAIRO,
//     CAIRO_IMAGE,
//     COREGRAPHICS_IMAGE,
//     COREGRAPHICS_CGCONTEXT,
//     SKIA,
//     DUAL_DT,
//     D2D1_1_IMAGE,
//     RECORDING,
//     TILED)
//
//TEST(SurfaceFormat,
//     B8G8R8A8,
//     B8G8R8X8,
//     R8G8B8A8,
//     R8G8B8X8,
//     A8R8G8B8,
//     X8R8G8B8,
//     R8G8B8,
//     B8G8R8,
//     R5G6B5_UINT16,
//     A8,
//     YUV,
//     NV12,
//     YUV422,
//     HSV,
//     Lab,
//     Depth,
//     UNKNOWN,
//     A8R8G8B8_UINT32,
//     X8R8G8B8_UINT32)
//
//TEST(FilterType,
//     BLEND,
//     TRANSFORM,
//     MORPHOLOGY,
//     COLOR_MATRIX,
//     FLOOD,
//     TILE,
//     TABLE_TRANSFER,
//     DISCRETE_TRANSFER,
//     LINEAR_TRANSFER,
//     GAMMA_TRANSFER,
//     CONVOLVE_MATRIX,
//     DISPLACEMENT_MAP,
//     TURBULENCE,
//     ARITHMETIC_COMBINE,
//     COMPOSITE,
//     DIRECTIONAL_BLUR,
//     GAUSSIAN_BLUR,
//     POINT_DIFFUSE,
//     POINT_SPECULAR,
//     SPOT_DIFFUSE,
//     SPOT_SPECULAR,
//     DISTANT_DIFFUSE,
//     DISTANT_SPECULAR,
//     CROP,
//     PREMULTIPLY,
//     UNPREMULTIPLY)
//
//TEST(DrawTargetType,
//     SOFTWARE_RASTER,
//     HARDWARE_RASTER,
//     VECTOR)
//
//TEST(BackendType,
//     NONE,
//     DIRECT2D,
//     CAIRO,
//     SKIA,
//     RECORDING,
//     DIRECT2D1_1,
//     BACKEND_LAST)
//
//TEST(FontType,
//     DWRITE,
//     GDI,
//     MAC,
//     SKIA,
//     CAIRO,
//     COREGRAPHICS,
//     FONTCONFIG)
//
//TEST(CompositionOp,
//     OP_OVER,
//     OP_ADD,
//     OP_ATOP,
//     OP_OUT,
//     OP_IN,
//     OP_SOURCE,
//     OP_DEST_IN,
//     OP_DEST_OUT,
//     OP_DEST_OVER,
//     OP_DEST_ATOP,
//     OP_XOR,
//     OP_MULTIPLY,
//     OP_SCREEN,
//     OP_OVERLAY,
//     OP_DARKEN,
//     OP_LIGHTEN,
//     OP_COLOR_DODGE,
//     OP_COLOR_BURN,
//     OP_HARD_LIGHT,
//     OP_SOFT_LIGHT,
//     OP_DIFFERENCE,
//     OP_EXCLUSION,
//     OP_HUE,
//     OP_SATURATION,
//     OP_COLOR,
//     OP_LUMINOSITY,
//     OP_COUNT)

#ifdef __cplusplus
}
#endif

#endif //MOZ2D_TEST_ENUMS_H
