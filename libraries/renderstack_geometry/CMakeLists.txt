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
    include/renderstack_geometry/exception.hpp
    include/renderstack_geometry/geometry.hpp
    include/renderstack_geometry/geometry.inl
    include/renderstack_geometry/log.hpp
    include/renderstack_geometry/corner.inl
    include/renderstack_geometry/polygon.inl
    include/renderstack_geometry/operation/geometry_operation.hpp
    include/renderstack_geometry/operation/clone.hpp
    include/renderstack_geometry/operation/catmull_clark.hpp
    include/renderstack_geometry/shapes/cone.hpp
    include/renderstack_geometry/shapes/disc.hpp
    include/renderstack_geometry/shapes/sphere.hpp
    include/renderstack_geometry/shapes/triangle.hpp
    include/renderstack_geometry/shapes/cube.hpp
    include/renderstack_geometry/shapes/torus.hpp
    source/geometry.cpp
    source/log.cpp
    source/polygon.cpp
    source/operation/geometry_operation.cpp
    source/operation/clone.cpp
    source/operation/catmull_clark.cpp
    source/shapes/cone.cpp
    source/shapes/disc.cpp
    source/shapes/sphere.cpp
    source/shapes/triangle.cpp
    source/shapes/cube.cpp
    source/shapes/torus.cpp
)

set_target_properties(renderstack_geometry PROPERTIES
                      CXX_STANDARD 17
                      CXX_STANDARD_REQUIRED ON
                      COMPILE_FLAGS "${WARNING_FLAGS}")

if (CLANG_TIDY_EXE)
    set_target_properties(renderstack_geometry PROPERTIES CXX_CLANG_TIDY "${DO_CLANG_TIDY}")
endif()

source_group("RenderStack Geometry Sources"
             REGULAR_EXPRESSION
             "include/renderstack_geometry/.*\\.inl|include/renderstack_geometry/.*\\.hpp|source/.*\\.cpp")

install(TARGETS renderstack_geometry DESTINATION lib)
