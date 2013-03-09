# CMakeLists.txt for renderstack_mesh

include_directories(
   ${RENDERSTACK_WINDOW_TOOLKIT_INCLUDE_DIRS}
   ${GLM_INCLUDE_DIRS}
   ${CMAKE_SOURCE_DIR}/libraries/khronos
   ${CMAKE_SOURCE_DIR}/libraries/renderstack_geometry/include
   ${CMAKE_SOURCE_DIR}/libraries/renderstack_graphics/include
   ${CMAKE_SOURCE_DIR}/libraries/renderstack_mesh/include
   ${CMAKE_SOURCE_DIR}/libraries/renderstack_toolkit/include
)

add_library(
   renderstack_mesh STATIC
   include/renderstack_mesh/build_info.hpp
   include/renderstack_mesh/geometry_mesh.hpp
   include/renderstack_mesh/mesh.hpp
   include/renderstack_mesh/index_range.hpp
   include/renderstack_mesh/normal_style.hpp
   include/renderstack_mesh/mesh_mode.hpp
   source/build_info.cpp
   source/geometry_mesh.cpp
   source/mesh.cpp
   source/mesh_mode.cpp
   source/index_range.cpp
   source/normal_style.cpp
)


install(TARGETS renderstack_mesh DESTINATION lib)
