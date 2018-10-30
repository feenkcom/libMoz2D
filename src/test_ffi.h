//
// Created by Aliaksei Syrel on 13.01.17.
//

#ifndef MOZ2D_TEST_FFI_H
#define MOZ2D_TEST_FFI_H

#include "exports.h"
#include "macros.h"
#include <stdint.h>
#include <limits.h>
#include <float.h>

#define FFI_FUNCTION_NAME(prefix, body) CONCATENATE(prefix, body)

#define FFI_HEADER_FOR_EACH(prefix, type) LIBRARY_API type FFI_FUNCTION_NAME(prefix, type) (type aValue);
#define FFI_HEADER(prefix, ...) FOR_EACH(FFI_HEADER_FOR_EACH, prefix, __VA_ARGS__)

#define FFI_HEADER_COMBO_FOR_EACH(prefix, type) LIBRARY_API bool FFI_FUNCTION_NAME(prefix, type) (type aValue1, type aValue2, type aValue3, type aValue4, type aValue5);
#define FFI_HEADER_COMBO(prefix, ...) FOR_EACH(FFI_HEADER_COMBO_FOR_EACH, prefix, __VA_ARGS__)

#define FFI_HEADER_FOR_EACH_ASSOCIATION(prefix, type, value) LIBRARY_API type FFI_FUNCTION_NAME(prefix, type) ();
#define FFI_HEADER_ASSOCIATION(prefix, ...) PAIRS_DO(FFI_HEADER_FOR_EACH_ASSOCIATION, prefix, __VA_ARGS__)

#define FFI_CALLBACK_HEADER_FOR_EACH(prefix, type) LIBRARY_API type FFI_FUNCTION_NAME(prefix, type) (type(*function)(type), type value);
#define FFI_CALLBACK_HEADER(prefix, ...) FOR_EACH(FFI_CALLBACK_HEADER_FOR_EACH, prefix, __VA_ARGS__)

#define FFI_TYPES_FIXED int8_t, uint8_t, int16_t, uint16_t, int32_t, uint32_t, int64_t, uint64_t, intptr_t, uintptr_t
#define FFI_TYPES_FIXED_SIGNED_MIN int8_t, INT8_MIN, int16_t, INT16_MIN, int32_t, INT32_MIN, int64_t, INT64_MIN, intptr_t, INTPTR_MIN
#define FFI_TYPES_FIXED_SIGNED_MAX int8_t, INT8_MAX, int16_t, INT16_MAX, int32_t, INT32_MAX, int64_t, INT64_MAX, intptr_t, INTPTR_MAX
#define FFI_TYPES_FIXED_UNSIGNED_MAX uint8_t, UINT8_MAX, uint16_t, UINT16_MAX, uint32_t, UINT32_MAX, uint64_t, UINT64_MAX, uintptr_t, UINTPTR_MAX

typedef unsigned char uchar;
typedef unsigned short ushort;
typedef unsigned int uint;
typedef unsigned long ulong;
typedef long double ldouble;

#define FFI_TYPES char, uchar, short, ushort, int, uint, long, ulong, float, double, ldouble
#define FFI_TYPES_SIGNED_MIN char, CHAR_MIN, short, SHRT_MIN, int, INT_MIN, long, LONG_MIN
#define FFI_TYPES_SIGNED_MAX char, CHAR_MAX, short, SHRT_MAX, int, INT_MAX, long, LONG_MAX
#define FFI_TYPES_UNSIGNED_MAX uchar, UCHAR_MAX, ushort, USHRT_MAX, uint, UINT_MAX, ulong, ULONG_MAX
#define FFI_TYPES_FLOATING_MIN float, FLT_MIN, double, DBL_MIN, ldouble, LDBL_MIN
#define FFI_TYPES_FLOATING_MAX float, FLT_MAX, double, DBL_MAX, ldouble, LDBL_MAX

#ifdef __cplusplus
extern "C" {
#endif

FFI_HEADER(moz2d_test_ffi_fixed_, FFI_TYPES_FIXED)
FFI_HEADER_ASSOCIATION(moz2d_test_ffi_fixed_signed_min_, FFI_TYPES_FIXED_SIGNED_MIN)
FFI_HEADER_ASSOCIATION(moz2d_test_ffi_fixed_signed_max_, FFI_TYPES_FIXED_SIGNED_MAX)
FFI_HEADER_ASSOCIATION(moz2d_test_ffi_fixed_unsigned_max_, FFI_TYPES_FIXED_UNSIGNED_MAX)

FFI_HEADER(moz2d_test_ffi_, FFI_TYPES)
FFI_HEADER_COMBO(moz2d_test_ffi_combo_, FFI_TYPES)
FFI_HEADER_ASSOCIATION(moz2d_test_ffi_signed_min_, FFI_TYPES_SIGNED_MIN)
FFI_HEADER_ASSOCIATION(moz2d_test_ffi_signed_max_, FFI_TYPES_SIGNED_MAX)
FFI_HEADER_ASSOCIATION(moz2d_test_ffi_unsigned_max_, FFI_TYPES_UNSIGNED_MAX)
FFI_HEADER_ASSOCIATION(moz2d_test_ffi_floating_min_, FFI_TYPES_FLOATING_MIN)
FFI_HEADER_ASSOCIATION(moz2d_test_ffi_floating_max_, FFI_TYPES_FLOATING_MAX)

FFI_CALLBACK_HEADER(moz2d_test_ffi_callback_, FFI_TYPES_FIXED)
FFI_CALLBACK_HEADER(moz2d_test_ffi_callback_, FFI_TYPES)

#ifdef __cplusplus
}
#endif

#endif //MOZ2D_TEST_FFI_H