#include "renderstack_toolkit/platform.hpp"
#include "main/debug_renderer.hpp"
#include "renderstack_graphics/buffer.hpp"
#include "renderstack_graphics/configuration.hpp"
#include "renderstack_graphics/program.hpp"
#include "renderstack_graphics/renderer.hpp"
#include "renderstack_graphics/uniform_block.hpp"
#include "renderstack_graphics/uniform_buffer_range.hpp"
#include "renderstack_graphics/uniform.hpp"
#include "renderstack_graphics/vertex_format.hpp"
#include "renderstack_graphics/vertex_stream_mappings.hpp"
#include "renderstack_toolkit/gl.hpp"
#include "renderstack_toolkit/strong_gl_enums.hpp"
#include "renderstack_toolkit/math_util.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <fstream>
#include <sstream>
#include <sys/stat.h>
#include <iomanip>

using namespace renderstack::graphics;
using namespace renderstack;
using namespace gl;
using namespace glm;
using namespace std;


debug_renderer::debug_renderer(
   shared_ptr<renderstack::graphics::renderer>  renderer,
   shared_ptr<programs>                         programs,
   shared_ptr<renderstack::graphics::buffer>    uniform_buffer
)
:  m_renderer(renderer)
,  m_programs(programs)
{
   auto &r = *m_renderer;

   if (m_programs->use_uniform_buffers())
   {
      m_uniform_buffer_range = make_shared<uniform_buffer_range>(
         m_programs->block,
         uniform_buffer
      );
   }

   m_render_states.depth.set_enabled(true);
   m_render_states.face_cull.set_enabled(true);

   auto vf = m_vertex_format = make_shared<vertex_format>();
   vf->make_attribute(
      vertex_attribute_usage::position,
      gl::vertex_attrib_pointer_type::float_,
      gl::vertex_attrib_pointer_type::float_,
      0,
      3
   );
   vf->make_attribute(
      vertex_attribute_usage::color,
      gl::vertex_attrib_pointer_type::float_,   // TODO unsigned byte would be sufficient
      gl::vertex_attrib_pointer_type::float_,
      0,
      4
   );

   m_capacity_lines = 1000;
   m_vertex_offset = 0;
   m_index_offset = 0;

   m_vertex_buffer = make_shared<buffer>(
      renderstack::graphics::buffer_target::array_buffer,
      m_capacity_lines * 2,
      vf->stride()
   );
   m_vertex_buffer->allocate_storage(r);

   m_index_buffer = make_shared<buffer>(
      renderstack::graphics::buffer_target::element_array_buffer,
      m_capacity_lines * 2,
      sizeof(GLushort)
   );
   m_index_buffer->allocate_storage(r);

   m_vertex_stream = make_shared<renderstack::graphics::vertex_stream>();
   m_programs->mappings->add_to_vertex_stream(
      m_vertex_stream,
      m_vertex_buffer,
      m_vertex_format);

   auto va = m_vertex_stream->vertex_array();
   r.set_vertex_array(va);
   r.setup_attribute_pointers(m_vertex_stream, 0);
   va->set_index_buffer(m_index_buffer);

   r.reset_vertex_array();
}

void debug_renderer::set_camera(
   glm::mat4 const &clip_from_world,
   glm::mat4 const &view_from_world
)
{
   m_clip_from_world = clip_from_world;
   m_view_from_world = view_from_world;
}

void debug_renderer::set_model(glm::mat4 const &world_from_model)
{
   if (m_index_offset > m_current_draw.first)
   {
      m_current_draw.count = m_index_offset - m_current_draw.first;
      m_draws.push_back(m_current_draw);
   }

   m_current_draw.world_from_model = world_from_model;
   m_current_draw.first = m_index_offset;
}

