//
// Created by Aliaksei Syrel on 13.01.17.
//

#ifndef MOZ2D_MACROS_H
#define MOZ2D_MACROS_H

#define GLUE(x, y) x y

#define CONCATENATE(arg1, arg2)   CONCATENATE1(arg1, arg2)
#define CONCATENATE1(arg1, arg2)  CONCATENATE2(arg1, arg2)
#define CONCATENATE2(arg1, arg2)  arg1##arg2

#define STRINGIZE(arg)  STRINGIZE1(arg)
#define STRINGIZE1(arg) STRINGIZE2(arg)
#define STRINGIZE2(arg) #arg

/*
* Count the number of arguments passed to a macro function, very carefully
* tiptoeing around an MSVC bug where it improperly expands __VA_ARGS__ as a
* single token in argument lists.  See these URLs for details:
*
*   http://connect.microsoft.com/VisualStudio/feedback/details/380090/variadic-macro-replacement
*   http://cplusplus.co.il/2010/07/17/variadic-macro-to-count-number-of-arguments/#comment-644
*/
#define COUNT_ARGS_IMPL2( _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _20, _21, _22, _23, _24, _25, _26, _27, _28, _29, _30, count, ...) count
#define COUNT_ARGS_IMPL(args) COUNT_ARGS_IMPL2 args
#define COUNT_ARGS(...) COUNT_ARGS_IMPL((__VA_ARGS__, 30, 29, 28, 27, 26, 25, 24, 23, 22, 21, 20, 19, 18, 17, 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0))



/* For each do */

#define FOR_EACH_1(what, x, y, ...)\
  what(x, y)
#define FOR_EACH_2(what, x, y, ...)\
  what(x, y)\
  GLUE(FOR_EACH_1, (what, x, __VA_ARGS__))
#define FOR_EACH_3(what, x, y, ...)\
  what(x, y)\
  GLUE(FOR_EACH_2, (what, x, __VA_ARGS__))
#define FOR_EACH_4(what, x, y, ...)\
  what(x, y)\
  GLUE(FOR_EACH_3, (what, x, __VA_ARGS__))
#define FOR_EACH_5(what, x, y, ...)\
  what(x, y)\
  GLUE(FOR_EACH_4, (what, x, __VA_ARGS__))
#define FOR_EACH_6(what, x, y, ...)\
  what(x, y)\
  GLUE(FOR_EACH_5, (what, x, __VA_ARGS__))
#define FOR_EACH_7(what, x, y, ...)\
  what(x, y)\
  GLUE(FOR_EACH_6, (what, x, __VA_ARGS__))
#define FOR_EACH_8(what, x, y, ...)\
  what(x, y)\
  GLUE(FOR_EACH_7, (what, x, __VA_ARGS__))
#define FOR_EACH_9(what, x, y, ...)\
  what(x, y)\
  GLUE(FOR_EACH_8, (what, x, __VA_ARGS__))
#define FOR_EACH_10(what, x, y, ...)\
  what(x, y)\
  GLUE(FOR_EACH_9, (what, x, __VA_ARGS__))
#define FOR_EACH_11(what, x, y, ...)\
  what(x, y)\
  GLUE(FOR_EACH_10, (what, x, __VA_ARGS__))
#define FOR_EACH_12(what, x, y, ...)\
  what(x, y)\
  GLUE(FOR_EACH_11, (what, x, __VA_ARGS__))
#define FOR_EACH_13(what, x, y, ...)\
  what(x, y)\
  GLUE(FOR_EACH_12, (what, x, __VA_ARGS__))
#define FOR_EACH_14(what, x, y, ...)\
  what(x, y)\
  GLUE(FOR_EACH_13, (what, x, __VA_ARGS__))
#define FOR_EACH_15(what, x, y, ...)\
  what(x, y)\
  GLUE(FOR_EACH_14, (what, x, __VA_ARGS__))
