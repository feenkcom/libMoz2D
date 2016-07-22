include_directories(${MOZ_TOP_PATH}/gfx)


set(SRC draw_target.cpp draw_options.cpp filter_node.cpp color.cpp rect.cpp pattern.cpp path.cpp structs.cpp)

add_library(wrapper OBJECT ${SRC})