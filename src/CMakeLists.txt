include_directories(${MOZ_TOP_PATH}/gfx)
include_directories(${MOZ_TOP_PATH}/gfx/cairo/cairo/src)
include_directories(${MOZ_TOP_OBJ_PATH}/gfx/cairo/cairo/src)
include_directories(${MOZ_TOP_PATH}/modules/freetype2/include)
include_directories(${MOZ_TOP_PATH}/gfx/skia)
include_directories(${MOZ_TOP_PATH}/gfx/skia/skia/include/config)
include_directories(${MOZ_TOP_PATH}/gfx/skia/skia/include/core)

set(SRC draw_target.cpp draw_options.cpp filter_node.cpp color.cpp rect.cpp pattern.cpp path.cpp structs.cpp point.cpp text.cpp font.cpp mac_font.mm)

add_library(wrapper OBJECT ${SRC})