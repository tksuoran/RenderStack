# CMakeLists.txt for gles3-dummy

include_directories(
${CMAKE_SOURCE_DIR}/libraries/khronos
)

add_library(
    libGLESv2 STATIC
    gles3.c
)

install(TARGETS libGLESv2 DESTINATION lib)

