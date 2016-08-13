add_definitions(-DMOZILLA_INTERNAL_API)

include_directories(${MOZ_TOP_PATH}/gfx)
include_directories(${MOZ_TOP_PATH}/gfx/cairo/cairo/src)
include_directories(${MOZ_TOP_OBJ_PATH}/gfx/cairo/cairo/src)
include_directories(${MOZ_TOP_PATH}/modules/freetype2/include)
include_directories(${MOZ_TOP_PATH}/gfx/skia)
include_directories(${MOZ_TOP_PATH}/gfx/skia/skia/include/config)
include_directories(${MOZ_TOP_PATH}/gfx/skia/skia/include/core)

add_subdirectory(telemetry)
add_subdirectory(preferences)
add_subdirectory(services)
add_subdirectory(undefined)

set(SRC
	draw_target.cpp draw_options.cpp filter_node.cpp color.cpp rect.cpp
	pattern.cpp path.cpp point.cpp text.cpp
    font_style.cpp font_family_name.cpp font_family_list.cpp
	font_group.cpp text_run.cpp source_surface.cpp)

add_library(wrapper OBJECT ${SRC})