#define FOR_EACH_16(what, x, y, ...)\
  what(x, y)\
  GLUE(FOR_EACH_15, (what, x, __VA_ARGS__))
#define FOR_EACH_17(what, x, y, ...)\
  what(x, y)\
  GLUE(FOR_EACH_16, (what, x, __VA_ARGS__))
#define FOR_EACH_18(what, x, y, ...)\
  what(x, y)\
  GLUE(FOR_EACH_17, (what, x, __VA_ARGS__))
#define FOR_EACH_19(what, x, y, ...)\
  what(x, y)\
  GLUE(FOR_EACH_18, (what, x, __VA_ARGS__))
#define FOR_EACH_20(what, x, y, ...)\
  what(x, y)\
  GLUE(FOR_EACH_19, (what, x, __VA_ARGS__))
#define FOR_EACH_21(what, x, y, ...)\
  what(x, y)\
  GLUE(FOR_EACH_20, (what, x, __VA_ARGS__))
#define FOR_EACH_22(what, x, y, ...)\
  what(x, y)\
  GLUE(FOR_EACH_21, (what, x, __VA_ARGS__))
#define FOR_EACH_23(what, x, y, ...)\
  what(x, y)\
  GLUE(FOR_EACH_22, (what, x, __VA_ARGS__))
#define FOR_EACH_24(what, x, y, ...)\
  what(x, y)\
  GLUE(FOR_EACH_23, (what, x, __VA_ARGS__))
#define FOR_EACH_25(what, x, y, ...)\
  what(x, y)\
  GLUE(FOR_EACH_24, (what, x, __VA_ARGS__))
#define FOR_EACH_26(what, x, y, ...)\
  what(x, y)\
  GLUE(FOR_EACH_25, (what, x, __VA_ARGS__))
#define FOR_EACH_27(what, x, y, ...)\
  what(x, y)\
  GLUE(FOR_EACH_26, (what, x, __VA_ARGS__))
#define FOR_EACH_28(what, x, y, ...)\
  what(x, y)\
  GLUE(FOR_EACH_27, (what, x, __VA_ARGS__))
#define FOR_EACH_29(what, x, y, ...)\
  what(x, y)\
  GLUE(FOR_EACH_28, (what, x, __VA_ARGS__))
#define FOR_EACH_30(what, x, y, ...)\
  what(x, y)\
  GLUE(FOR_EACH_29, (what, x, __VA_ARGS__))

/* Pick the right helper macro to invoke. */
#define FOR_EACH_CHOOSE_HELPER2(count) FOR_EACH_##count
#define FOR_EACH_CHOOSE_HELPER1(count) FOR_EACH_CHOOSE_HELPER2(count)
#define FOR_EACH_CHOOSE_HELPER(count) FOR_EACH_CHOOSE_HELPER1(count)
/* The actual macro. */
#define FOR_EACH(what, x, ...) GLUE(FOR_EACH_CHOOSE_HELPER(COUNT_ARGS( __VA_ARGS__)), (what, x, __VA_ARGS__))


/* Pairs do */

#define PAIRS_DO_1(what, z, x, y, ...)\
  fail
#define PAIRS_DO_2(what, z, x, y, ...)\
  what(z, x, y)
#define PAIRS_DO_3(what, z, x, y, x2, y2, ...)\
  fail
#define PAIRS_DO_4(what, z, x, y, x2, y2, ...)\
  what(z, x, y)\
  GLUE(PAIRS_DO_2, (what, z, x2, y2, __VA_ARGS__))
#define PAIRS_DO_5(what, z, x, y, x2, y2, ...)\
  fail
#define PAIRS_DO_6(what, z, x, y, x2, y2, ...)\
  what(z, x, y)\
  GLUE(PAIRS_DO_4, (what, z, x2, y2, __VA_ARGS__))
#define PAIRS_DO_7(what, z, x, y, x2, y2, ...)\
  fail
