add_definitions(-DMOZILLA_INTERNAL_API)

include_directories(${MOZ_TOP_PATH})
include_directories(${MOZ_TOP_OBJ_PATH})
include_directories(${MOZ_TOP_OBJ_PATH}/dist/include/cairo)
include_directories(${MOZ_TOP_PATH}/ipc/chromium/src)
include_directories(${MOZ_TOP_PATH}/nsprpub/pr/include)
include_directories(${MOZ_TOP_OBJ_PATH}/ipc/ipdl/_ipdlheaders)

set(SRC
    ${MOZ_TOP_PATH}/ipc/chromium/src/base/platform_thread_win.cc
    ${MOZ_TOP_PATH}/ipc/chromium/src/base/pickle.cc
    ${MOZ_TOP_PATH}/intl/icu/source/stubdata/stubdata.c
	${MOZ_TOP_PATH}/gfx/layers/d3d9/DeviceManagerD3D9.cpp
	${MOZ_TOP_PATH}/gfx/layers/d3d9/Nv3DVUtils.cpp)

add_library(undefined_windows OBJECT ${SRC})

set_source_files_properties(${MOZ_TOP_PATH}/ipc/chromium/src/base/pickle.cc PROPERTIES COMPILE_FLAGS "/FI${MOZ_TOP_OBJ_PATH}/dist/include/mozilla/UniquePtr.h")