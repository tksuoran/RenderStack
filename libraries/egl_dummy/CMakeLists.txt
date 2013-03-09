# CMakeLists.txt for egl-dummy

include_directories(
   ${CMAKE_SOURCE_DIR}/libraries/khronos
)

add_library(
    libEGL STATIC
    egl.c
)

install(TARGETS libEGL DESTINATION lib)

