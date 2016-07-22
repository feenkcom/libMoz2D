cmake_minimum_required (VERSION 3.0)
project (Moz2D)

set (Moz2D_VERSION_MAJOR 1)
set (Moz2D_VERSION_MINOR 0)

set(CMAKE_OSX_ARCHITECTURES "i386")

set (MOZ_TOP_PATH ${PROJECT_SOURCE_DIR}/mozilla-central CACHE STRING "Path to mozilla-central repo")
set (MOZ_TOP_OBJ_PATH ${MOZ_TOP_PATH}/obj-mozilla CACHE STRING "Path to object directory within mozilla central repo")

enable_language(ASM)

add_definitions(-DMOZILLA_EXTERNAL_LINKAGE)
#set (CMAKE_ASM_FLAGS "${CMAKE_ASM_FLAGS} -Wa,--noexecstack -include ${MOZ_TOP_OBJ_PATH}/mozilla-config.h")
set (CMAKE_ASM_FLAGS "${CMAKE_ASM_FLAGS} -arch i386 -m32 -Wa,--noexecstack -include ${MOZ_TOP_OBJ_PATH}/mozilla-config.h")
set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -mssse3 -msse4.1 -include ${MOZ_TOP_OBJ_PATH}/mozilla-config.h")
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -mssse3 -msse4.1 -std=gnu++11 -include ${MOZ_TOP_OBJ_PATH}/mozilla-config.h")
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Qunused-arguments -Wall -Wc++11-compat -Wempty-body -Wignored-qualifiers -Woverloaded-virtual -Wpointer-arith -Wsign-compare -Wtype-limits -Wunreachable-code -Wwrite-strings -Wc++11-compat-pedantic -Wc++14-compat -Wc++14-compat-pedantic -Wclass-varargs -Wimplicit-fallthrough -Wloop-analysis -Wthread-safety -Wno-invalid-offsetof -Wno-inline-new-delete -Wno-error=deprecated-declarations -Wno-error=array-bounds -Wno-unknown-warning-option -Wno-return-type-c-linkage -fno-exceptions -fno-strict-aliasing -stdlib=libc++ -fno-rtti -fno-exceptions -fno-math-errno -pthread -pipe")

set (CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -Wl")
set (CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -framework CoreFoundation -framework OpenGL -framework IOSurface -framework Cocoa -framework CoreServices -framework CoreGraphics -framework CoreText -framework QuartzCore")
set (CMAKE_POSITION_INDEPENDENT_CODE TURE)

include_directories(${MOZ_TOP_OBJ_PATH}/dist/include)
include_directories(${MOZ_TOP_OBJ_PATH}/dist/include/nspr)
  
	add_subdirectory(${MOZ_TOP_OBJ_PATH}/gfx/2d)
	add_subdirectory(${MOZ_TOP_OBJ_PATH}/gfx/cairo/cairo/src)
	add_subdirectory(${MOZ_TOP_OBJ_PATH}/gfx/cairo/libpixman/src)
	add_subdirectory(${MOZ_TOP_OBJ_PATH}/gfx/skia)

	add_subdirectory(${MOZ_TOP_OBJ_PATH}/memory/fallible)
	add_subdirectory(${MOZ_TOP_OBJ_PATH}/memory/mozalloc)
	add_subdirectory(${MOZ_TOP_OBJ_PATH}/mfbt)

	add_subdirectory(${PROJECT_SOURCE_DIR}/src)

add_library(${PROJECT_NAME} SHARED

	$<TARGET_OBJECTS:gfx_2d>
	$<TARGET_OBJECTS:gfx_cairo_cairo_src>
	$<TARGET_OBJECTS:gfx_cairo_libpixman_src>
	$<TARGET_OBJECTS:gfx_skia>

	$<TARGET_OBJECTS:fallible>
	$<TARGET_OBJECTS:memory_mozalloc>
	$<TARGET_OBJECTS:mfbt> 
  
	$<TARGET_OBJECTS:wrapper>
)