static void debug_program()
{
   GLint current_program;
   GLint active_attributes;
   GLint link_status;

   gl::get_integer_v(GL_CURRENT_PROGRAM,        &current_program);

   GLint p = current_program;

   if (p == 0)
      return;

   gl::get_program_iv(p, GL_ACTIVE_ATTRIBUTES,   &active_attributes);
   gl::get_program_iv(p, GL_LINK_STATUS,         &link_status);

   printf("Program:           %d\n", p);
   printf("Link status:       %d\n", link_status);
   printf("Active attributes: %d\n", active_attributes);
   for (int i = 0; i < active_attributes; ++i)
   {
      GLsizei buf_size = 1024;
      GLsizei length;
      GLint   size;
      GLenum  type;
      char    buffer[1024];

      gl::get_active_attrib(p, i, buf_size, &length, &size, &type, &buffer[0]);
      GLint location = gl::get_attrib_location(p, buffer);

      printf("Program attribute %d: %s size = %d, type %d, location = %d\n", i, buffer, size, type, location);
   }
}
static void debug_buffer(GLenum target, GLint buffer, GLenum index_type)
{
   GLint64  size;
   GLint    usage;
   GLint    access_flags;
   GLint    mapped;
   GLvoid   *map_pointer;
   GLint    map_offset;
   GLint    map_length;

   GLint    old_binding;

   if (buffer == 0)
      return;

   switch (target)
   {
   case GL_ARRAY_BUFFER:               gl::get_integer_v(GL_ARRAY_BUFFER_BINDING,               &old_binding); break;
   case GL_ELEMENT_ARRAY_BUFFER:       gl::get_integer_v(GL_ELEMENT_ARRAY_BUFFER_BINDING,       &old_binding); break;
   case GL_PIXEL_PACK_BUFFER:          gl::get_integer_v(GL_PIXEL_PACK_BUFFER_BINDING,          &old_binding); break;
   case GL_PIXEL_UNPACK_BUFFER:        gl::get_integer_v(GL_PIXEL_UNPACK_BUFFER_BINDING,        &old_binding); break;
   case GL_COPY_READ_BUFFER:           gl::get_integer_v(GL_COPY_READ_BUFFER_BINDING,           &old_binding); break;
   case GL_COPY_WRITE_BUFFER:          gl::get_integer_v(GL_COPY_WRITE_BUFFER_BINDING,          &old_binding); break;
   case GL_UNIFORM_BUFFER:             gl::get_integer_v(GL_UNIFORM_BUFFER_BINDING,             &old_binding); break;
   case GL_TRANSFORM_FEEDBACK_BUFFER:  gl::get_integer_v(GL_TRANSFORM_FEEDBACK_BUFFER_BINDING,  &old_binding); break;
   case GL_DRAW_INDIRECT_BUFFER:       gl::get_integer_v(GL_DRAW_INDIRECT_BUFFER_BINDING,       &old_binding); break;
   }

   gl::bind_buffer(target, buffer);

   gl::get_buffer_parameter_i64v(target,  GL_BUFFER_SIZE,         &size);
   gl::get_buffer_parameter_iv(target,    GL_BUFFER_USAGE,        &usage);
   gl::get_buffer_parameter_iv(target,    GL_BUFFER_ACCESS_FLAGS, &access_flags);
   gl::get_buffer_parameter_iv(target,    GL_BUFFER_MAPPED,       &mapped);
   gl::get_buffer_pointer_v   (target,    GL_BUFFER_MAP_POINTER,  &map_pointer);
   gl::get_buffer_parameter_iv(target,    GL_BUFFER_MAP_OFFSET,   &map_offset);
   gl::get_buffer_parameter_iv(target,    GL_BUFFER_MAP_LENGTH,   &map_length);

   printf("Buffer %d\n", buffer);
   printf("Size:           %d\n", static_cast<unsigned long>(size));
   printf("Usage:          %d\n", usage);
   printf("Access flags:   %d\n", access_flags);
   printf("Mapped:         %d\n", mapped);
   printf("Map pointer:    %x\n", map_pointer);
   printf("Map offset:     %d\n", map_offset);
   printf("Map length:     %d\n", map_length);

   if (!mapped)
   {
      switch (index_type)
      {
      case GL_NONE:
         {
            size_t count = static_cast<size_t>(size) / sizeof(float);
            float *data = new float[count];

            gl::get_buffer_sub_data(target, 0, count * 4, data);

            for (size_t i = 0; i < count && i < 16; ++i)
               printf("%f ", data[i]);

            printf("\n");

            delete[] data;
         }
         break;

      case GL_UNSIGNED_BYTE:
         {
            size_t count = static_cast<size_t>(size) / sizeof(uint8_t);
            uint8_t *data = new uint8_t[count];

            gl::get_buffer_sub_data(target, 0, count * sizeof(uint8_t), data);
            for (size_t i = 0; i < count && i < 16; ++i)
               printf("%u ", data[i]);

            printf("\n");

            delete[] data;
         }
         break;

      case GL_UNSIGNED_SHORT:
         {
            size_t count = static_cast<size_t>(size) / sizeof(uint16_t);
            uint16_t *data = new uint16_t[count];

            gl::get_buffer_sub_data(target, 0, count * sizeof(uint16_t), data);
            for (size_t i = 0; i < count && i < 16; ++i)
               printf("%u ", data[i]);

            printf("\n");

            delete[] data;
         }
         break;

      case GL_UNSIGNED_INT:
         {
            size_t count = static_cast<size_t>(size) / sizeof(uint32_t);
            uint32_t *data = new uint32_t[count];

            gl::get_buffer_sub_data(target, 0, count * sizeof(uint32_t), data);
            for (size_t i = 0; i < count && i < 16; ++i)
               printf("%u ", data[i]);

            printf("\n");

            delete[] data;
         }
         break;

      default:
         assert(0);
      }
   }

   gl::bind_buffer(target, old_binding);
}
static void debug_vao()
{
   GLint max_vertex_attribs;
   GLint vertex_array_binding;
   GLint element_array_buffer_binding;

   gl::get_integer_v(GL_MAX_VERTEX_ATTRIBS,           &max_vertex_attribs);
   gl::get_integer_v(GL_VERTEX_ARRAY_BINDING,         &vertex_array_binding);
   gl::get_integer_v(GL_ELEMENT_ARRAY_BUFFER_BINDING, &element_array_buffer_binding);

   printf("Vertex array binding:          %d\n", vertex_array_binding);
   printf("Element array buffer binding:  %d\n", element_array_buffer_binding);
   debug_buffer(GL_ELEMENT_ARRAY_BUFFER, element_array_buffer_binding, GL_UNSIGNED_SHORT);

   for (int i = 0; i < max_vertex_attribs; ++i)
   {
      GLint buffer_binding;
      GLint enabled;
      GLint size;
      GLint stride;
      GLint type;
      GLint normalized;
      GLint integer;
      GLint divisor;
      GLvoid *pointer;

      gl::get_vertex_attrib_iv(i, GL_VERTEX_ATTRIB_ARRAY_BUFFER_BINDING,   &buffer_binding);
      gl::get_vertex_attrib_iv(i, GL_VERTEX_ATTRIB_ARRAY_ENABLED,          &enabled);
      gl::get_vertex_attrib_iv(i, GL_VERTEX_ATTRIB_ARRAY_SIZE,             &size);
      gl::get_vertex_attrib_iv(i, GL_VERTEX_ATTRIB_ARRAY_STRIDE,           &stride);
      gl::get_vertex_attrib_iv(i, GL_VERTEX_ATTRIB_ARRAY_TYPE,             &type);
      gl::get_vertex_attrib_iv(i, GL_VERTEX_ATTRIB_ARRAY_NORMALIZED,       &normalized);
      gl::get_vertex_attrib_iv(i, GL_VERTEX_ATTRIB_ARRAY_INTEGER,          &integer);
      gl::get_vertex_attrib_iv(i, GL_VERTEX_ATTRIB_ARRAY_DIVISOR,          &divisor);

      gl::get_vertex_attrib_pointer_v(i, GL_VERTEX_ATTRIB_ARRAY_POINTER, &pointer);

      if (buffer_binding || enabled)
      {
         printf("Vertex attrib %d: buffer = %d, enabled = %d, size = %d, stride = %d, type = %d, normalized = %d, integer = %d, divisor = %d, pointer = %u\n",
            i,
            buffer_binding,
            enabled,
            size,
            stride,
            type,
            normalized,
            integer,
            divisor,
            reinterpret_cast<unsigned int>(pointer)
         );

         debug_buffer(GL_ARRAY_BUFFER, buffer_binding, GL_NONE);
      }
   }
}
static void debug()
{
   debug_program();
   debug_vao();

   GLint active_texture;
   gl::get_integer_v(GL_ACTIVE_TEXTURE, &active_texture);
}