#define PAIRS_DO_8(what, z, x, y, x2, y2, ...)\
  what(z, x, y)\
  GLUE(PAIRS_DO_6, (what, z, x2, y2, __VA_ARGS__))
#define PAIRS_DO_9(what, z, x, y, x2, y2, ...)\
  fail
#define PAIRS_DO_10(what, z, x, y, x2, y2, ...)\
  what(z, x, y)\
  GLUE(PAIRS_DO_8, (what, z, x2, y2, __VA_ARGS__))
#define PAIRS_DO_11(what, z, x, y, x2, y2, ...)\
  fail
#define PAIRS_DO_12(what, z, x, y, x2, y2, ...)\
  what(z, x, y)\
  GLUE(PAIRS_DO_10, (what, z, x2, y2, __VA_ARGS__))
#define PAIRS_DO_13(what, z, x, y, x2, y2, ...)\
  fail
#define PAIRS_DO_14(what, z, x, y, x2, y2, ...)\
  what(z, x, y)\
  GLUE(PAIRS_DO_12, (what, z, x2, y2, __VA_ARGS__))
#define PAIRS_DO_15(what, z, x, y, x2, y2, ...)\
  fail
#define PAIRS_DO_16(what, z, x, y, x2, y2, ...)\
  what(z, x, y)\
  GLUE(PAIRS_DO_14, (what, z, x2, y2, __VA_ARGS__))
#define PAIRS_DO_17(what, z, x, y, x2, y2, ...)\
  fail
#define PAIRS_DO_18(what, z, x, y, x2, y2, ...)\
  what(z, x, y)\
  GLUE(PAIRS_DO_16, (what, z, x2, y2, __VA_ARGS__))
#define PAIRS_DO_19(what, z, x, y, x2, y2, ...)\
  fail
#define PAIRS_DO_20(what, z, x, y, x2, y2, ...)\
  what(z, x, y)\
  GLUE(PAIRS_DO_18, (what, z, x2, y2, __VA_ARGS__))
#define PAIRS_DO_21(what, z, x, y, x2, y2, ...)\
  fail
#define PAIRS_DO_22(what, z, x, y, x2, y2, ...)\
  what(z, x, y)\
  GLUE(PAIRS_DO_20, (what, z, x2, y2, __VA_ARGS__))
#define PAIRS_DO_23(what, z, x, y, x2, y2, ...)\
  fail
#define PAIRS_DO_24(what, z, x, y, x2, y2, ...)\
  what(z, x, y)\
  GLUE(PAIRS_DO_22, (what, z, x2, y2, __VA_ARGS__))
#define PAIRS_DO_25(what, z, x, y, x2, y2, ...)\
  fail
#define PAIRS_DO_26(what, z, x, y, x2, y2, ...)\
  what(z, x, y)\
  GLUE(PAIRS_DO_24, (what, z, x2, y2, __VA_ARGS__))
#define PAIRS_DO_27(what, z, x, y, x2, y2, ...)\
  fail
#define PAIRS_DO_28(what, z, x, y, x2, y2, ...)\
  what(z, x, y)\
  GLUE(PAIRS_DO_26, (what, z, x2, y2, __VA_ARGS__))
#define PAIRS_DO_29(what, z, x, y, x2, y2, ...)\
  fail
#define PAIRS_DO_30(what, z, x, y, x2, y2, ...)\
  what(z, x, y)\
  GLUE(PAIRS_DO_28, (what, z, x2, y2, __VA_ARGS__))

#define PAIRS_DO_CHOOSE_2(count) PAIRS_DO_##count
#define PAIRS_DO_CHOOSE_1(count) PAIRS_DO_CHOOSE_2(count)
#define PAIRS_DO_CHOOSE(count) PAIRS_DO_CHOOSE_1(count)

#define PAIRS_DO(what, z, ...)  GLUE(PAIRS_DO_CHOOSE(COUNT_ARGS( __VA_ARGS__)), (what, z, __VA_ARGS__))

#endif //MOZ2D_MACROS_H