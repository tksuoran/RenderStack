# CMakeLists.txt for renderstack_ui

if (RENDERSTACK_USE_FREETYPE)
    #message("FREETYPE_INCLUDE_DIRS = ${FREETYPE_INCLUDE_DIRS}")
    include_directories(${FREETYPE_INCLUDE_DIRS})
endif ()

include_directories(
    ${RENDERSTACK_WINDOW_TOOLKIT_INCLUDE_DIRS}
    ${GLM_INCLUDE_DIRS}
    ${KHRONOS_INCLUDE}
    ${CMAKE_SOURCE_DIR}/libraries/RectangleBinPack
    ${CMAKE_SOURCE_DIR}/libraries/renderstack_geometry/include
    ${CMAKE_SOURCE_DIR}/libraries/renderstack_graphics/include
    ${CMAKE_SOURCE_DIR}/libraries/renderstack_mesh/include
    ${CMAKE_SOURCE_DIR}/libraries/renderstack_renderer/include
    ${CMAKE_SOURCE_DIR}/libraries/renderstack_toolkit/include
    ${CMAKE_SOURCE_DIR}/libraries/renderstack_ui/include
)

add_library(
    renderstack_ui STATIC
    include/renderstack_ui/action.hpp
    include/renderstack_ui/area.hpp
    include/renderstack_ui/bitmap.hpp
    include/renderstack_ui/button.hpp
    include/renderstack_ui/choice.hpp
    include/renderstack_ui/color_picker.hpp
    include/renderstack_ui/dock.hpp
    include/renderstack_ui/font.hpp
    include/renderstack_ui/glyph.hpp
    include/renderstack_ui/gui_renderer.hpp
    include/renderstack_ui/layer.hpp
    include/renderstack_ui/log.hpp
    include/renderstack_ui/menulist.hpp
    include/renderstack_ui/ninepatch.hpp
    include/renderstack_ui/ninepatch_style.hpp
    include/renderstack_ui/popup.hpp
    include/renderstack_ui/push_button.hpp
    include/renderstack_ui/rectangle.hpp
    include/renderstack_ui/slider.hpp
    include/renderstack_ui/style.hpp
    include/renderstack_ui/text_buffer.hpp
    source/area.cpp
    source/button.cpp
    source/choice.cpp
    source/color_picker.cpp
    source/dock.cpp
    source/font.cpp
    source/glyph.cpp
    source/gui_renderer.cpp
    source/log.cpp
    source/menulist.cpp
    source/ninepatch.cpp
    source/ninepatch_style.cpp
    source/popup.cpp
    source/push_button.cpp
    source/slider.cpp
    source/text_buffer.cpp
)

source_group(
    "RenderStack UI Sources"
    REGULAR_EXPRESSION
    "include/renderstack_ui/.*\\.inl|include/renderstack_ui/.*\\.hpp|source/.*\\.cpp")

install(TARGETS renderstack_ui DESTINATION lib)
