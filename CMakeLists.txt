cmake_minimum_required (VERSION 3.0)
project (Moz2D)

set (Moz2D_VERSION_MAJOR 1)
set (Moz2D_VERSION_MINOR 0)

set(CMAKE_OSX_ARCHITECTURES "i386")

set (MOZ_TOP_PATH ${PROJECT_SOURCE_DIR}/mozilla-central CACHE STRING "Path to mozilla-central repo")
set (MOZ_TOP_OBJ_PATH ${MOZ_TOP_PATH}/obj-mozilla CACHE STRING "Path to object directory within mozilla central repo")

enable_language(ASM)

add_definitions(-DMOZILLA_EXTERNAL_LINKAGE -DMOZ_DUMP_PAINTING -DXPCOM_GLUE_USE_NSPR)
#set (CMAKE_ASM_FLAGS "${CMAKE_ASM_FLAGS} -Wa,--noexecstack -include ${MOZ_TOP_OBJ_PATH}/mozilla-config.h")
set (CMAKE_ASM_FLAGS "${CMAKE_ASM_FLAGS} -arch i386 -m32 -Wa,--noexecstack -include ${MOZ_TOP_OBJ_PATH}/mozilla-config.h")
set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -mssse3 -msse4.1 -include ${MOZ_TOP_OBJ_PATH}/mozilla-config.h")
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -mssse3 -msse4.1 -std=gnu++11 -include ${MOZ_TOP_OBJ_PATH}/mozilla-config.h")
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Qunused-arguments -Wall -Wc++11-compat -Wempty-body -Wignored-qualifiers -Woverloaded-virtual -Wpointer-arith -Wsign-compare -Wtype-limits -Wunreachable-code -Wwrite-strings -Wc++11-compat-pedantic -Wc++14-compat -Wc++14-compat-pedantic -Wc++1z-compat -Wclass-varargs -Wimplicit-fallthrough -Wloop-analysis -Wstring-conversion -Wthread-safety -Wno-invalid-offsetof -Wno-inline-new-delete -Wno-error=deprecated-declarations -Wno-error=array-bounds -Wno-unknown-warning-option -Wno-return-type-c-linkage -fno-exceptions -fno-strict-aliasing -stdlib=libc++ -fno-rtti -fno-exceptions -fno-math-errno -pthread -pipe")
# We throw runtime exceptions if stub method gets executed on runtime.
# Normally, it should not happen.
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fexceptions")

