include_directories(${MOZ_TOP_PATH})
include_directories(${MOZ_TOP_PATH}/gfx/layers/ipc)
include_directories(${MOZ_TOP_PATH}/layout)
include_directories(${MOZ_TOP_PATH}/layout/generic)
include_directories(${MOZ_TOP_PATH}/layout/style)
include_directories(${MOZ_TOP_PATH}/dom/base)
include_directories(${MOZ_TOP_OBJ_PATH})
include_directories(${MOZ_TOP_PATH}/xpcom)
include_directories(${MOZ_TOP_OBJ_PATH}/xpcom)
include_directories(${MOZ_TOP_PATH}/xpcom/ds)
include_directories(${MOZ_TOP_PATH}/xpcom/glue)
include_directories(${MOZ_TOP_PATH}/xpcom/threads)
include_directories(${MOZ_TOP_PATH}/xpcom/build)
include_directories(${MOZ_TOP_PATH}/ipc/chromium/src)
include_directories(${MOZ_TOP_OBJ_PATH}/ipc/ipdl/_ipdlheaders)

set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DIMPL_LIBXUL")
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -I${MOZ_TOP_PATH}/ipc/chromium/src/base/logging.h")
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -I${MOZ_TOP_PATH}/xpcom/glue/nsComponentManagerUtils.h")
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -I${MOZ_TOP_PATH}/xpcom/base")
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -I${MOZ_TOP_PATH}/xpcom/components")
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -I${MOZ_TOP_PATH}/xpcom/io")
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -I${MOZ_TOP_PATH}/xpcom/reflect/xptcall")
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -I${MOZ_TOP_PATH}/xpcom/reflect/xptinfo")
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -I${MOZ_TOP_PATH}/docshell/base")
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -I${MOZ_TOP_PATH}/chrome")

set(SRC
    ${MOZ_TOP_PATH}/intl/icu/source/stubdata/stubdata.c
    ${MOZ_TOP_PATH}/xpcom/ds/nsCRT.cpp
    ${MOZ_TOP_PATH}/xpcom/ds/Tokenizer.cpp
    ${MOZ_TOP_PATH}/xpcom/ds/nsStaticNameTable.cpp
    
    ${MOZ_TOP_PATH}/xpcom/ds/nsObserverService.cpp
    ${MOZ_TOP_PATH}/xpcom/ds/nsObserverList.cpp
    ${MOZ_TOP_PATH}/xpcom/ds/nsStringEnumerator.cpp
    ${MOZ_TOP_PATH}/xpcom/ds/nsSupportsPrimitives.cpp
    #${MOZ_TOP_PATH}/xpcom/base/nsMemoryReporterManager.cpp
 
    ${MOZ_TOP_PATH}/xpcom/io/Base64.cpp
    ${MOZ_TOP_PATH}/xpcom/base/Logging.cpp
    ${MOZ_TOP_PATH}/xpcom/base/NSPRLogModulesParser.cpp
    ${MOZ_TOP_PATH}/xpcom/base/nsMemoryImpl.cpp
    ${MOZ_TOP_PATH}/xpcom/build/nsXPCOMStrings.cpp
    ${MOZ_TOP_PATH}/xpcom/build/Services.cpp
    ${MOZ_TOP_PATH}/xpcom/components/nsComponentManager.cpp
    ${MOZ_TOP_PATH}/xpcom/components/nsCategoryManager.cpp
    ${MOZ_TOP_PATH}/xpcom/io/nsNativeCharsetUtils.cpp
    
    #${MOZ_TOP_PATH}/xpcom/build/IOInterposer.cpp
    #${MOZ_TOP_PATH}/xpcom/threads/nsThread.cpp
    #${MOZ_TOP_PATH}/xpcom/threads/nsThreadManager.cpp
    #${MOZ_TOP_PATH}/xpcom/threads/nsEventQueue.cpp
    #MessageChannelFixed.cpp
    
    ${MOZ_TOP_PATH}/gfx/ipc/D3DMessageUtils.cpp
    ${MOZ_TOP_PATH}/gfx/layers/opengl/OGLShaderProgram.cpp
    ${MOZ_TOP_OBJ_PATH}/ipc/ipdl/GraphicsMessages.cpp
    nsContentUtils.cpp nsJS.cpp nsServices.cpp nsTextRunTransformationsFixed.cpp
    PGPU.cpp nsAppRunner.cpp nsPresContext.cpp GPUProcessManager.cpp nsFont.cpp
    nsFixedMemoryReporterManager.cpp ShaderProgram.cpp gfxPlatform.cpp nsSVGUtils.cpp
    nsWidget.cpp nsFakePresContext.cpp)

add_library(undefined OBJECT ${SRC})

# nsJS.cpp includes jsapi.h, jsfriendapi.h headers from js/src/
# nsJS.cpp includes ErrorResult.h headers from dom/bindings/
set_source_files_properties(nsJS.cpp PROPERTIES COMPILE_FLAGS "-I${MOZ_TOP_PATH}/js/src -I${MOZ_TOP_PATH}/dom/bindings")

# GPUProcessManager.cpp includes GPUProcessManager.h from gfx/ipc
set_source_files_properties(GPUProcessManager.cpp PROPERTIES COMPILE_FLAGS "-I${MOZ_TOP_PATH}/gfx/ipc")

set_source_files_properties(SVGContextPaint.cpp PROPERTIES COMPILE_FLAGS "-I${MOZ_TOP_PATH}/layout/svg")