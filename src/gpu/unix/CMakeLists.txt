add_definitions(-DMOZILLA_INTERNAL_API)

include_directories(${MOZ_TOP_PATH})
include_directories(${MOZ_TOP_PATH}/gfx)
include_directories(${PROJECT_SOURCE_DIR}/src)
include_directories(${PROJECT_SOURCE_DIR}/src/gpu)

set(SRC
	gpu_manager_unix.cpp)

add_library(gpu_unix OBJECT ${SRC})