# CMakeLists.txt for renderstack_toolkit

set(
    sources
    include/renderstack_toolkit/gl_core.hpp
    include/renderstack_toolkit/file.hpp
    include/renderstack_toolkit/log.hpp
    include/renderstack_toolkit/gl.hpp
    include/renderstack_toolkit/strong_gl_enums.hpp
    include/renderstack_toolkit/window.hpp
    include/renderstack_toolkit/math_util.hpp
    include/renderstack_toolkit/platform.hpp
    include/renderstack_toolkit/enable_shared_from_this.hpp
    include/renderstack_toolkit/smart_ptr_builder.hpp
    include/renderstack_toolkit/service.hpp
    include/renderstack_toolkit/services.hpp
    source/log.cpp
    source/math_util.cpp
    source/file.cpp
    source/gl_core.cpp
    source/service.cpp
    source/services.cpp
)

if (RENDERSTACK_USE_GLFW)
    list(APPEND sources source/glfw_window.cpp)
endif()

if (RENDERSTACK_USE_GLWT)
    list(APPEND sources source/glwt_window.cpp)
endif ()

if (RENDERSTACK_GL_API_OPENGL_ES_2)
    list(APPEND sources source/gles_ext.cpp)
endif ()

# list(APPEND sources include/renderstack_toolkit/dynamic_opengl_es3.hpp)

if (RENDERSTACK_GL_API_OPENGL)
    list(APPEND sources include/renderstack_toolkit/glext.hpp source/glext.cpp)
endif ()

include_directories(
    ${RENDERSTACK_WINDOW_TOOLKIT_INCLUDE_DIRS}
    ${GLM_INCLUDE_DIRS}
    ${KHRONOS_INCLUDE}
    ${CMAKE_SOURCE_DIR}/libraries/renderstack_toolkit/include
)

add_library(
    renderstack_toolkit STATIC
    ${sources}
    ${extra_sources}
)

set_target_properties(renderstack_toolkit PROPERTIES
                      CXX_STANDARD 17
                      CXX_STANDARD_REQUIRED ON
                      COMPILE_FLAGS "${WARNING_FLAGS}")

if (CLANG_TIDY_EXE)
    set_target_properties(renderstack_toolkit PROPERTIES CXX_CLANG_TIDY "${DO_CLANG_TIDY}")
endif()

source_group("RenderStack Toolkit Sources"
             REGULAR_EXPRESSION
             "include/renderstack_toolkit/.*\\.inl|include/renderstack_toolkit/.*\\.hpp|source/.*\\.cpp")

install(TARGETS renderstack_toolkit DESTINATION lib)

