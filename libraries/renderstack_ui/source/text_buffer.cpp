#include "renderstack_toolkit/platform.hpp"
#include "renderstack_toolkit/gl.hpp"
#include "renderstack_toolkit/strong_gl_enums.hpp"
#include "renderstack_graphics/configuration.hpp"
#include "renderstack_graphics/vertex_format.hpp"
#include "renderstack_graphics/vertex_stream_mappings.hpp"
#include "renderstack_mesh/mesh.hpp"
#include "renderstack_ui/context.hpp"
#include "renderstack_ui/gui_renderer.hpp"
#include "renderstack_ui/text_buffer.hpp"
#include "renderstack_ui/log.hpp"
#include <fstream>
#include <sstream>
#include <sys/stat.h>
#include <iomanip>
#include <vector>
#include <stdexcept>
#include <algorithm>
#include <limits>

#define LOG_CATEGORY &log_text_buffer

namespace renderstack { namespace ui {

using namespace std;
using namespace gl;
using namespace renderstack::graphics;


text_buffer::text_buffer(
   shared_ptr<class gui_renderer> renderer,
   shared_ptr<class font> font,
   shared_ptr<renderstack::graphics::vertex_stream_mappings> mappings
)
:  m_renderer(renderer)
,  m_font(font)
,  m_max_chars(2000)
{
   if (m_mesh.index_count() > std::numeric_limits<unsigned int>::max())
      throw std::runtime_error("font::prepare_gl_resources: no code path for index types other than unsigned int");

   m_mesh.allocate_vertex_buffer(m_renderer->vertex_buffer(), 4 * m_max_chars);
   m_mesh.allocate_index_buffer(m_renderer->index_buffer(), 6 * m_max_chars);

   log_trace("preparing index buffer");

   auto &r = *m_renderer->renderer();

   m_renderer->edit_ibo();
   unsigned short *start = static_cast<unsigned short *>(
      m_mesh.index_buffer()->map(
         r,
         m_mesh.first_index(), 
         m_mesh.index_count(), 
         (gl::buffer_access_mask::value)
         (
            gl::buffer_access_mask::map_write_bit | 
            gl::buffer_access_mask::map_invalidate_range_bit
         )
      )
   );
   unsigned short *ptr = start;
   unsigned short vertex_index = 
      configuration::can_use.draw_elements_base_vertex 
      ? 0 
      : static_cast<unsigned short>(m_mesh.first_vertex());
   for (unsigned short i = 0; i < m_max_chars; ++i)
   {
      *ptr++ = vertex_index;
      *ptr++ = vertex_index + 1;
      *ptr++ = vertex_index + 2;
      *ptr++ = vertex_index;
      *ptr++ = vertex_index + 2;
      *ptr++ = vertex_index + 3;
      vertex_index += 4;
   }
   m_mesh.index_buffer()->unmap(r);
}
text_buffer::~text_buffer()
{
   //  \todo deallocate from mesh_pool
}


shared_ptr<class font> text_buffer::font()
{
   return m_font;
}
void text_buffer::render()
{
   slog_trace(
      "text_buffer::render() m_chars_printed = %u",
      static_cast<unsigned int>(m_chars_printed));

   if (m_chars_printed > 0)
   {
      gl::begin_mode::value         begin_mode     = gl::begin_mode::triangles;
      GLsizei                       count          = static_cast<GLsizei>(m_chars_printed * 6); // 2 triangles = 6 indices per char
      gl::draw_elements_type::value index_type     = gl::draw_elements_type::unsigned_short;
      GLvoid                        *index_pointer = reinterpret_cast<GLvoid*>(m_mesh.first_index() * m_mesh.index_buffer()->stride());
      GLint                         base_vertex    = configuration::can_use.draw_elements_base_vertex
                                                      ? static_cast<GLint>(m_mesh.first_vertex())
                                                      : 0;

      m_renderer->draw_elements_base_vertex(begin_mode, count, index_type, index_pointer, base_vertex);
   }
}
void text_buffer::begin_print()
{
   slog_trace("text_buffer::begin_print()");

   //  We want to write directly to the vertex buffer but
   //  we don't yet know how many char / vertices will be
   //  written - use explicit flushing.
   m_renderer->edit_vbo();

   auto &r = *m_renderer->renderer();

   m_vertex_ptr = static_cast<float*>(
      m_mesh.vertex_buffer()->map(
         r,
         m_mesh.first_vertex(),
         m_mesh.vertex_count(),
         (gl::buffer_access_mask::value)
         (
            gl::buffer_access_mask::map_write_bit | 
            gl::buffer_access_mask::map_invalidate_range_bit |
            gl::buffer_access_mask::map_flush_explicit_bit
         )
      )
   );

   log_trace("m_vertex_ptr = %p", m_vertex_ptr);

   m_chars_printed = 0;
   m_bounding_box.reset_for_grow();
}
size_t text_buffer::end_print()
{
   slog_trace("text_buffer::end_print() m_chars_printed = %u",
      static_cast<unsigned int>(m_chars_printed));

   auto &r = *m_renderer->renderer();

   m_vertex_ptr = nullptr;
   m_mesh.vertex_buffer()->flush_and_unmap(r, 4 * m_chars_printed);
   return m_chars_printed;
}
void text_buffer::print(string const &text, float x, float y)
{
   int a = 0;
   m_chars_printed += m_font->print(text, m_bounding_box, m_vertex_ptr, x, y);
   ++a;
}
void text_buffer::measure(string const &text)
{
   m_bounding_box.reset_for_grow();
   if (text.size() == 0)
      return;

   m_font->measure(text, m_bounding_box);
}
void text_buffer::print_center(string const &text, float x, float y)
{
   measure(text);
   glm::vec2 p(x, y);
   p -= m_bounding_box.half_size();
   print(text, std::ceil(p.x), std::ceil(p.y));
}

} }
