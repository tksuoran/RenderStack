# CMakeLists.txt for renderstack_graphics

include_directories(
   ${RENDERSTACK_WINDOW_TOOLKIT_INCLUDE_DIRS}
   ${GLM_INCLUDE_DIRS}
   ${KHRONOS_INCLUDE}
   ${CMAKE_SOURCE_DIR}/libraries/renderstack_graphics/include
   ${CMAKE_SOURCE_DIR}/libraries/renderstack_toolkit/include
   ${PNG_INCLUDE_DIRS}
   ${ZLIB_INCLUDE_DIRS}
)

add_library(
   renderstack_graphics STATIC
   include/renderstack_graphics/buffer.hpp
   include/renderstack_graphics/configuration.hpp
   include/renderstack_graphics/log.hpp
   include/renderstack_graphics/program.hpp
   include/renderstack_graphics/sampler.hpp
   include/renderstack_graphics/samplers.hpp
   include/renderstack_graphics/shader_monitor.hpp
   include/renderstack_graphics/texture.hpp
   include/renderstack_graphics/uniform.hpp
   include/renderstack_graphics/uniform_block.hpp
   include/renderstack_graphics/uniform_buffer_range.hpp
   include/renderstack_graphics/vertex_attribute.hpp
   include/renderstack_graphics/vertex_format.hpp
   include/renderstack_graphics/vertex_array.hpp
   include/renderstack_graphics/vertex_stream.hpp
   include/renderstack_graphics/vertex_stream_binding.hpp
   include/renderstack_graphics/vertex_attribute_mapping.hpp
   include/renderstack_graphics/vertex_attribute_mappings.hpp
   include/renderstack_graphics/fragment_output.hpp
   include/renderstack_graphics/fragment_outputs.hpp

   include/renderstack_graphics/state_trackers.hpp
   include/renderstack_graphics/blend_state.hpp
   include/renderstack_graphics/depth_state.hpp
   include/renderstack_graphics/face_cull_state.hpp
   include/renderstack_graphics/color_mask_state.hpp
   include/renderstack_graphics/renderer.hpp
   include/renderstack_graphics/stencil_state.hpp
   include/renderstack_graphics/debug.hpp

   source/buffer.cpp
   source/configuration.cpp
   source/log.cpp
   source/program.cpp
   source/sampler.cpp
   source/samplers.cpp
   source/shader_monitor.cpp
   source/texture.cpp
   source/uniform.cpp
   source/uniform_block.cpp
   source/uniform_buffer_range.cpp
   source/vertex_attribute.cpp
   source/vertex_format.cpp
   source/vertex_array.cpp
   source/vertex_stream.cpp
   source/vertex_stream_binding.cpp
   source/vertex_attribute_mapping.cpp
   source/vertex_attribute_mappings.cpp
   source/fragment_output.cpp
   source/fragment_outputs.cpp

   source/state_trackers.cpp
   source/blend_state.cpp
   source/depth_state.cpp
   source/face_cull_state.cpp
   source/color_mask_state.cpp
   source/renderer.cpp
   source/stencil_state.cpp
   source/debug.cpp

   include/renderstack_graphics/load_png.hpp
   source/load_png.cpp

)

source_group(
   "RenderStack Graphics Sources"
   REGULAR_EXPRESSION
   "include/renderstack_graphics/.*\\.inl|include/renderstack_graphics/.*\\.hpp|source/.*\\.cpp")

# source_group("src" REGULAR_EXPRESSION ".hpp|.cpp")

install(TARGETS renderstack_graphics DESTINATION lib)
