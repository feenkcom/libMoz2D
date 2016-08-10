include_directories(${MOZ_TOP_PATH})
include_directories(${MOZ_TOP_OBJ_PATH})
include_directories(${MOZ_TOP_PATH}/ipc/chromium/src)
include_directories(${MOZ_TOP_OBJ_PATH}/ipc/ipdl/_ipdlheaders)

set(SRC
    ${MOZ_TOP_PATH}/ipc/chromium/src/base/platform_thread_posix.cc
    ${MOZ_TOP_PATH}/ipc/chromium/src/base/platform_thread_mac.mm
    ${MOZ_TOP_PATH}/ipc/chromium/src/base/scoped_nsautorelease_pool.mm)

add_library(undefined_mac OBJECT ${SRC})