void debug_renderer::render()
{
   if (m_index_offset > m_current_draw.first)
   {
      m_current_draw.count = m_index_offset - m_current_draw.first;
      m_draws.push_back(m_current_draw);
   }

   auto &r = *m_renderer;
   auto &t = r.track();
   auto &o = m_programs->uniform_offsets;
   auto p = m_programs->debug_line;

   t.execute(&m_render_states);
   r.set_program(p);
   r.use_vertex_stream(m_vertex_stream);

   if (m_programs->use_uniform_buffers())
   {
      assert(m_uniform_buffer_range);
      r.set_uniform_buffer_range(m_programs->block->binding_point(), m_uniform_buffer_range);
   }

   for (auto i = m_draws.cbegin(); i != m_draws.cend(); ++i)
   {
      auto draw = *i;
      mat4 clip_from_model = m_clip_from_world * draw.world_from_model;
      //mat4 clip_from_model(1.0f);

      if (m_programs->use_uniform_buffers())
      {
         unsigned char *start = m_uniform_buffer_range->begin_edit(r);
         ::memcpy(&start[o.clip_from_model], value_ptr(clip_from_model), 16 * sizeof(float));
         m_uniform_buffer_range->end_edit(r);
      }
      else
      {
         gl::uniform_matrix_4fv(p->uniform_at(m_programs->uniform_keys.clip_from_model), 1, GL_FALSE, value_ptr(clip_from_model));
      }

      gl::draw_elements(
         gl::begin_mode::lines,
         draw.count,
         gl::draw_elements_type::unsigned_short,
         reinterpret_cast<GLvoid*>(draw.first * 2));
   }

   m_draws.clear();
}

