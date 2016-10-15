include_directories(${MOZ_TOP_PATH})
include_directories(${MOZ_TOP_PATH}/chrome)
include_directories(${MOZ_TOP_PATH}/xpcom/ds)
include_directories(${MOZ_TOP_PATH}/xpcom/glue)
include_directories(${MOZ_TOP_PATH}/xpcom/components)
include_directories(${MOZ_TOP_PATH}/xpcom/base)
include_directories(${MOZ_TOP_PATH}/xpcom/build)
include_directories(${MOZ_TOP_PATH}/xpcom/io)
include_directories(${MOZ_TOP_PATH}/xpcom/threads)
include_directories(${MOZ_TOP_PATH}/xpcom/reflect/xptinfo)
include_directories(${MOZ_TOP_OBJ_PATH})
include_directories(${MOZ_TOP_OBJ_PATH}/xpcom)
include_directories(${PROJECT_SOURCE_DIR}/src)


set(SRC
    ${MOZ_TOP_PATH}/dom/base/nsGkAtoms.cpp
    ${MOZ_TOP_PATH}/xpcom/ds/nsAtomTable.cpp
    ${MOZ_TOP_PATH}/xpcom/ds/nsAtomService.cpp
    services.cpp)

add_library(services OBJECT ${SRC})