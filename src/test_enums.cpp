//
// Created by Aliaksei Syrel on 10.01.17.
//

#include "test_enums.h"
//#include "exports.h"
//#include "2d/2D.h"
//
//#define PREFIX(x) PREFIX1(x)
//#define PREFIX1(x) PREFIX2(x)
//#define PREFIX2(x) moz2d_test_##x
//
//#define FUNC_EACH_IMP(enumName, enumValueName) FUNC_EACH_IMP1(enumName, enumValueName)
//#define FUNC_EACH_IMP1(enumName, enumValueName) FUNC_EACH_IMP2(enumName, enumValueName)
//#define FUNC_EACH_IMP2(enumName, enumValueName) enumName##_##enumValueName
//
//#define FUNC_EACH(enumName, enumValueName) PREFIX(FUNC_EACH_IMP(enumName,enumValueName))
//#define FUNC(enumName) PREFIX(enumName)
//
//#define MAP_NAME(enumName) MAP_NAME1(enumName)
//#define MAP_NAME1(enumName) MAP_NAME2(enumName)
//#define MAP_NAME2(enumName) Map_##enumName
//
//#define MAP_STRUCT_NAME(enumName) MAP_STRUCT_NAME1(enumName)
//#define MAP_STRUCT_NAME1(enumName) MAP_STRUCT_NAME2(enumName)
//#define MAP_STRUCT_NAME2(enumName) Map##enumName
//
//#define CALL_FUNCTION_NAME(enumName) CALL_FUNCTION_NAME1(enumName)
//#define CALL_FUNCTION_NAME1(enumName) CALL_FUNCTION_NAME2(enumName)
//#define CALL_FUNCTION_NAME2(enumName) call_function_##enumName
//
//
//#define FOR_EACH_1(what, x, y, ...)\
//    what(x, y)
//#define FOR_EACH_2(what, x, y, ...)\
//  what(x, y)\
//  FOR_EACH_1(what, x, __VA_ARGS__)
//#define FOR_EACH_3(what, x, y, ...)\
//  what(x, y)\
//  FOR_EACH_2(what, x, __VA_ARGS__)
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
//
///*
//* Count the number of arguments passed to FOR_EACH, very carefully
//* tiptoeing around an MSVC bug where it improperly expands __VA_ARGS__ as a
//* single token in argument lists.  See these URLs for details:
//*
//*   http://connect.microsoft.com/VisualStudio/feedback/details/380090/variadic-macro-replacement
//*   http://cplusplus.co.il/2010/07/17/variadic-macro-to-count-number-of-arguments/#comment-644
//*/
//#define COUNT_FOR_EACH_ARGS_IMPL2(_1, _2, count, ...) \
//   count
//#define COUNT_FOR_EACH_ARGS_IMPL(args) \
//   COUNT_FOR_EACH_ARGS_IMPL2 args
//#define COUNT_FOR_EACH_ARGS(...) \
//   COUNT_FOR_EACH_ARGS_IMPL((__VA_ARGS__, 2, 1, 0))
///* Pick the right helper macro to invoke. */
//#define FOR_EACH_CHOOSE_HELPER2(count) FOR_EACH_##count
//#define FOR_EACH_CHOOSE_HELPER1(count) FOR_EACH_CHOOSE_HELPER2(count)
//#define FOR_EACH_CHOOSE_HELPER(count) FOR_EACH_CHOOSE_HELPER1(count)
///* The actual macro. */
//#define FOR_EACH_GLUE(x, y) x y
//#define FOR_EACH(what, x, ...) FOR_EACH_GLUE(FOR_EACH_CHOOSE_HELPER(COUNT_FOR_EACH_ARGS(__VA_ARGS__)), (what, x, __VA_ARGS__))
//
//
//#define TEST_FOR_EACH(enumName, enumValueName) LIBRARY_API enumName FUNC_EACH(enumName, enumValueName) () { return enumName::enumValueName; }
//
//using namespace mozilla::gfx;
//
//FOR_EACH(TEST_FOR_EACH, SurfaceType, DATA);