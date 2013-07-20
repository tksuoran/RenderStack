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

set(Boost_USE_STATIC_LIBS OFF)
set(Boost_USE_MULTITHREADED ON) 
set(Boost_USE_STATIC_RUNTIME OFF)
find_package(Boost 1.53.0)

if (Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIRS})
    # add_executable(progname file1.cxx file2.cxx)
    # target_link_libraries(progname ${Boost_LIBRARIES})                                                                                                                                                                                                                            
endif()

set(RENDERSTACK_GL_API "OpenGL" CACHE STRING "OpenGL API version")
set_property(CACHE RENDERSTACK_GL_API PROPERTY STRINGS "OpenGL" "OpenGL ES 2" "OpenGL ES 3")
if (RENDERSTACK_GL_API STREQUAL "OpenGL")
   set(RENDERSTACK_GL_API_OPENGL 1)
   unset(RENDERSTACK_GL_API_OPENGL_ES_1)
   unset(RENDERSTACK_GL_API_OPENGL_ES_2)
   unset(RENDERSTACK_GL_API_OPENGL_ES_3)
   add_definitions(-DRENDERSTACK_GL_API_OPENGL=1)
elseif (RENDERSTACK_GL_API STREQUAL "OpenGL ES 2")
   unset(RENDERSTACK_GL_API_OPENGL)
   unset(RENDERSTACK_GL_API_OPENGL_ES_1)
   set(RENDERSTACK_GL_API_OPENGL_ES_2 1)
   unset(RENDERSTACK_GL_API_OPENGL_ES_3)
   add_definitions(-DRENDERSTACK_GL_API_OPENGL_ES_2=1)
   # add_definitions(-DRENDERSTACK_DLOAD_ALL_GL_SYMBOLS=1)
   set(RENDERSTACK_USE_EGL 1)
elseif (RENDERSTACK_GL_API STREQUAL "OpenGL ES 3")
   unset(RENDERSTACK_GL_API_OPENGL)
   unset(RENDERSTACK_GL_API_OPENGL_ES_1)
   unset(RENDERSTACK_GL_API_OPENGL_ES_2)
   set(RENDERSTACK_GL_API_OPENGL_ES_3 1)
   add_definitions(-DRENDERSTACK_GL_API_OPENGL_ES_3=1)
   # add_definitions(-DRENDERSTACK_DLOAD_ALL_GL_SYMBOLS=1)
   set(RENDERSTACK_USE_EGL 1)
else ()
   message(FATAL_ERROR "Unsupported RENDERSTACK_WINDOW_TOOLKIT")
endif ()

# if (RENDERSTACK_USE_EGL)
#    add_definitions(-DKHRONOS_SUPPORT_INT64=1)
#    message("egl")
# endif ()

option(RENDERSTACK_USE_FREETYPE "Use Freetype" TRUE)

# Slightly hacky
set(GLM_INCLUDE_DIRS "${CMAKE_SOURCE_DIR}/libraries/glm-0.9.4.4")
set(TINYXML_INCLUDE_DIRS "${CMAKE_SOURCE_DIR}/libraries/tinyxml2")


include("ExportFile.cmake") # This is used by executables to copy data files

# include_directories(SYSTEM ${CMAKE_SOURCE_DIR}/libraries/khronos)

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
   if (RENDERSTACK_GL_API_OPENGL)
	  add_definitions(-DRENDERSTACK_DLOAD_WINDOWS_GL_SYMBOLS=1)
   endif ()
else ()
   message(FATAL_ERROR "Platform not supported.")
endif ()

if (RENDERSTACK_USE_FREETYPE)
   if (MSVC)
      set(FREETYPE_FOUND TRUE)
      set(FREETYPE_INCLUDE_DIR_ft2build "${CMAKE_SOURCE_DIR}/libraries/freetype-2.5.0/include")
      set(FREETYPE_INCLUDE_DIR_freetype2 "${CMAKE_SOURCE_DIR}/libraries/freetype-2.5.0/include/freetype")
      set(FREETYPE_INCLUDE_DIRS "${FREETYPE_INCLUDE_DIR_ft2build};${FREETYPE_INCLUDE_DIR_freetype2}")
      include_external_msproject(
         freetype-2.5.0
         ${CMAKE_SOURCE_DIR}/libraries/freetype-2.5.0/builds/win32/vc2010/freetype.vcxproj
      )
   else ()
      find_package(Freetype REQUIRED)
   endif ()
   add_definitions(-DRENDERSTACK_USE_FREETYPE)
endif ()

if (MSVC OR (WIN32 AND CMAKE_C_COMPILER_ID MATCHES "Intel"))
   add_definitions(-D_VARIADIC_MAX=8)

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

unset(KHRONOS_INCLUDE)