set (CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -Wl")
# Plugin must not be compiled with dynamic lookup of undefined symbols, Pharo VM can not load plugin
#set (CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -undefined dynamic_lookup")
set (CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -framework CoreFoundation -framework OpenGL -framework IOSurface -framework Cocoa -framework CoreServices -framework CoreGraphics -framework CoreText -framework QuartzCore -framework IOKit")
set (CMAKE_POSITION_INDEPENDENT_CODE TURE)

include_directories(${MOZ_TOP_OBJ_PATH}/dist/include)
include_directories(${MOZ_TOP_OBJ_PATH}/dist/include/nspr)
include_directories(${MOZ_TOP_PATH}/intl/locale)

	add_subdirectory(${MOZ_TOP_OBJ_PATH}/gfx/2d)
	add_subdirectory(${MOZ_TOP_OBJ_PATH}/gfx/config)
	add_subdirectory(${MOZ_TOP_OBJ_PATH}/gfx/cairo/cairo/src)
	add_subdirectory(${MOZ_TOP_OBJ_PATH}/gfx/cairo/libpixman/src)
	add_subdirectory(${MOZ_TOP_OBJ_PATH}/gfx/gl)
	add_subdirectory(${MOZ_TOP_OBJ_PATH}/gfx/skia)
	add_subdirectory(${MOZ_TOP_OBJ_PATH}/gfx/src)
	add_subdirectory(${MOZ_TOP_OBJ_PATH}/gfx/graphite2/src)
	add_subdirectory(${MOZ_TOP_OBJ_PATH}/gfx/harfbuzz/src)
	add_subdirectory(${MOZ_TOP_OBJ_PATH}/gfx/ots/src)
	add_subdirectory(${MOZ_TOP_OBJ_PATH}/gfx/qcms)
	add_subdirectory(${MOZ_TOP_OBJ_PATH}/modules/woff2)
	add_subdirectory(${MOZ_TOP_OBJ_PATH}/modules/brotli)
	add_subdirectory(${MOZ_TOP_OBJ_PATH}/modules/zlib/src)
	add_subdirectory(${MOZ_TOP_OBJ_PATH}/gfx/thebes)

	add_subdirectory(${MOZ_TOP_OBJ_PATH}/memory/fallible)
	add_subdirectory(${MOZ_TOP_OBJ_PATH}/memory/mozalloc)
	add_subdirectory(${MOZ_TOP_OBJ_PATH}/mfbt)

	add_subdirectory(${MOZ_TOP_OBJ_PATH}/dom/encoding)
	
	add_subdirectory(${MOZ_TOP_OBJ_PATH}/xpcom/glue)
	add_subdirectory(${MOZ_TOP_OBJ_PATH}/xpcom/string)
		
	add_subdirectory(${MOZ_TOP_OBJ_PATH}/config/external/nspr/libc)
	add_subdirectory(${MOZ_TOP_OBJ_PATH}/config/external/nspr/pr)
	add_subdirectory(${MOZ_TOP_OBJ_PATH}/config/external/nspr/ds)
	add_subdirectory(${MOZ_TOP_OBJ_PATH}/config/external/icu/common)
	add_subdirectory(${MOZ_TOP_OBJ_PATH}/config/external/icu/stubdata)
	add_subdirectory(${MOZ_TOP_OBJ_PATH}/config/external/icu/i18n)

	add_subdirectory(${MOZ_TOP_OBJ_PATH}/intl/locale)
	add_subdirectory(${MOZ_TOP_OBJ_PATH}/intl/unicharutil/util/internal)
	add_subdirectory(${MOZ_TOP_OBJ_PATH}/mozglue/misc)
		
	add_subdirectory(${PROJECT_SOURCE_DIR}/src)

file(GLOB LIBRARIES "libs/mac/*.dylib")

add_library(${PROJECT_NAME} SHARED

	$<TARGET_OBJECTS:gfx_2d>
	$<TARGET_OBJECTS:gfx_config>
	$<TARGET_OBJECTS:gfx_gl>
	$<TARGET_OBJECTS:gfx_cairo_cairo_src>
	$<TARGET_OBJECTS:gfx_cairo_libpixman_src>
	$<TARGET_OBJECTS:gfx_skia>
	$<TARGET_OBJECTS:gfx_src>

	# Harfbuzz is needed to work with Glyphs and Text	
	$<TARGET_OBJECTS:gfx_harfbuzz_src>
	# Graphite2 is needed to work with Glyphs and Text	
	$<TARGET_OBJECTS:gfx_graphite2_src>
	# Quick color management used by Thebes
	$<TARGET_OBJECTS:gfx_qcms>
	# Sanitiser for OpenType used by Thebes
	$<TARGET_OBJECTS:gfx_ots_src>
	# Needed by OTS
	$<TARGET_OBJECTS:modules_zlib_src>
	# Needed by OTS
	$<TARGET_OBJECTS:woff2>
	# Needed by woff2
	$<TARGET_OBJECTS:brotli>
	# Thebes is needed to work with platform dependend fonts
	$<TARGET_OBJECTS:gfx_thebes>

	$<TARGET_OBJECTS:fallible>
	$<TARGET_OBJECTS:memory_mozalloc>
	# Mozilla Framework Based on Templates https://developer.mozilla.org/en-US/docs/Mozilla/MFBT
	$<TARGET_OBJECTS:mfbt>

	# xpcom glue provides hash tables and various ns* stuff for thebes
	$<TARGET_OBJECTS:xpcomglue_s>
	# For nsString and friends needed by thebes
	$<TARGET_OBJECTS:xpcom_string>
	# For PR_strtod and PR_vsxprintf needed by xpcom_string
	$<TARGET_OBJECTS:nspr4>
	# For PL_strncasecmp needed by xpcom_string
	$<TARGET_OBJECTS:plc4>
	# For PL_HashTable needed by nsLocale
	$<TARGET_OBJECTS:plds4>

	# Needed bu FontUtils to get raw font name
	$<TARGET_OBJECTS:dom_encoding>

	# Needed to support Unicode
	$<TARGET_OBJECTS:intl_locale>
	$<TARGET_OBJECTS:intl_locale_mac>
	$<TARGET_OBJECTS:intl_unicharutil_util_internal>
	$<TARGET_OBJECTS:icuuc>
	$<TARGET_OBJECTS:icustubdata>
	$<TARGET_OBJECTS:icui18n>

	# TimeStamp utils
	$<TARGET_OBJECTS:mozglue_misc>

	# C bindings
	$<TARGET_OBJECTS:wrapper>
	# Fake Telemetry
	$<TARGET_OBJECTS:telemetry>
	# Fake preferences
	$<TARGET_OBJECTS:preferences>
	# Services
	$<TARGET_OBJECTS:services>
	# Defines undefined symbols
	$<TARGET_OBJECTS:undefined>	
)

set_source_files_properties(${MOZ_TOP_PATH}/xpcom/glue/nsThreadUtils.cpp PROPERTIES COMPILE_FLAGS "-DMOZILLA_INTERNAL_API")

target_link_libraries(${PROJECT_NAME} ${LIBRARIES})

# Uncomment to experiment with api without Pharo
add_definitions(-DMOZILLA_INTERNAL_API)
include_directories(${MOZ_TOP_PATH}/gfx)
# Add executable called "main" that is built from the source files 
add_executable (main src/main.cpp) 
# Link the executable to the lib library. 
target_link_libraries (main ${PROJECT_NAME}) 