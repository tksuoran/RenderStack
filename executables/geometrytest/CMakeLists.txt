# CMakeLists.txt for RenderStack/executables/geometrytest

# message("RENDERSTACK_WINDOW_TOOLKIT_LIBRARIES = ${RENDERSTACK_WINDOW_TOOLKIT_LIBRARIES}")
# message("FREETYPE_LIBRARIES = ${FREETYPE_LIBRARIES}")

include_directories(
    SYSTEM ${CMAKE_SOURCE_DIR}/libraries/khronos
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

file(GLOB_RECURSE res_images RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}" "${CMAKE_CURRENT_SOURCE_DIR}/res/images/*")
file(GLOB_RECURSE res_fonts RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}" "${CMAKE_CURRENT_SOURCE_DIR}/res/fonts/*")
file(GLOB_RECURSE res_sm0 RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}" "${CMAKE_CURRENT_SOURCE_DIR}/res/shaders/sm0/*")
file(GLOB_RECURSE res_sm4 RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}" "${CMAKE_CURRENT_SOURCE_DIR}/res/shaders/sm4/*")
file(GLOB_RECURSE res_sm5 RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}" "${CMAKE_CURRENT_SOURCE_DIR}/res/shaders/sm5/*")

foreach (r ${res_images} ${res_fonts} ${res_sm0} ${res_sm4} ${res_sm5})
   cm_export_file(${CMAKE_CURRENT_SOURCE_DIR}/${r} ${CMAKE_CURRENT_BINARY_DIR}/${r} geometrytest)
endforeach(r)
message(STATUS ${CMAKE_CURRENT_BINARY_DIR}/res/source.txt "${CMAKE_CURRENT_SOURCE_DIR}/res")
file(WRITE ${CMAKE_CURRENT_BINARY_DIR}/res/src_path.txt "${CMAKE_CURRENT_SOURCE_DIR}/res")
file(WRITE ${CMAKE_CURRENT_BINARY_DIR}/res/dst_path.txt "${CMAKE_CURRENT_BINARY_DIR}/res")

add_executable(
   geometrytest
   util/aabb.cpp
   util/aabb.hpp
   util/bounding_box.hpp
   util/controller.hpp
   util/controller.cpp
   util/frame_controller.cpp
   util/frame_controller.hpp
   renderer/model.cpp
   renderer/model.hpp
   main/application.cpp
   main/application.hpp
   main/game.cpp
   main/game.hpp
   main/game_render.cpp
   main/input.cpp
   main/menu.cpp
   main/menu.hpp
   main/on_load.cpp
   main/programs.cpp
   main/programs.hpp
   main/screen.hpp
   main/tests.cpp
   main/textures.cpp
   main/textures.hpp
   main/update.cpp
   ${res_images}
   ${res_fonts}
   ${res_sm0}
   ${res_sm4}
   ${res_sm5}
)

foreach(dir "renderer" "main" "util")
   source_group("${dir}" REGULAR_EXPRESSION "${dir}/.*\\.hpp|${dir}/.*\\.cpp")
endforeach() 
source_group("Source Files" REGULAR_EXPRESSION ".hpp|.cpp")

source_group(res\\images FILES ${res_images})
source_group(res\\fonts FILES ${res_fonts})
source_group(res\\shaders\\sm5 FILES ${res_sm5})
source_group(res\\shaders\\sm4 FILES ${res_sm4})
source_group(res\\shaders\\sm0 FILES ${res_sm0})

target_link_libraries(
   geometrytest
   renderstack_ui 
   renderstack_renderer
   renderstack_mesh
   renderstack_graphics
   renderstack_geometry
   renderstack_scene
   renderstack_toolkit
   RectangleBinPack
   ${RENDERSTACK_WINDOW_TOOLKIT_LIBRARIES}
)

if (RENDERSTACK_USE_FREETYPE)
   if (MSVC)
      add_dependencies(geometrytest freetype-2.4.9)
      set_target_properties(
         geometrytest PROPERTIES
         LINK_FLAGS "/ENTRY:mainCRTStartup"
      )
   else ()
      target_link_libraries(geometrytest ${FREETYPE_LIBRARIES})
   endif()
endif()
