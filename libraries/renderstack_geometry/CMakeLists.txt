# CMakeLists.txt for renderstack_geometry

include_directories(
   ${GLM_INCLUDE_DIRS}
   ${CMAKE_SOURCE_DIR}/libraries/renderstack_geometry/include
   ${CMAKE_SOURCE_DIR}/libraries/renderstack_toolkit/include
)

add_library(
   renderstack_geometry STATIC
   include/renderstack_geometry/property_map.hpp
   include/renderstack_geometry/property_map.inl
   include/renderstack_geometry/property_map_collection.hpp
   include/renderstack_geometry/property_map_collection.inl
   include/renderstack_geometry/cone.hpp
   include/renderstack_geometry/corner.hpp
   include/renderstack_geometry/disc.hpp
   include/renderstack_geometry/edge.hpp
   include/renderstack_geometry/exception.hpp
   include/renderstack_geometry/geometry.hpp
   include/renderstack_geometry/operation/geometry_operation.hpp
   include/renderstack_geometry/operation/clone.hpp
   include/renderstack_geometry/operation/catmull_clark.hpp
   include/renderstack_geometry/point.hpp
   include/renderstack_geometry/polygon.hpp
   include/renderstack_geometry/polygon.inl
   include/renderstack_geometry/sphere.hpp
   include/renderstack_geometry/triangle.hpp
   include/renderstack_geometry/cube.hpp
   source/cone.cpp
   source/corner.cpp
   source/disc.cpp
   source/geometry.cpp
   source/operation/geometry_operation.cpp
   source/operation/clone.cpp
   source/operation/catmull_clark.cpp
   source/polygon.cpp
   source/sphere.cpp
   source/triangle.cpp
   source/cube.cpp
)

source_group(
   "RenderStack Geometry Sources"
   REGULAR_EXPRESSION
   "include/renderstack_geometry/.*\\.inl|include/renderstack_geometry/.*\\.hpp|source/.*\\.cpp")

install(TARGETS renderstack_geometry DESTINATION lib)
