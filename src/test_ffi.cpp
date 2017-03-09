//
// Created by Aliaksei Syrel on 13.01.17.
//

#include "test_ffi.h"

#define FFI_SOURCE_FOR_EACH(prefix, type) LIBRARY_API type FFI_FUNCTION_NAME(prefix, type) (type aValue) { return aValue; };
#define FFI_SOURCE(prefix, ...) FOR_EACH(FFI_SOURCE_FOR_EACH, prefix, __VA_ARGS__)

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
FFI_SOURCE_ASSOCIATION(moz2d_test_ffi_signed_min_, FFI_TYPES_SIGNED_MIN)
FFI_SOURCE_ASSOCIATION(moz2d_test_ffi_signed_max_, FFI_TYPES_SIGNED_MAX)
FFI_SOURCE_ASSOCIATION(moz2d_test_ffi_unsigned_max_, FFI_TYPES_UNSIGNED_MAX)

FFI_CALLBACK_SOURCE(moz2d_test_ffi_callback_, FFI_TYPES)