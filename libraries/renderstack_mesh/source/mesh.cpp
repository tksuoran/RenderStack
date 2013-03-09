#include "renderstack_toolkit/platform.hpp"
#include "renderstack_toolkit/logstream.hpp"
#include "renderstack_graphics/vertex_buffer.hpp"
#include "renderstack_graphics/index_buffer.hpp"
#include "renderstack_mesh/mesh.hpp"
#include "renderstack_mesh/index_range.hpp"
#include <cassert>

#define LOG_CATEGORY &graphics_mesh

namespace renderstack { namespace mesh {

using namespace std;
using namespace renderstack::graphics;

mesh::mesh()
:  m_first_vertex          (0)
,  m_first_index           (0)
,  m_vertex_count          (0)
,  m_index_count           (0)
,  m_next_index_range_start(0)
{
}

shared_ptr<renderstack::graphics::vertex_buffer> mesh::vertex_buffer() const { return m_vertex_buffer; }
shared_ptr<renderstack::graphics::index_buffer> mesh::index_buffer() const { return m_index_buffer; }
size_t mesh::first_vertex() const { return m_first_vertex; }
size_t mesh::first_index () const { return m_first_index; }
size_t mesh::vertex_count() const { return m_vertex_count; }
size_t mesh::index_count () const { return m_index_count; }


void mesh::allocate_vertex_buffer(
   size_t vertex_stride,
   size_t vertex_count,
   gl::buffer_usage_hint::value usage
)
{
   slog_trace(
      "mesh::allocate_vbo(vertex_stride = %u, vertex_count = %u, usage = %s)",
      static_cast<unsigned int>(vertex_stride),
      static_cast<unsigned int>(vertex_count),
      gl::enum_string(usage)
   );

   m_vertex_buffer = std::make_shared<renderstack::graphics::vertex_buffer>(
      vertex_count,
      vertex_stride, 
      usage
   );
   m_vertex_count = vertex_count;
   m_first_vertex = m_vertex_buffer->allocate_vertices(vertex_count);
}
void mesh::allocate_vertex_buffer(
   shared_ptr<renderstack::graphics::vertex_buffer> vertex_buffer, 
   size_t vertex_count
)
{
   slog_trace(
      "mesh::allocate_vertex_buffer(vertex_buffer = %p, vertex_count = %u)",
      vertex_buffer.get(),
      static_cast<unsigned int>(vertex_count));

   m_vertex_buffer = vertex_buffer;
   m_vertex_count = vertex_count;
   m_first_vertex = vertex_buffer->allocate_vertices(vertex_count);
}
void mesh::allocate_index_buffer(size_t index_stride, size_t index_count, gl::buffer_usage_hint::value usage)
{
   slog_trace(
      "mesh::allocate_ibo(index_stride = %u, index_count = %u, usage = %s)",
      static_cast<unsigned int>(index_stride),
      static_cast<unsigned int>(index_count),
      gl::enum_string(usage)
   );

   m_index_buffer = std::make_shared<renderstack::graphics::index_buffer>(
      index_count,
      index_stride, 
      usage
   );
   m_index_count = index_count;
   m_first_index = m_index_buffer->allocate_indices(index_count);
}
void mesh::allocate_index_buffer(
   shared_ptr<renderstack::graphics::index_buffer> index_buffer,
   size_t index_count
)
{
   slog_trace(
      "mesh::allocate_ibo(ibo_pool = %p, index_count = %u)",
      index_buffer.get(),
      static_cast<unsigned int>(index_count)
   );

   m_index_buffer = index_buffer;
   m_index_count = index_count;
   m_first_index = index_buffer->allocate_indices(index_count);
}
void mesh::allocate_index_range(
   gl::begin_mode::value   begin_mode,
   size_t                  index_count,
   index_range             &range
)
{
   slog_trace(
      "mesh::allocate_index_range(begin_mode = %s, index_count = %u, index_range = %p)",
      gl::enum_string(begin_mode),
      static_cast<unsigned int>(index_count),
      &range
   );

   range.begin_mode = begin_mode;
   range.first_index = m_next_index_range_start;
   range.index_count = index_count;
   m_next_index_range_start += index_count;
   if (m_next_index_range_start > m_index_count)
      throw std::runtime_error("out of memory");
}
void mesh::set_first_vertex(size_t value) { m_first_vertex = value; }
void mesh::set_first_index (size_t value) { m_first_index = value; }
void mesh::set_vertex_count(size_t value) { m_vertex_count = value; }
void mesh::set_index_count (size_t value) { m_index_count = value; }

} }

