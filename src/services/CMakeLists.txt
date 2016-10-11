include_directories(${MOZ_TOP_PATH})
include_directories(${MOZ_TOP_PATH}/xpcom/ds)
include_directories(${MOZ_TOP_OBJ_PATH})
include_directories(${PROJECT_SOURCE_DIR}/src)


set(SRC
    ${MOZ_TOP_PATH}/dom/base/nsGkAtoms.cpp
    ${MOZ_TOP_PATH}/xpcom/ds/nsAtomTable.cpp
    ${MOZ_TOP_PATH}/xpcom/ds/nsAtomService.cpp
    services.cpp)

add_library(services OBJECT ${SRC})