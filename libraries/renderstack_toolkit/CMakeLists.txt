# CMakeLists.txt for renderstack_toolkit

set(
   sources
   include/renderstack_toolkit/gl_core.hpp
   include/renderstack_toolkit/file.hpp
   include/renderstack_toolkit/debugstream.hpp
   include/renderstack_toolkit/logstream.hpp
   include/renderstack_toolkit/gl.hpp
   include/renderstack_toolkit/strong_gl_enums.hpp
   include/renderstack_toolkit/window.hpp
   include/renderstack_toolkit/math_util.hpp
   include/renderstack_toolkit/lodepng.h
   include/renderstack_toolkit/platform.hpp
   include/renderstack_toolkit/enable_shared_from_this.hpp
   include/renderstack_toolkit/smart_ptr_builder.hpp
   source/logstream.cpp
   source/lodepng.cpp
   source/math_util.cpp
   source/file.cpp
   source/gl_core.cpp
)

if (RENDERSTACK_USE_GLFW)
   list(APPEND sources source/glfw_window.cpp)
endif()

if (RENDERSTACK_USE_GLWT)
   list(APPEND sources source/glwt_window.cpp)
endif()

if (RENDERSTACK_USE_GLES2_OR_GLES3)
   list(
      APPEND sources
      include/renderstack_toolkit/dynamic_opengl_es3.hpp
      source/gles3ext.cpp
   )
else ()
   list(
      APPEND sources
      include/renderstack_toolkit/glext.hpp
      source/glext.cpp
   )
endif ()

include_directories(
   ${RENDERSTACK_WINDOW_TOOLKIT_INCLUDE_DIRS}
   ${GLM_INCLUDE_DIRS}
   ${CMAKE_SOURCE_DIR}/libraries/khronos
   ${CMAKE_SOURCE_DIR}/libraries/renderstack_toolkit/include
)

add_library(
   renderstack_toolkit STATIC
   ${sources}
)

install(TARGETS renderstack_toolkit DESTINATION lib)

