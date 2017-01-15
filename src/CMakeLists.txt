add_definitions(-DMOZILLA_INTERNAL_API)

include_directories(${MOZ_TOP_PATH})
include_directories(${MOZ_TOP_PATH}/gfx)
include_directories(${MOZ_TOP_PATH}/gfx/cairo/cairo/src)
include_directories(${MOZ_TOP_OBJ_PATH}/gfx/cairo/cairo/src)
include_directories(${MOZ_TOP_PATH}/modules/freetype2/include)
include_directories(${MOZ_TOP_PATH}/gfx/skia)
include_directories(${MOZ_TOP_PATH}/gfx/skia/skia/include/config)
include_directories(${MOZ_TOP_PATH}/gfx/skia/skia/include/core)
include_directories(${MOZ_TOP_PATH}/gfx/skia/skia/include/utils)
include_directories(${MOZ_TOP_PATH}/gfx/skia/skia/include/gpu)
include_directories(${MOZ_TOP_PATH}/gfx/skia/skia/include/views)
include_directories(${MOZ_TOP_PATH}/ipc/chromium/src)

add_subdirectory(telemetry)
add_subdirectory(services)
add_subdirectory(undefined)

set(SRC
	draw_target.cpp gl.cpp draw_options.cpp filter_node.cpp color.cpp rect.cpp
	pattern.cpp path.cpp point.cpp text.cpp gl.cpp ref_ptr.cpp
    font_style.cpp font_family_name.cpp font_family_list.cpp
	font_group.cpp text_run.cpp source_surface.cpp debug.cpp
	gfx_config.cpp preferences.cpp test_enums.cpp test_ffi.cpp)

add_library(wrapper OBJECT ${SRC})