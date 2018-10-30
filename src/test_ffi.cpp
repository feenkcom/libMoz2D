//
// Created by Aliaksei Syrel on 13.01.17.
//

#include "test_ffi.h"

#define FFI_SOURCE_FOR_EACH(prefix, type) LIBRARY_API type FFI_FUNCTION_NAME(prefix, type) (type aValue) { return aValue; };
#define FFI_SOURCE(prefix, ...) FOR_EACH(FFI_SOURCE_FOR_EACH, prefix, __VA_ARGS__)

#define FFI_SOURCE_COMBO_FOR_EACH(prefix, type) LIBRARY_API bool FFI_FUNCTION_NAME(prefix, type) (type aValue1, type aValue2, type aValue3, type aValue4, type aValue5) { return aValue1 == aValue2 && aValue2 == aValue3 && aValue3 == aValue4 && aValue4 == aValue5; };
#define FFI_SOURCE_COMBO(prefix, ...) FOR_EACH(FFI_SOURCE_COMBO_FOR_EACH, prefix, __VA_ARGS__)

#define FFI_SOURCE_FOR_EACH_ASSOCIATION(prefix, type, value) type FFI_FUNCTION_NAME(prefix, type) () { return value; };
#define FFI_SOURCE_ASSOCIATION(prefix, ...) PAIRS_DO(FFI_SOURCE_FOR_EACH_ASSOCIATION, prefix, __VA_ARGS__)

#define FFI_CALLBACK_SOURCE_FOR_EACH(prefix, type) LIBRARY_API type FFI_FUNCTION_NAME(prefix, type) (type(*function)(type), type value) { return function(value); };
#define FFI_CALLBACK_SOURCE(prefix, ...) FOR_EACH(FFI_CALLBACK_SOURCE_FOR_EACH, prefix, __VA_ARGS__)

FFI_SOURCE(moz2d_test_ffi_fixed_, FFI_TYPES_FIXED)
FFI_SOURCE_ASSOCIATION(moz2d_test_ffi_fixed_signed_min_, FFI_TYPES_FIXED_SIGNED_MIN)
FFI_SOURCE_ASSOCIATION(moz2d_test_ffi_fixed_signed_max_, FFI_TYPES_FIXED_SIGNED_MAX)
FFI_SOURCE_ASSOCIATION(moz2d_test_ffi_fixed_unsigned_max_, FFI_TYPES_FIXED_UNSIGNED_MAX)

FFI_CALLBACK_SOURCE(moz2d_test_ffi_callback_, FFI_TYPES_FIXED)

FFI_SOURCE(moz2d_test_ffi_, FFI_TYPES)
FFI_SOURCE_COMBO(moz2d_test_ffi_combo_, FFI_TYPES)
FFI_SOURCE_ASSOCIATION(moz2d_test_ffi_signed_min_, FFI_TYPES_SIGNED_MIN)
FFI_SOURCE_ASSOCIATION(moz2d_test_ffi_signed_max_, FFI_TYPES_SIGNED_MAX)
FFI_SOURCE_ASSOCIATION(moz2d_test_ffi_unsigned_max_, FFI_TYPES_UNSIGNED_MAX)
FFI_SOURCE_ASSOCIATION(moz2d_test_ffi_floating_min_, FFI_TYPES_FLOATING_MIN)
FFI_SOURCE_ASSOCIATION(moz2d_test_ffi_floating_max_, FFI_TYPES_FLOATING_MAX)

FFI_CALLBACK_SOURCE(moz2d_test_ffi_callback_, FFI_TYPES)