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

set(RENDERSTACK_WINDOW_TOOLKIT_INCLUDE_DIRS "")

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
#     add_definitions(-DKHRONOS_SUPPORT_INT64=1)
#     message("egl")
# endif ()


option(RENDERSTACK_USE_FREETYPE "Use Freetype" TRUE)
option(RENDERSTACK_3DMOUSE_SUPPORT "3D Connexion 3D Mouse Support (experimental)" FALSE)
if (RENDERSTACK_3DMOUSE_SUPPORT)
    add_definitions(-DRENDERSTACK_3DMOUSE_SUPPORT=1)
endif ()

# Slightly hacky
set(GLM_INCLUDE_DIRS "${CMAKE_SOURCE_DIR}/libraries/glm-0.9.7.2")
set(TINYXML_INCLUDE_DIRS "${CMAKE_SOURCE_DIR}/libraries/tinyxml2")
set(TINYXML_LIBS tinyxml2)

# libpng requires some hacking due to dependency to zlib
if (MSVC)
    set(PNG_INCLUDE_DIRS "${CMAKE_SOURCE_DIR}/libraries/lpng1621" "${CMAKE_BINARY_DIR}/libraries/lpng1621")
    set(PNG_LIBRARIES png16_static zlibstatic)
    
    # Prevent libpng from trying to install anything
    set(SKIP_INSTALL_ALL TRUE)

    # Pre-override libpng cmake configuration to suit MSVC builds
    set(PNG_SHARED FALSE)
    set(PNG_STATIC TRUE)
    option(PNG_TESTS "Build libpng tests" NO)

    # Tell libpng that we have zlib
    set(ZLIB_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/libraries/zlib")
    set(ZLIB_LIBRARY zlib)
    if (ZLIB_INCLUDE_DIR AND EXISTS "${ZLIB_INCLUDE_DIR}/zlib.h")
        file(STRINGS "${ZLIB_INCLUDE_DIR}/zlib.h" ZLIB_H REGEX "^#define ZLIB_VERSION \"[^\"]*\"$")

        string(REGEX REPLACE "^.*ZLIB_VERSION \"([0-9]+).*$" "\\1" ZLIB_VERSION_MAJOR "${ZLIB_H}")
        string(REGEX REPLACE "^.*ZLIB_VERSION \"[0-9]+\\.([0-9]+).*$" "\\1" ZLIB_VERSION_MINOR  "${ZLIB_H}")
        string(REGEX REPLACE "^.*ZLIB_VERSION \"[0-9]+\\.[0-9]+\\.([0-9]+).*$" "\\1" ZLIB_VERSION_PATCH "${ZLIB_H}")
        set(ZLIB_VERSION_STRING "${ZLIB_VERSION_MAJOR}.${ZLIB_VERSION_MINOR}.${ZLIB_VERSION_PATCH}")

        # only append a TWEAK version if it exists:
        set(ZLIB_VERSION_TWEAK "")
        if ("${ZLIB_H}" MATCHES "^.*ZLIB_VERSION \"[0-9]+\\.[0-9]+\\.[0-9]+\\.([0-9]+).*$")
            set(ZLIB_VERSION_TWEAK "${CMAKE_MATCH_1}")
            set(ZLIB_VERSION_STRING "${ZLIB_VERSION_STRING}.${ZLIB_VERSION_TWEAK}")
        endif ()

        set(ZLIB_MAJOR_VERSION "${ZLIB_VERSION_MAJOR}")
        set(ZLIB_MINOR_VERSION "${ZLIB_VERSION_MINOR}")
        set(ZLIB_PATCH_VERSION "${ZLIB_VERSION_PATCH}")
    endif ()
    set(ZLIB_INCLUDE_DIR "${ZLIB_INCLUDE_DIR}" "${CMAKE_BINARY_DIR}/libraries/zlib")
    set(ZLIB_INCLUDE_DIRS "${ZLIB_INCLUDE_DIR}")
    set(ZLIB_LIBRARIES ${ZLIB_LIBRARY})
    set(ZLIB_FOUND TRUE)

    add_subdirectory(libraries/zlib)
    add_subdirectory(libraries/lpng1621)
else ()
    find_package(PNG REQUIRED)
    find_package(ZLIB REQUIRED)
endif ()


include("ExportFile.cmake") # This is used by executables to copy data files

# include_directories(SYSTEM ${CMAKE_SOURCE_DIR}/libraries/khronos)

# Not supported yet
# add_definitions(-DSUPPORT_LEGACY_OPENGL=1)

# Compiler options
set (CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g -fno-omit-frame-pointer -fsanitize=address,undefined")
set (CMAKE_LINKER_FLAGS_DEBUG "${CMAKE_LINKER_FLAGS_DEBUG} -fno-omit-frame-pointer -fsanitize=address,undefined")

if (APPLE)
    set(CMAKE_XCODE_ATTRIBUTE_CLANG_CXX_LANGUAGE_STANDARD "c++17")
    set(CMAKE_XCODE_ATTRIBUTE_CLANG_CXX_LIBRARY "libc++")
elseif (CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -g -Wall")
elseif (CMAKE_CXX_COMPILER MATCHES ".*clang")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -g -Wall")
elseif (MSVC)
    add_definitions(/W4 /wd4201)
    if (RENDERSTACK_GL_API_OPENGL)
        add_definitions(-DRENDERSTACK_DLOAD_WINDOWS_GL_SYMBOLS=1)
    endif ()
else ()
    message(FATAL_ERROR "Platform not supported.")
endif ()

if (RENDERSTACK_USE_FREETYPE)
    set(FREETYPE_ROOT_DIR "${CMAKE_SOURCE_DIR}/libraries/freetype-2.10.0")
    add_subdirectory(libraries/freetype-2.10.0)
    set(FREETYPE_FOUND TRUE)
    set(FREETYPE_INCLUDE_DIR_ft2build "${FREETYPE_ROOT_DIR}/include")
    set(FREETYPE_INCLUDE_DIR_freetype2 "${FREETYPE_ROOT_DIR}/include/freetype")
    set(FREETYPE_INCLUDE_DIRS "${FREETYPE_INCLUDE_DIR_ft2build}" "${FREETYPE_INCLUDE_DIR_freetype2}")
    set(FREETYPE_LIBRARIES "freetype")

    #if (MSVC)
        #set(FREETYPE_FOUND TRUE)
        #set(FREETYPE_INCLUDE_DIR_ft2build "${FREETYPE_ROOT_DIR}/include")
        #set(FREETYPE_INCLUDE_DIR_freetype2 "${FREETYPE_ROOT_DIR}/include/freetype")
        #set(FREETYPE_INCLUDE_DIRS "${FREETYPE_INCLUDE_DIR_ft2build}" "${FREETYPE_INCLUDE_DIR_freetype2}")
        #if (MSVC14)
        #    include_external_msproject(freetype2 ${FREETYPE_ROOT_DIR}/builds/windows/vc2015/freetype.vcxproj)
        #else ()
        #     message(FATAL_ERROR "MSVC14 required.")
        #endif ()
    #else ()
    #    find_package(Freetype REQUIRED)
    #endif ()
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
        set(CMAKE_C_FLAGS    "${CMAKE_C_FLAGS} /GS-")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /GS-")
    endif ()
endif ()

unset(KHRONOS_INCLUDE)

if (RENDERSTACK_USE_EGL)
    list(APPEND KHRONOS_INCLUDE ${EGL_INCLUDE_DIR})
    list(APPEND RENDERSTACK_WINDOW_TOOLKIT_LIBRARIES ${EGL_LIBRARY})
endif ()

if (RENDERSTACK_GL_API_OPENGL)
    add_definitions(-DGLCOREARB_PROTOTYPES)
    list(APPEND KHRONOS_INCLUDE "${CMAKE_SOURCE_DIR}/libraries/khronos")
    find_package(OpenGL REQUIRED COMPONENTS OpenGL OPTIONAL_COMPONENTS EGL GLX)
endif ()

if (RENDERSTACK_GL_API_OPENGL_ES_2)
    list(APPEND KHRONOS_INCLUDE ${GLESv2_INCLUDE_DIR})
    list(APPEND RENDERSTACK_WINDOW_TOOLKIT_LIBRARIES ${GLESv2_LIBRARY})
endif ()

if (RENDERSTACK_GL_API_OPENGL_ES_3)
    list(APPEND KHRONOS_INCLUDE ${GLESv3_INCLUDE_DIR})
    list(APPEND RENDERSTACK_WINDOW_TOOLKIT_LIBRARIES ${GLESv3_LIBRARY})
endif ()

if (RENDERSTACK_USE_GLFW)
    option(GLFW_BUILD_EXAMPLES "Build the GLFW example programs" OFF)
    option(GLFW_BUILD_TESTS    "Build the GLFW test programs" OFF)
    option(GLFW_BUILD_DOCS     "Build the GLFW documentation" OFF)
    option(BUILD_SHARED_LIBS   "Build shared libraries" OFF)
    add_definitions(-DGLFW_NO_GLU)
    add_subdirectory(libraries/glfw)
    add_definitions(-DRENDERSTACK_USE_GLFW=1)
    list(APPEND RENDERSTACK_WINDOW_TOOLKIT_INCLUDE_DIRS "${CMAKE_SOURCE_DIR}/libraries/glfw/include")
    list(APPEND RENDERSTACK_WINDOW_TOOLKIT_LIBRARIES "glfw" ${GLFW_LIBRARIES})
    if (RENDERSTACK_GL_API_OPENGL)
        add_definitions(-DGLFW_INCLUDE_GLCOREARB)
    endif ()
    if (RENDERSTACK_GL_API_OPENGL_ES_2)
        add_definitions(-DGLFW_INCLUDE_ES2)
    endif ()
    if (RENDERSTACK_GL_API_OPENGL_ES_3)
        add_definitions(-DGLFW_INCLUDE_ES3)
    endif ()
endif()

if (RENDERSTACK_USE_GLWT)
    add_subdirectory(libraries/glwt)
    list(APPEND RENDERSTACK_WINDOW_TOOLKIT_INCLUDE_DIRS "${glwt_SOURCE_DIR}/include" "${glxw_BINARY_DIR}/include")
    add_definitions(-DRENDERSTACK_USE_GLWT=1)
    set(RENDERSTACK_WINDOW_TOOLKIT_LIBRARIES glwt)
    if (RENDERSTACK_USE_EGL)
        list(APPEND RENDERSTACK_WINDOW_TOOLKIT_LIBRARIES "glxw_egl")
        list(APPEND KHRONOS_INCLUDE ${GLXW_EGL_PATH})
        find_path(
            EGL_LIBRARY
            libEGL.lib
            PATHS
            "C:/Imagination/PowerVR/GraphicsSDK/SDK_3.1/Builds/Windows/x86_32/Lib/libEGL.lib"
            "C:/Program Files (x86)/ARM/Mali Developer Tools/OpenGL ES 3.0 Emulator v1.0.0/lib/32-bit/libEGL.lib"
            "C:/AdrenoSDK/Lib/Win32/OGLES3/libEGL.lib"
        )
        list(APPEND RENDERSTACK_WINDOW_TOOLKIT_LIBRARIES "${EGL_LIBRARY}")
    endif ()
    if (RENDERSTACK_GL_API_OPENGL)
        add_definitions(-DRENDERSTACK_DLOAD_ALL_GL_SYMBOLS)
        add_definitions(-DGLFW_INCLUDE_GLCOREARB -DGLCOREARB_PROTOTYPES)
        list(APPEND KHRONOS_INCLUDE ${CMAKE_SOURCE_DIR}/libraries/khronos)
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
            list(APPEND RENDERSTACK_WINDOW_TOOLKIT_LIBRARIES ${GLESv2_LIBRARY})
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
            list(APPEND RENDERSTACK_WINDOW_TOOLKIT_LIBRARIES ${GLESv3_LIBRARY})
        endif()
    endif ()
endif()


if (RENDERSTACK_SUPPORT_LEGACY_OPENGL)
    add_definitions(-DRENDERSTACK_GL_API_OPENGL_WITH_LEGACY=1)
endif ()

add_subdirectory(libraries)
add_subdirectory(executables)