void debug_renderer::set_color(glm::vec4 color)
{
   m_color = color;
}


void debug_renderer::begin_edit()
{
   auto &r = *m_renderer;
   m_vertex_offset = 0;
   m_index_offset = 0;
   m_in_edit = true;
   r.set_vertex_array(m_vertex_stream->vertex_array());
   r.set_buffer(renderstack::graphics::buffer_target::array_buffer, m_vertex_buffer);
   m_vertex_ptr = reinterpret_cast<float*>(
      m_vertex_buffer->map(
         r,
         0,
         m_capacity_lines * 2,
         static_cast<gl::buffer_access_mask::value>(
            gl::buffer_access_mask::map_flush_explicit_bit   |
            gl::buffer_access_mask::map_invalidate_range_bit |
            gl::buffer_access_mask::map_write_bit
         )
      )
   );

   m_index_ptr = reinterpret_cast<uint16_t*>(
      m_index_buffer->map(
         r,
         0,
         m_capacity_lines * 2,
         static_cast<gl::buffer_access_mask::value>(
            gl::buffer_access_mask::map_flush_explicit_bit   |
            gl::buffer_access_mask::map_invalidate_range_bit |
            gl::buffer_access_mask::map_write_bit
         )
      )
   );

   m_vertex_ptr_start = m_vertex_ptr;
   m_index_ptr_start = m_index_ptr;

   m_current_draw.first = 0;
   m_current_draw.count = 0;
}

void debug_renderer::end_edit()
{
   auto &r = *m_renderer;
   m_vertex_buffer->flush_and_unmap(r, m_vertex_offset);
   m_index_buffer->flush_and_unmap(r, m_index_offset);

   m_in_edit = false;
   m_vertex_ptr = nullptr;
   m_index_ptr = nullptr;
}

uint16_t debug_renderer::add_point(vec3 p)
{
   *m_vertex_ptr++ = p.x;
   *m_vertex_ptr++ = p.y;
   *m_vertex_ptr++ = p.z;
   *m_vertex_ptr++ = m_color.x;
   *m_vertex_ptr++ = m_color.y;
   *m_vertex_ptr++ = m_color.z;
   *m_vertex_ptr++ = m_color.w;
   return m_vertex_offset++;
}
void debug_renderer::add_line(vec3 start, vec3 end)
{
   m_index_ptr[m_index_offset++] = add_point(start);
   m_index_ptr[m_index_offset++] = add_point(end);
}

void debug_renderer::add_box(vec3 min_, vec3 max_)
{
   uint16_t a = add_point(vec3(min_.x, min_.y, min_.z));  //    h      g  
   uint16_t b = add_point(vec3(max_.x, min_.y, min_.z));  //   e      f   
   uint16_t c = add_point(vec3(max_.x, min_.y, max_.z));  //              
   uint16_t d = add_point(vec3(min_.x, min_.y, max_.z));  //    d      c  
   uint16_t e = add_point(vec3(min_.x, max_.y, min_.z));  //   a      b   
   uint16_t f = add_point(vec3(max_.x, max_.y, min_.z));
   uint16_t g = add_point(vec3(max_.x, max_.y, max_.z));
   uint16_t h = add_point(vec3(min_.x, max_.y, max_.z));

   m_index_ptr[m_index_offset++] = a; // bottom abcd
   m_index_ptr[m_index_offset++] = b;
   m_index_ptr[m_index_offset++] = b;
   m_index_ptr[m_index_offset++] = c;
   m_index_ptr[m_index_offset++] = c;
   m_index_ptr[m_index_offset++] = d;
   m_index_ptr[m_index_offset++] = d;
   m_index_ptr[m_index_offset++] = a;

   m_index_ptr[m_index_offset++] = e; // top efgh
   m_index_ptr[m_index_offset++] = f;
   m_index_ptr[m_index_offset++] = f;
   m_index_ptr[m_index_offset++] = g;
   m_index_ptr[m_index_offset++] = g;
   m_index_ptr[m_index_offset++] = h;
   m_index_ptr[m_index_offset++] = h;
   m_index_ptr[m_index_offset++] = e;

   m_index_ptr[m_index_offset++] = a; // connect bottom and top
   m_index_ptr[m_index_offset++] = e;
   m_index_ptr[m_index_offset++] = b;
   m_index_ptr[m_index_offset++] = f;
   m_index_ptr[m_index_offset++] = c;
   m_index_ptr[m_index_offset++] = g;
   m_index_ptr[m_index_offset++] = d;
   m_index_ptr[m_index_offset++] = h;
}