if (RENDERSTACK_USE_GLFW)
   option(GLFW_BUILD_EXAMPLES "Build the GLFW example programs" OFF)
   option(GLFW_BUILD_TESTS    "Build the GLFW test programs" OFF)
   option(BUILD_SHARED_LIBS   "Build shared libraries" OFF)
   add_definitions(-DGLFW_NO_GLU)
   add_subdirectory(libraries/glfw)
   add_definitions(-DRENDERSTACK_USE_GLFW=1)
   set(RENDERSTACK_WINDOW_TOOLKIT_INCLUDE_DIRS "${CMAKE_SOURCE_DIR}/libraries/glfw/include")
   list(APPEND RENDERSTACK_WINDOW_TOOLKIT_LIBRARIES glfw;${GLFW_LIBRARIES})
   if (RENDERSTACK_USE_EGL)
      set(KHRONOS_INCLUDE ${EGL_INCLUDE_DIR};${KHRONOS_INCLUDE})
		set(RENDERSTACK_WINDOW_TOOLKIT_LIBRARIES "${EGL_LIBRARY}";${RENDERSTACK_WINDOW_TOOLKIT_LIBRARIES})
   endif ()
   if (RENDERSTACK_GL_API_OPENGL)
      add_definitions(-DGLFW_INCLUDE_GLCOREARB -DGLCOREARB_PROTOTYPES)
		set(KHRONOS_INCLUDE ${CMAKE_SOURCE_DIR}/libraries/khronos;${KHRONOS_INCLUDE})
   endif ()
   if (RENDERSTACK_GL_API_OPENGL_ES_2)
      add_definitions(-DGLFW_INCLUDE_ES2)
      set(KHRONOS_INCLUDE ${GLESv2_INCLUDE_DIR};${KHRONOS_INCLUDE})
		set(RENDERSTACK_WINDOW_TOOLKIT_LIBRARIES "${GLESv2_LIBRARY}";${RENDERSTACK_WINDOW_TOOLKIT_LIBRARIES})
   endif ()
   if (RENDERSTACK_GL_API_OPENGL_ES_3)
      add_definitions(-DGLFW_INCLUDE_ES3)
      set(KHRONOS_INCLUDE ${GLESv3_INCLUDE_DIR};${KHRONOS_INCLUDE})
	  set(RENDERSTACK_WINDOW_TOOLKIT_LIBRARIES "${GLESv3_LIBRARY}";${RENDERSTACK_WINDOW_TOOLKIT_LIBRARIES})
	  message(${RENDERSTACK_WINDOW_TOOLKIT_LIBRARIES})
   endif ()
endif()

if (RENDERSTACK_USE_GLWT)
   add_subdirectory(libraries/glwt)
   set(RENDERSTACK_WINDOW_TOOLKIT_INCLUDE_DIRS "${glwt_SOURCE_DIR}/include;${glxw_BINARY_DIR}/include")
   add_definitions(-DRENDERSTACK_USE_GLWT=1)
   set(RENDERSTACK_WINDOW_TOOLKIT_LIBRARIES glwt)
   if (RENDERSTACK_USE_EGL)
      set(RENDERSTACK_WINDOW_TOOLKIT_LIBRARIES glxw_egl;${RENDERSTACK_WINDOW_TOOLKIT_LIBRARIES})
      set(KHRONOS_INCLUDE ${GLXW_EGL_PATH};${KHRONOS_INCLUDE})
      find_path(
         EGL_LIBRARY
         libEGL.lib
         PATHS
         "C:/Imagination/PowerVR/GraphicsSDK/SDK_3.1/Builds/Windows/x86_32/Lib/libEGL.lib"
         "C:/Program Files (x86)/ARM/Mali Developer Tools/OpenGL ES 3.0 Emulator v1.0.0/lib/32-bit/libEGL.lib"
         "C:/AdrenoSDK/Lib/Win32/OGLES3/libEGL.lib"
      )
      set(RENDERSTACK_WINDOW_TOOLKIT_LIBRARIES "${EGL_LIBRARY}";${RENDERSTACK_WINDOW_TOOLKIT_LIBRARIES})
   endif ()
   if (RENDERSTACK_GL_API_OPENGL)
      add_definitions(-DRENDERSTACK_DLOAD_ALL_GL_SYMBOLS)
      add_definitions(-DGLFW_INCLUDE_GLCOREARB -DGLCOREARB_PROTOTYPES)
      set(KHRONOS_INCLUDE ${CMAKE_SOURCE_DIR}/libraries/khronos;${KHRONOS_INCLUDE})
   endif ()
   if (RENDERSTACK_GL_API_OPENGL_ES_2)
      find_path(
         GLESv2_LIBRARY
         libGLESv2.lib
         PATHS
         "C:/Imagination/PowerVR/GraphicsSDK/SDK_3.1/Builds/Windows/x86_32/Lib/libGLESv2.lib"
         "C:/Program Files (x86)/ARM/Mali Developer Tools/OpenGL ES 3.0 Emulator v1.0.0/lib/32-bit/libGLESv2.lib"
         "C:/AdrenoSDK/Lib/Win32/OGLES3/libGLESv2.lib"
      )
      if (GLESv2_LIBRARY)
         set(RENDERSTACK_WINDOW_TOOLKIT_LIBRARIES "${GLESv2_LIBRARY}";${RENDERSTACK_WINDOW_TOOLKIT_LIBRARIES})
      endif()
   endif ()
   if (RENDERSTACK_GL_API_OPENGL_ES_3)
      find_path(
         GLESv3_LIBRARY
         libGLESv2.lib
         PATHS
         "C:/Imagination/PowerVR/GraphicsSDK/SDK_3.1/Builds/Windows/x86_32/Lib/libGLESv2.lib"
         "C:/Program Files (x86)/ARM/Mali Developer Tools/OpenGL ES 3.0 Emulator v1.0.0/lib/32-bit/libGLESv2.lib"
         "C:/AdrenoSDK/Lib/Win32/OGLES3/libGLESv2.lib"
      )
      if (GLESv3_LIBRARY)
         set(RENDERSTACK_WINDOW_TOOLKIT_LIBRARIES "${GLESv3_LIBRARY}";${RENDERSTACK_WINDOW_TOOLKIT_LIBRARIES})
      endif()
   endif ()
endif()


if (RENDERSTACK_SUPPORT_LEGACY_OPENGL)
   add_definitions(-DRENDERSTACK_GL_API_OPENGL_WITH_LEGACY=1)
endif ()

add_subdirectory(libraries)
add_subdirectory(executables)
