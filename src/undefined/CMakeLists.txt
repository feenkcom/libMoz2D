include_directories(${MOZ_TOP_PATH})
include_directories(${MOZ_TOP_OBJ_PATH})
include_directories(${MOZ_TOP_PATH}/xpcom/ds)
include_directories(${MOZ_TOP_PATH}/xpcom/glue)

add_definitions(-DMOZILLA_INTERNAL_API)

set(SRC
    ${MOZ_TOP_PATH}/xpcom/ds/nsCRT.cpp
    ${MOZ_TOP_PATH}/xpcom/ds/Tokenizer.cpp
    ${MOZ_TOP_PATH}/xpcom/io/Base64.cpp
    ${MOZ_TOP_PATH}/xpcom/base/Logging.cpp
    ${MOZ_TOP_PATH}/xpcom/base/NSPRLogModulesParser.cpp
    nsContentUtils.cpp nsJS.cpp nsServices.cpp nsTextRunTransformationsFixed.cpp PGPU.cpp nsAppRunner.cpp nsPresContext.cpp GPUProcessManager.cpp nsFont.cpp nsMemoryReporterManager.cpp)

add_library(undefined OBJECT ${SRC})

# nsJS.cpp includes jsapi.h, jsfriendapi.h headers from js/src/
# nsJS.cpp includes ErrorResult.h headers from dom/bindings/
set_source_files_properties(nsJS.cpp PROPERTIES COMPILE_FLAGS "-I${MOZ_TOP_PATH}/js/src -I${MOZ_TOP_PATH}/dom/bindings")

# PGPU.cpp includes PGPU.h from ipc/ipdl/_ipdlheaders
# PGPU.h includes base/process_util.h from ipc/chromium/src
set_source_files_properties(PGPU.cpp PROPERTIES COMPILE_FLAGS "-I${MOZ_TOP_OBJ_PATH}/ipc/ipdl/_ipdlheaders -I${MOZ_TOP_PATH}/ipc/chromium/src")

# GPUProcessManager.cpp includes GPUProcessManager.h from gfx/ipc
set_source_files_properties(GPUProcessManager.cpp PROPERTIES COMPILE_FLAGS "-I${MOZ_TOP_PATH}/gfx/ipc -I${MOZ_TOP_PATH}/ipc/chromium/src -I${MOZ_TOP_OBJ_PATH}/ipc/ipdl/_ipdlheaders")