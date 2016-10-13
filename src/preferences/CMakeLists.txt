add_definitions(-DNDEBUG=1)
add_definitions(-DTRIMMED=1)
add_definitions(-DSTATIC_EXPORTABLE_JS_API)
add_definitions(-DMOZILLA_INTERNAL_API)
add_definitions(-DIMPL_LIBXUL)

include_directories(${MOZ_TOP_PATH})
include_directories(${MOZ_TOP_OBJ_PATH})

set(SRC FakePreferences.cpp)

add_library(preferences OBJECT ${SRC})