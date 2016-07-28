include_directories(${MOZ_TOP_PATH})

set(SRC FakeTelemetry.cpp)

add_library(telemetry OBJECT ${SRC})