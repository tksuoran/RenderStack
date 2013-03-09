# CMakeLists.txt for renderstack_scene

include_directories(
   ${RENDERSTACK_WINDOW_TOOLKIT_INCLUDE_DIRS}
   ${GLM_INCLUDE_DIRS}
   ${CMAKE_SOURCE_DIR}/libraries/khronos
   ${CMAKE_SOURCE_DIR}/libraries/renderstack_geometry/include
   ${CMAKE_SOURCE_DIR}/libraries/renderstack_graphics/include
   ${CMAKE_SOURCE_DIR}/libraries/renderstack_mesh/include
   ${CMAKE_SOURCE_DIR}/libraries/renderstack_scene/include
   ${CMAKE_SOURCE_DIR}/libraries/renderstack_toolkit/include
)

add_library(
   renderstack_scene STATIC
   include/renderstack_scene/camera.hpp
   include/renderstack_scene/frame.hpp
   include/renderstack_scene/light.hpp
   include/renderstack_scene/projection.hpp
   include/renderstack_scene/stereo_parameters.hpp
   include/renderstack_scene/transform.hpp
   source/camera.cpp
   source/frame.cpp
   source/light.cpp
   source/projection.cpp
   source/stereo_parameters.cpp
   source/transform.cpp
)

install(TARGETS renderstack_scene DESTINATION lib)
