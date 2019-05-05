# CMakeLists.txt for renderstack_scene

include_directories(
    ${GLM_INCLUDE_DIRS}
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
    include/renderstack_scene/log.hpp
    include/renderstack_scene/projection.hpp
    include/renderstack_scene/stereo_parameters.hpp
    include/renderstack_scene/transform.hpp
    include/renderstack_scene/viewport.hpp
    source/camera.cpp
    source/frame.cpp
    source/light.cpp
    source/log.cpp
    source/projection.cpp
    source/stereo_parameters.cpp
    source/transform.cpp
)

set_target_properties(renderstack_scene PROPERTIES
                      CXX_STANDARD 17
                      CXX_STANDARD_REQUIRED ON
                      COMPILE_FLAGS "${WARNING_FLAGS}")

if (CLANG_TIDY_EXE)
    set_target_properties(renderstack_scene PROPERTIES CXX_CLANG_TIDY "${DO_CLANG_TIDY}")
endif()

source_group("RenderStack Scene Sources"
             REGULAR_EXPRESSION
             "include/renderstack_scene/.*\\.inl|include/renderstack_scene/.*\\.hpp|source/.*\\.cpp")

install(TARGETS renderstack_scene DESTINATION lib)
