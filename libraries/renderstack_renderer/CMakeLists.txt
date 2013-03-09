# CMakeLists.txt for renderstack_renderer

include_directories(
   ${RENDERSTACK_WINDOW_TOOLKIT_INCLUDE_DIRS}
   ${GLM_INCLUDE_DIRS}
   ${CMAKE_SOURCE_DIR}/libraries/khronos
   ${CMAKE_SOURCE_DIR}/libraries/renderstack_geometry/include
   ${CMAKE_SOURCE_DIR}/libraries/renderstack_graphics/include
   ${CMAKE_SOURCE_DIR}/libraries/renderstack_mesh/include
   ${CMAKE_SOURCE_DIR}/libraries/renderstack_renderer/include
   ${CMAKE_SOURCE_DIR}/libraries/renderstack_toolkit/include
)

add_library(
   renderstack_renderer STATIC
   include/renderstack_renderer/blend_state.hpp
   include/renderstack_renderer/context.hpp
   include/renderstack_renderer/depth_state.hpp
   include/renderstack_renderer/face_cull_state.hpp
   include/renderstack_renderer/color_mask_state.hpp
   include/renderstack_renderer/render_state.hpp
   include/renderstack_renderer/renderer.hpp
   include/renderstack_renderer/stencil_state.hpp
   source/blend_state.cpp
   source/context.cpp
   source/depth_state.cpp
   source/face_cull_state.cpp
   source/color_mask_state.cpp
   source/renderer.cpp
   source/stencil_state.cpp
)


install(TARGETS renderstack_renderer DESTINATION lib)
