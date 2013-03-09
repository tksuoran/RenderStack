# CMakeLists.txt for RenderStack/

project(RenderStack C CXX)

cmake_minimum_required(VERSION 2.8.0)

# Warn users
if ( MAKE_SOURCE_DIR STREQUAL CMAKE_BINARY_DIR AND NOT MSVC_IDE )
   message(FATAL_ERROR "In-source builds are not allowed.
Please create a directory and run cmake from there, passing the path
to this source directory as the last argument.
This process created the file `CMakeCache.txt' and the directory `CMakeFiles'.
Please delete them.")
endif()

set(RENDERSTACK_WINDOW_TOOLKIT "GLFW" CACHE STRING "Window Toolkit")
set_property(CACHE RENDERSTACK_WINDOW_TOOLKIT PROPERTY STRINGS "GLFW" "GLWT")
if (RENDERSTACK_WINDOW_TOOLKIT STREQUAL "GLFW")
   set(RENDERSTACK_USE_GLFW 1)
   unset(RENDERSTACK_USE_GLWT)
elseif (RENDERSTACK_WINDOW_TOOLKIT STREQUAL "GLWT")
   set(RENDERSTACK_USE_GLWT 1)
   unset(RENDERSTACK_USE_GLFW)
else ()
   message(FATAL_ERROR "Unsupported RENDERSTACK_WINDOW_TOOLKIT")
endif ()

option(RENDERSTACK_USE_FREETYPE "Use Freetype" TRUE)
option(RENDERSTACK_SUPPORT_LEGACY_OPENGL "Support legacy OpenGL" FALSE)
if (RENDERSTACK_SUPPORT_LEGACY_OPENGL)
   add_definitions(-DRENDERSTACK_SUPPORT_LEGACY_OPENGL=1)
endif ()

if (NOT ANDROID)
   option(RENDERSTACK_USE_GLES2_OR_GLES3 "RenderStack: Use OpenGL ES 2 or 3" OFF)
else ()
   set(RENDERSTACK_USE_GLES2_OR_GLES3 ON)
endif ()

set(GLM_INCLUDE_DIRS "${CMAKE_SOURCE_DIR}/libraries/glm-0.9.4.2")

if (RENDERSTACK_USE_GLES2_OR_GLES3)
   add_definitions(-DRENDERSTACK_USE_GLES2_OR_GLES3 -DRENDERSTACK_USE_OPENGL_ES_ANY)
endif()

include("ExportFile.cmake") # This is used by executables to copy data files

include_directories(SYSTEM ${CMAKE_SOURCE_DIR}/libraries/khronos)

# Not supported yet
# add_definitions(-DSUPPORT_LEGACY_OPENGL=1)

# Compiler options
if (APPLE)
   set(CMAKE_XCODE_ATTRIBUTE_CLANG_CXX_LANGUAGE_STANDARD "c++11")
   set(CMAKE_XCODE_ATTRIBUTE_CLANG_CXX_LIBRARY "libc++")
elseif (CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX)
   set(CMAKE_CXX_FLAGS "${CMAKE_C_FLAGS} -std=c++0x -g -Wall")
elseif (CMAKE_CXX_COMPILER MATCHES ".*clang")
   set(CMAKE_CXX_FLAGS "${CMAKE_C_FLAGS} -std=c++11 -g -Wall")
elseif (MSVC)
   add_definitions(/W4 /wd4201)
else ()
   message(FATAL_ERROR "Platform not supported.")
endif()

if (RENDERSTACK_USE_FREETYPE)
   if (MSVC)
      set(FREETYPE_FOUND TRUE)
      set(FREETYPE_INCLUDE_DIR_ft2build "${CMAKE_SOURCE_DIR}/libraries/freetype-2.4.9/include")
      set(FREETYPE_INCLUDE_DIR_freetype2 "${CMAKE_SOURCE_DIR}/libraries/freetype-2.4.9/include/freetype")
      set(FREETYPE_INCLUDE_DIRS "${FREETYPE_INCLUDE_DIR_ft2build};${FREETYPE_INCLUDE_DIR_freetype2}")
      include_external_msproject(
         freetype-2.4.9
         ${CMAKE_SOURCE_DIR}/libraries/freetype-2.4.9/builds/win32/vc2010/freetype.vcxproj
      )
   else ()
      find_package(Freetype REQUIRED)
   endif ()
   add_definitions(-DRENDERSTACK_USE_FREETYPE)
endif ()

if (MSVC OR (WIN32 AND CMAKE_C_COMPILER_ID MATCHES "Intel"))
   add_definitions(-D_VARIADIC_MAX=7)

   option(RENDERSTACK_HAS_ITERATOR_DEBUGGING "MSVC Iterator Debugging" OFF)
   option(RENDERSTACK_DISABLE_BUFFER_CHECKS  "MSVC Disable Buffer Checks" ON)
  
   if (RENDERSTACK_NO_ITERATOR_DEBUGGING)
      add_definitions(-D_HAS_ITERATOR_DEBUGGING=0)
   endif ()

   if (NOT RENDERSTACK_HAS_ITERATOR_DEBUGGING)
      add_definitions(-D_SECURE_SCL)
      add_definitions(-D_SCL_SECURE_NO_WARNINGS)
      add_definitions(-D_HAS_ITERATOR_DEBUGGING=0)
      # message(WARNING "Iterator debugging disabled")
   endif ()

   if (RENDERSTACK_NO_BUFFER_CHECKS)
      set(CMAKE_C_FLAGS   "${CMAKE_C_FLAGS} /GS-")
      set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /GS-")
   endif ()
endif ()

if (RENDERSTACK_USE_GLFW)
   option(GLFW_BUILD_EXAMPLES "Build the GLFW example programs" OFF)
   option(GLFW_BUILD_TESTS    "Build the GLFW test programs" OFF)
   option(BUILD_SHARED_LIBS   "Build shared libraries" OFF)
   add_definitions(-DGLFW_NO_GLU -DGLFW_INCLUDE_GLCOREARB -DGLCOREARB_PROTOTYPES)
   add_subdirectory(libraries/glfw-3)
   add_definitions(-DRENDERSTACK_USE_GLFW=1)
   set(RENDERSTACK_WINDOW_TOOLKIT_INCLUDE_DIRS "${CMAKE_SOURCE_DIR}/libraries/glfw-3/include")
   set(RENDERSTACK_WINDOW_TOOLKIT_LIBRARIES "glfw;${GLFW_LIBRARIES}")
endif()

if (RENDERSTACK_USE_GLWT)
   add_subdirectory(libraries/glwt)
   set(RENDERSTACK_WINDOW_TOOLKIT_INCLUDE_DIRS "${glwt_SOURCE_DIR}/include;${glxw_BINARY_DIR}/include")
   add_definitions(-DRENDERSTACK_DLOAD_ALL_GL_SYMBOLS)
   add_definitions(-DRENDERSTACK_USE_GLWT=1)
   set(RENDERSTACK_WINDOW_TOOLKIT_LIBRARIES "glwt;glxw")
endif()


if (RENDERSTACK_SUPPORT_LEGACY_OPENGL)
   add_definitions(-DSUPPORT_LEGACY_OPENGL=1)
endif ()

add_subdirectory(libraries)
add_subdirectory(executables)
