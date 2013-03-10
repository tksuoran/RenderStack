#include "renderstack_ui/context.hpp"
#include "renderstack_ui/ninepatch.hpp"
#include "renderstack_ui/ninepatch_style.hpp"
#include "renderstack_ui/style.hpp"
#include "renderstack_graphics/configuration.hpp"
#include "renderstack_graphics/context.hpp"
#include "renderstack_graphics/vertex_buffer.hpp"
#include "renderstack_graphics/index_buffer.hpp"
#include "renderstack_graphics/vertex_format.hpp"
#include "renderstack_graphics/vertex_stream_mappings.hpp"
#include "renderstack_toolkit/gl.hpp"
#include "renderstack_toolkit/strong_gl_enums.hpp"
#include "renderstack_toolkit/lodepng.h"
#include "renderstack_toolkit/logstream.hpp"

#define LOG_CATEGORY &log_ninepatch

namespace renderstack { namespace ui {

using namespace std;
using namespace gl;
using namespace renderstack::graphics;

std::shared_ptr<vertex_format> context_ninepatch::vertex_format()
{
   if (!s_vertex_format)
   {
      s_vertex_format = make_shared<renderstack::graphics::vertex_format>();
      s_vertex_format->make_attribute(
         static_cast<vertex_attribute_usage::value>(
            vertex_attribute_usage::position | vertex_attribute_usage::tex_coord
         ),
         gl::vertex_attrib_pointer_type::float_,
         gl::vertex_attrib_pointer_type::float_,
         0, 
         4,
         false
      );
   }
   return s_vertex_format;
}

ninepatch::ninepatch(shared_ptr<ninepatch_style> style)
:  m_style(style)
{
   //  16 vertices, 9 quads 

   //  12 13 14 15     
   //                  
   //   8  9 10 11     
   //                  
   //   4  5  6  7     
   //                  
   //   0  1  2  3     
   auto mappings = style->program()->mappings();
   auto uc = renderstack::ui::context::current();
   m_vertex_stream = mappings->make_vertex_stream(
      uc->ninepatch().vertex_format()
   );

   m_vertex_stream->use();

   m_mesh.allocate_vertex_buffer(uc->get_2d_vertex_buffer(), 16);
   m_mesh.allocate_index_buffer(uc->get_2d_index_buffer(), 9 * 6);

   m_mesh.index_buffer()->bind();
   unsigned short *start = static_cast<unsigned short *>(
      m_mesh.index_buffer()->map_indices(
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

#define make_quad(a, b, c, d) *ptr++ = a; *ptr++ = b; *ptr++ = c; *ptr++ = a; *ptr++ = c; *ptr++ = d
   make_quad( 4,  5,  1,  0);
   make_quad( 5,  6,  2,  1);
   make_quad( 6,  7,  3,  2);

   make_quad( 8,  9,  5,  4);
   make_quad( 9, 10,  6,  5);
   make_quad(10, 11,  7,  6);

   make_quad(12, 13,  9,  8);
   make_quad(13, 14, 10,  9);
   make_quad(14, 15, 11, 10);
#undef make_quad

   m_mesh.index_buffer()->unmap_indices();

   if (m_vertex_stream->use())
   {
      m_mesh.vertex_buffer()->bind();
      m_mesh.index_buffer()->bind();
      m_vertex_stream->setup_attribute_pointers(0);
      gl::bind_vertex_array(0);
   }
}

void ninepatch::place(
   float x0,
   float y0,
   float width,
   float height
)
{
   slog_trace("ninepatch::place(x0 = %f, y0 = %f, width = %f, height = %f)", x0, y0, width, height);

   m_size.x = width;
   m_size.y = height;

   m_mesh.vertex_buffer()->bind();

   float *ptr = (float*)m_mesh.vertex_buffer()->map_vertices(
      m_mesh.first_vertex(),
      m_mesh.vertex_count(),
      (gl::buffer_access_mask::value)
      (
         gl::buffer_access_mask::map_write_bit | 
         gl::buffer_access_mask::map_invalidate_range_bit
      )
   );

   float u[4];
   u[0] = 0.0f;
   u[1] = m_style->border_uv().x;
   u[2] = 1.0f - m_style->border_uv().x;
   u[3] = 1.0f;

   float v[4];
   v[0] = 0.0f;
   v[1] = m_style->border_uv().y;
   v[2] = 1.0f - m_style->border_uv().y;
   v[3] = 1.0f;

   float x[4];
   x[0] = x0;
   x[1] = x0 + m_style->border_pixels().x; // m_style.Texture.Size.Width;
   x[2] = x0 + width - m_style->border_pixels().x; // * style.Texture.Size.Width;
   x[3] = x0 + width;

   float y[4];
   y[0] = y0;
   y[1] = y0 + m_style->border_pixels().y; // * style.Texture.Size.Height;
   y[2] = y0 + height - m_style->border_pixels().y; // * style.Texture.Size.Height;
   y[3] = y0 + height;

   for (int yi = 0; yi < 4; ++yi)
   {
      for (int xi = 0; xi < 4; ++xi)
      {
         *ptr++ = x[xi];
         *ptr++ = y[yi];
         *ptr++ = u[xi];
         *ptr++ = v[3 - yi];
      }
   }

   m_mesh.vertex_buffer()->unmap_vertices();
}

void ninepatch::render()
{
   slog_trace("ninepatch::render()");

   gl::begin_mode::value         begin_mode     = gl::begin_mode::triangles;
   GLsizei                       count          = static_cast<GLsizei>(mesh().index_count());
   gl::draw_elements_type::value index_type     = gl::draw_elements_type::unsigned_short;
   GLvoid                        *index_pointer = reinterpret_cast<GLvoid*>(mesh().first_index() * mesh().index_buffer()->index_stride());
   GLint                         base_vertex    = static_cast<GLint>(mesh().first_vertex());

   if (m_vertex_stream->use())
   {
      if (!renderstack::graphics::configuration::can_use.draw_elements_base_vertex)
         throw std::runtime_error("not yet implemented");

      gl::draw_elements_base_vertex(begin_mode, count, index_type, index_pointer, base_vertex);
   }
   else
   {
      mesh().vertex_buffer()->bind();
      mesh().index_buffer()->bind();
      m_vertex_stream->setup_attribute_pointers(static_cast<GLint>(mesh().first_vertex()));
      gl::draw_elements(begin_mode, count, index_type, index_pointer);
   }
}

} }


//  9 patch                                     
//                                              
//  4 x 4 = 16 vertices                         
//  3 x 3 =  9 quads    = 9 x 6 = 54 indices    
//                                              
//  b = border                                  
//                                              
//  m:::n   o::p       0  a (0,     0) x0, y0   
//  ::::    ::::       1  b (b,     0) x1, y0   
//  i:::j   k::l       2  c (1-b,   0) x2, y0   
//      ::::           3  d (1,     0) x3, y0   
//      ::::           4  e (0,     b) x0, y1   
//  e   f:::g  h       5  f (b,     b) x1, y1   
//  ::::    ::::       6  g (1-b,   b) x2, y1   
//  ::::    ::::       7  h (1,     b) x3, y1   
//  a:::b   c::d       8  i (0,   1-b) x0, y2   
//                     9  j (b,   1-b)          
//  12 13 14 15       10  k (1-b, 1-b)          
//                    11  l (1,   1-b)          
//   8  9 10 11       12  m (0,     1)          
//                    13  n (b,     1)          
//   4  5  6  7       14  o (1-b,   1)          
//                    15  p (1,     1)          
//   0  1  2  3                                 
