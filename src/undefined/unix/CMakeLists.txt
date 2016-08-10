include_directories(${MOZ_TOP_PATH})
include_directories(${MOZ_TOP_OBJ_PATH})
include_directories(${MOZ_TOP_PATH}/ipc/chromium/src)
include_directories(${MOZ_TOP_OBJ_PATH}/ipc/ipdl/_ipdlheaders)

set(SRC
    ${MOZ_TOP_PATH}/ipc/chromium/src/base/platform_thread_posix.cc)

add_library(undefined_unix OBJECT ${SRC})