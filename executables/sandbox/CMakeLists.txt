# CMakeLists.txt for RenderStack/executables/sandbox

# message("RENDERSTACK_WINDOW_TOOLKIT_LIBRARIES = ${RENDERSTACK_WINDOW_TOOLKIT_LIBRARIES}")
# message("FREETYPE_LIBRARIES = ${FREETYPE_LIBRARIES}")

include_directories(
    ${KHRONOS_INCLUDE}
    ${CMAKE_SOURCE_DIR}/libraries/SimpleJSON
    ${PNG_INCLUDE_DIRS}
    ${ZLIB_INCLUDE_DIRS}
)

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${RENDERSTACK_WINDOW_TOOLKIT_INCLUDE_DIRS}
    ${GLM_INCLUDE_DIRS}
    ${CMAKE_SOURCE_DIR}/libraries/renderstack_geometry/include
    ${CMAKE_SOURCE_DIR}/libraries/renderstack_graphics/include
    ${CMAKE_SOURCE_DIR}/libraries/renderstack_mesh/include
    ${CMAKE_SOURCE_DIR}/libraries/renderstack_renderer/include
    ${CMAKE_SOURCE_DIR}/libraries/renderstack_scene/include
    ${CMAKE_SOURCE_DIR}/libraries/renderstack_toolkit/include
    ${CMAKE_SOURCE_DIR}/libraries/renderstack_ui/include
)

# file(GLOB_RECURSE RELATIVE res res/*)
# file(GLOB res RELATIVE res/*)
# file(GLOB_RECURSE variable [RELATIVE path] [FOLLOW_SYMLINKS] [globbing expressions]...)

file(GLOB_RECURSE res_images     RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}" "${CMAKE_CURRENT_SOURCE_DIR}/res/images/*")
file(GLOB_RECURSE res_fonts      RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}" "${CMAKE_CURRENT_SOURCE_DIR}/res/fonts/*")
file(GLOB_RECURSE res_polyhedra  RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}" "${CMAKE_CURRENT_SOURCE_DIR}/res/polyhedra/*")
file(GLOB_RECURSE res_shaders    RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}" "${CMAKE_CURRENT_SOURCE_DIR}/res/shaders/*")

foreach (r ${res_images} ${res_fonts} ${res_polyhedra} ${res_shaders})
    cm_export_file(${CMAKE_CURRENT_SOURCE_DIR}/${r} ${CMAKE_CURRENT_BINARY_DIR}/${r} sandbox)
endforeach(r)

file(WRITE ${CMAKE_CURRENT_BINARY_DIR}/res/src_path.txt "${CMAKE_CURRENT_SOURCE_DIR}")
file(WRITE ${CMAKE_CURRENT_BINARY_DIR}/res/dst_path.txt "${CMAKE_CURRENT_BINARY_DIR}")

add_executable(
    sandbox

    main/main.cpp
    main/application.cpp
    main/application.hpp
    main/screen.hpp
    main/menu.cpp
    main/menu.hpp
    main/game.cpp
    main/game.hpp
    main/game_render.cpp
    main/input.cpp
    main/on_load.cpp
    main/programs.cpp
    main/programs.hpp
    main/textures.cpp
    main/textures.hpp
    main/update.cpp
    main/log.cpp
    main/log.hpp

    logic/manipulator_manager.cpp
    logic/manipulator_manager.hpp
    logic/manipulator.cpp
    logic/manipulator.hpp
    logic/reference_frame.cpp
    logic/reference_frame.hpp
    logic/selection_manager.cpp
    logic/selection_manager.hpp

    scene/model.cpp
    scene/model.hpp
    scene/scene_manager.cpp
    scene/scene_manager.hpp
    scene/material_manager.cpp
    scene/material_manager.hpp
    scene/material.cpp
    scene/material.hpp

    renderers/base_renderer.cpp
    renderers/base_renderer.hpp
    renderers/deferred_renderer.cpp
    renderers/deferred_renderer.hpp
    renderers/forward_renderer.cpp
    renderers/forward_renderer.hpp
    renderers/light_debug_renderer.cpp
    renderers/light_debug_renderer.hpp
    renderers/light_mesh.cpp
    renderers/light_mesh.hpp
    renderers/id_renderer.cpp
    renderers/id_renderer.hpp
    renderers/debug_renderer.cpp
    renderers/debug_renderer.hpp
    renderers/quad_renderer.cpp
    renderers/quad_renderer.hpp

    parsers/netlib_polyhedron.cpp
    parsers/netlib_polyhedron.hpp
    parsers/json_polyhedron.cpp
    parsers/json_polyhedron.hpp

    util/aabb.cpp
    util/aabb.hpp
    util/bounding_box.hpp
    util/controller.hpp
    util/controller.cpp
    util/frame_controller.cpp
    util/frame_controller.hpp

    ${res_images}
    ${res_fonts}
    ${res_polyhedra}
    ${res_shaders}

    icon.rc
)

foreach(dir "main" "logic" "scene" "renderers" "parsers" "util")
    source_group("${dir}" REGULAR_EXPRESSION "${dir}/.*\\.hpp|${dir}/.*\\.cpp")
endforeach() 
source_group("Source Files" REGULAR_EXPRESSION ".hpp|.cpp")

source_group(res\\images      FILES ${res_images})
source_group(res\\fonts       FILES ${res_fonts})
source_group(res\\polyhedra   FILES ${res_polyhedra})
source_group(res\\shaders     FILES ${res_shaders})


#message("PNG_LIBRARIES = ${PNG_LIBRARIES}")
#message("RENDERSTACK_WINDOW_TOOLKIT_LIBRARIES = ${RENDERSTACK_WINDOW_TOOLKIT_LIBRARIES}")
#message("TINYXML_LIBS = ${TINYXML_LIBS}")

target_link_libraries(
     sandbox
     PRIVATE
     renderstack_ui 
     renderstack_mesh
     renderstack_graphics
     renderstack_geometry
     renderstack_scene
     renderstack_toolkit
     RectangleBinPack
     ${RENDERSTACK_WINDOW_TOOLKIT_LIBRARIES}
     ${PNG_LIBRARIES}
)

if (RENDERSTACK_GL_API_OPENGL)
    target_link_libraries(sandbox PRIVATE OpenGL::OpenGL)
endif ()


if (RENDERSTACK_USE_FREETYPE)
    if (MSVC)
        add_dependencies(sandbox freetype2)
        set_target_properties(
            sandbox PROPERTIES
            LINK_FLAGS "/ENTRY:mainCRTStartup"
        )
    else ()
        target_link_libraries(sandbox PRIVATE ${FREETYPE_LIBRARIES})
    endif()
endif()
