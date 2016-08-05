add_definitions(-DNDEBUG=1)
add_definitions(-DTRIMMED=1)
add_definitions(-DOS_POSIX=1)
add_definitions(-DOS_MACOSX=1)
add_definitions(-DOS_ARCH=Darwin)
add_definitions(-DMOZ_WIDGET_TOOLKIT=cocoa)
add_definitions(-DSTATIC_EXPORTABLE_JS_API)
add_definitions(-DMOZILLA_INTERNAL_API)
add_definitions(-DIMPL_LIBXUL)

include_directories(${MOZ_TOP_PATH})
include_directories(${MOZ_TOP_OBJ_PATH})

set(SRC FakePreferences.cpp)

add_library(preferences OBJECT ${SRC})