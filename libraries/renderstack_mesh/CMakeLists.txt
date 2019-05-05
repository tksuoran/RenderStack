# CMakeLists.txt for renderstack_mesh

include_directories(
    ${GLM_INCLUDE_DIRS}
    ${CMAKE_SOURCE_DIR}/libraries/renderstack_renderer/include
    ${CMAKE_SOURCE_DIR}/libraries/renderstack_geometry/include
    ${CMAKE_SOURCE_DIR}/libraries/renderstack_graphics/include
    ${CMAKE_SOURCE_DIR}/libraries/renderstack_mesh/include
    ${CMAKE_SOURCE_DIR}/libraries/renderstack_toolkit/include
)

add_library(
    renderstack_mesh STATIC
    include/renderstack_mesh/build_info.hpp
    include/renderstack_mesh/geometry_mesh.hpp
    include/renderstack_mesh/log.hpp
    include/renderstack_mesh/mesh.hpp
    include/renderstack_mesh/index_range.hpp
    include/renderstack_mesh/normal_style.hpp
    include/renderstack_mesh/mesh_mode.hpp
    source/geometry_mesh.cpp
    source/log.cpp
    source/mesh.cpp
    source/mesh_mode.cpp
    source/index_range.cpp
    source/normal_style.cpp
)

set_target_properties(renderstack_mesh PROPERTIES
                      CXX_STANDARD 17
                      CXX_STANDARD_REQUIRED ON
                      COMPILE_FLAGS "${WARNING_FLAGS}")

if (CLANG_TIDY_EXE)
    set_target_properties(renderstack_mesh PROPERTIES CXX_CLANG_TIDY "${DO_CLANG_TIDY}")
endif()

source_group("RenderStack Mesh Sources"
             REGULAR_EXPRESSION
             "include/renderstack_mesh/.*\\.inl|include/renderstack_mesh/.*\\.hpp|source/.*\\.cpp")

install(TARGETS renderstack_mesh DESTINATION lib)
