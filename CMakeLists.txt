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
#set (CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -undefined dynamic_lookup")
set (CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -framework CoreFoundation -framework OpenGL -framework IOSurface -framework Cocoa -framework CoreServices -framework CoreGraphics -framework CoreText -framework QuartzCore")
set (CMAKE_POSITION_INDEPENDENT_CODE TURE)

include_directories(${MOZ_TOP_OBJ_PATH}/dist/include)
include_directories(${MOZ_TOP_OBJ_PATH}/dist/include/nspr)

	add_subdirectory(${MOZ_TOP_OBJ_PATH}/gfx/2d)
	add_subdirectory(${MOZ_TOP_OBJ_PATH}/gfx/cairo/cairo/src)
	add_subdirectory(${MOZ_TOP_OBJ_PATH}/gfx/cairo/libpixman/src)
	add_subdirectory(${MOZ_TOP_OBJ_PATH}/gfx/skia)
	add_subdirectory(${MOZ_TOP_OBJ_PATH}/gfx/graphite2/src)
	add_subdirectory(${MOZ_TOP_OBJ_PATH}/gfx/harfbuzz/src)
	# add_subdirectory(${MOZ_TOP_OBJ_PATH}/gfx/thebes)

	add_subdirectory(${MOZ_TOP_OBJ_PATH}/memory/fallible)
	add_subdirectory(${MOZ_TOP_OBJ_PATH}/memory/mozalloc)
	add_subdirectory(${MOZ_TOP_OBJ_PATH}/mfbt)
	
	add_subdirectory(${MOZ_TOP_OBJ_PATH}/xpcom/glue)
	add_subdirectory(${MOZ_TOP_OBJ_PATH}/xpcom/string)
	add_subdirectory(${MOZ_TOP_OBJ_PATH}/config/external/nspr/libc)
	add_subdirectory(${MOZ_TOP_OBJ_PATH}/config/external/nspr/pr)
	#add_subdirectory(${MOZ_TOP_OBJ_PATH}/intl/unicharutil)
	#add_subdirectory(${MOZ_TOP_OBJ_PATH}/intl/unicharutil/util)
	
	add_subdirectory(${PROJECT_SOURCE_DIR}/src)

file(GLOB LIBRARIES "*.dylib")

add_library(${PROJECT_NAME} SHARED

	$<TARGET_OBJECTS:gfx_2d>
	$<TARGET_OBJECTS:gfx_cairo_cairo_src>
	$<TARGET_OBJECTS:gfx_cairo_libpixman_src>
	$<TARGET_OBJECTS:gfx_skia>
	
	# Harfbuzz is needed to work with Glyphs and Text	
	$<TARGET_OBJECTS:gfx_harfbuzz_src>
	# Graphite2 is needed to work with Glyphs and Text	
	$<TARGET_OBJECTS:gfx_graphite2_src>

	$<TARGET_OBJECTS:fallible>
	$<TARGET_OBJECTS:memory_mozalloc>
	$<TARGET_OBJECTS:mfbt>
	
	# Thebes is needed to work with platform dependend fonts
	# $<TARGET_OBJECTS:gfx_thebes>
	
	# xpcom glue provides hash tables and various ns* stuff for thebes
	$<TARGET_OBJECTS:xpcomglue>
	# For nsString and friends needed by thebes
	$<TARGET_OBJECTS:xpcom_string>
	# For PR_strtod and PR_vsxprintf needed by xpcom_string
	$<TARGET_OBJECTS:nspr4>
	# For PL_strncasecmp needed by xpcom_string
	$<TARGET_OBJECTS:plc4>
	
	# Maybe usefull encoding convertions utils to work with text
	#$<TARGET_OBJECTS:intl_unicharutil>
	#$<TARGET_OBJECTS:unicharutil_external_s>
	
	$<TARGET_OBJECTS:wrapper>
)

target_link_libraries(${PROJECT_NAME} ${LIBRARIES})

include_directories(${MOZ_TOP_PATH}/gfx)
include_directories(${MOZ_TOP_PATH}/gfx/cairo/cairo/src)
include_directories(${MOZ_TOP_OBJ_PATH}/gfx/cairo/cairo/src)
#include_directories(${MOZ_TOP_PATH}/modules/freetype2/include)
# Add executable called "main" that is built from the source files 
add_executable (main src/main.cpp) 
# Link the executable to the lib library. 
target_link_libraries (main ${PROJECT_NAME}) 