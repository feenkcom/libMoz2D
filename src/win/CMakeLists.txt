add_definitions(-DMOZILLA_INTERNAL_API)

include_directories(${MOZ_TOP_PATH})
include_directories(${MOZ_TOP_PATH}/gfx)
include_directories(${PROJECT_SOURCE_DIR}/src)

set(SRC
	gpu_manager_windows.cpp)

add_library(wrapper_windows OBJECT ${SRC})