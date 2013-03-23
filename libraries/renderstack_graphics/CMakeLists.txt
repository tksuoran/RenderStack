# CMakeLists.txt for renderstack_graphics

include_directories(
   ${RENDERSTACK_WINDOW_TOOLKIT_INCLUDE_DIRS}
   ${KHRONOS_INCLUDE}
   ${CMAKE_SOURCE_DIR}/libraries/renderstack_graphics/include
   ${CMAKE_SOURCE_DIR}/libraries/renderstack_toolkit/include
)

add_library(
   renderstack_graphics STATIC
   include/renderstack_graphics/buffer.hpp
   include/renderstack_graphics/configuration.hpp
   include/renderstack_graphics/index_buffer.hpp
   include/renderstack_graphics/program.hpp
   include/renderstack_graphics/sampler.hpp
   include/renderstack_graphics/samplers.hpp
   include/renderstack_graphics/shader_monitor.hpp
   include/renderstack_graphics/uniform.hpp
   include/renderstack_graphics/uniform_block.hpp
   include/renderstack_graphics/uniform_buffer.hpp
   include/renderstack_graphics/uniform_buffer_range.hpp
   include/renderstack_graphics/vertex_buffer.hpp
   include/renderstack_graphics/vertex_attribute.hpp
   include/renderstack_graphics/vertex_format.hpp
   include/renderstack_graphics/vertex_stream.hpp
   include/renderstack_graphics/vertex_stream_binding.hpp
   include/renderstack_graphics/vertex_stream_mapping.hpp
   include/renderstack_graphics/vertex_stream_mappings.hpp
   include/renderstack_graphics/context.hpp
   source/buffer.cpp
   source/configuration.cpp
   source/context.cpp
   source/index_buffer.cpp
   source/program.cpp
   source/sampler.cpp
   source/samplers.cpp
   source/shader_monitor.cpp
   source/uniform.cpp
   source/uniform_block.cpp
   source/uniform_buffer.cpp
   source/uniform_buffer_range.cpp
   source/vertex_buffer.cpp
   source/vertex_attribute.cpp
   source/vertex_format.cpp
   source/vertex_stream.cpp
   source/vertex_stream_binding.cpp
   source/vertex_stream_mapping.cpp
   source/vertex_stream_mappings.cpp
)


install(TARGETS renderstack_graphics DESTINATION lib)
