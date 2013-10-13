#include "renderstack_toolkit/platform.hpp"
#include "renderers/debug_renderer.hpp"
#include "renderstack_graphics/buffer.hpp"
#include "renderstack_graphics/configuration.hpp"
#include "renderstack_graphics/program.hpp"
#include "renderstack_graphics/renderer.hpp"
#include "renderstack_graphics/uniform_block.hpp"
#include "renderstack_graphics/uniform_buffer_range.hpp"
#include "renderstack_graphics/uniform.hpp"
#include "renderstack_graphics/vertex_format.hpp"
#include "renderstack_graphics/vertex_stream_mappings.hpp"
#include "renderstack_scene/camera.hpp"
#include "renderstack_scene/viewport.hpp"
#include "renderstack_toolkit/gl.hpp"
#include "renderstack_toolkit/strong_gl_enums.hpp"
#include "renderstack_toolkit/math_util.hpp"
#include "renderstack_ui/font.hpp"
#include "renderstack_ui/gui_renderer.hpp"
#include "renderstack_ui/text_buffer.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <fstream>
#include <sstream>
#include <sys/stat.h>
#include <iomanip>

using namespace renderstack::graphics;
using namespace renderstack::ui;
using namespace gl;
using namespace glm;
using namespace std;


debug_renderer::debug_renderer()
:  service        ("debug_renderer")
,  m_renderer     (nullptr)
,  m_programs     (nullptr)
,  m_font         (nullptr)
,  m_text_buffer  (nullptr)
{
}

/*virtual*/ debug_renderer::~debug_renderer()
{
}

void debug_renderer::connect(
   shared_ptr<renderstack::graphics::renderer>  renderer_,
   shared_ptr<renderstack::ui::gui_renderer>    gui_renderer_,
   shared_ptr<programs>                         programs_
)
{
   m_renderer = renderer_;
   m_gui_renderer = gui_renderer_;
   m_programs = programs_;

   initialization_depends_on(renderer_);
   initialization_depends_on(gui_renderer_);
   initialization_depends_on(programs_);
}

void debug_renderer::initialize_service()
{
   assert(m_renderer);
   assert(m_programs);

   auto &r = *m_renderer;

#if defined(RENDERSTACK_USE_FREETYPE)
   auto p = m_programs->font;
   auto m = p->mappings();

   m_font = make_shared<font>(r, "res/fonts/Ubuntu-R.ttf", 10);
   m_text_buffer = make_shared<text_buffer>(m_gui_renderer, m_font, m);

   m_font_render_states.blend.set_enabled(true);
   m_font_render_states.blend.rgb().set_equation_mode(gl::blend_equation_mode::func_add);
   m_font_render_states.blend.rgb().set_source_factor(gl::blending_factor_src::one);
   m_font_render_states.blend.rgb().set_destination_factor(gl::blending_factor_dest::one_minus_src_alpha);
#endif

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

void debug_renderer::clear_text_lines()
{
   m_debug_lines.clear();
   m_debug_print_ats.clear();
}

void debug_renderer::printf(int x, int y, const char *format, ...)
{
   char buffer[1000]; // TODO
   va_list args;
   va_start(args, format);
   ::memset(buffer, 0, 1000);
   vsnprintf(buffer, 999, format, args);
   va_end(args);
   
   print_at p;
   p.x = x;
   p.y = y;
   p.text = buffer;
   m_debug_print_ats.push_back(p);
}

void debug_renderer::printf(const char *format, ...)
{
   char buffer[1000]; // TODO
   string::size_type line_begin;
   string::size_type len;
   string::size_type pos;
   va_list args;
   va_start(args, format);
   ::memset(buffer, 0, 1000);
   vsnprintf(buffer, 999, format, args);
   va_end(args);
   
   line_begin = 0;
   len = 0;
   for (pos = 0; (buffer[pos] != 0) && (pos < 1000); ++pos)
   {
      if (buffer[pos] == '\n')
      {
         if (len > 0)
         {
            string line(&buffer[line_begin], len);
            m_debug_lines.push_back(line);
         }
         line_begin = pos + 1;
         len = 0;
         continue;
      }
      ++len;
   }

   if (len > 0)
      m_debug_lines.push_back(
         string(&buffer[line_begin], len)
      );
}

void debug_renderer::render_text_lines(renderstack::scene::viewport const &vp)
{
   assert(m_programs);
   assert(m_text_buffer);

   float w = (float)vp.width();
   float h = (float)vp.height();

   m_text_buffer->begin_print();
   for (size_t i = 0; i < m_debug_lines.size(); ++i)
      m_text_buffer->print(m_debug_lines[i], 0.0f, h - (i + 1) * m_font->line_height());

   for (size_t i = 0; i < m_debug_print_ats.size(); ++i)
      m_text_buffer->print(m_debug_print_ats[i].text, m_debug_print_ats[i].x, m_debug_print_ats[i].y);

   int chars_printed = m_text_buffer->end_print();

   if (chars_printed > 0)
   {
      //auto gr = *m_gui_renderer;
      auto &r = *m_renderer;
      auto &t = r.track();
      t.execute(&m_font_render_states);

      auto p = m_programs->font;
      r.set_program(p);

      gl::viewport(0, 0, (GLsizei)w, (GLsizei)h);
      mat4 ortho = glm::ortho(0.0f, (float)w, 0.0f, (float)h);
      gl::scissor(0, 0, (GLsizei)w, (GLsizei)h);

      glm::vec4 white(1.0f, 1.0f, 1.0f, 0.66f); // gamma in 4th component
      if (m_programs->use_uniform_buffers())
      {
         unsigned char *start = m_programs->begin_edit_uniforms();
         ::memcpy(&start[m_programs->model_ubr->first_byte() + m_programs->model_block_access.clip_from_model], value_ptr(ortho), 16 * sizeof(float));
         ::memcpy(&start[m_programs->material_ubr->first_byte() + m_programs->material_block_access.color], value_ptr(white), 4 * sizeof(float));
         m_programs->model_ubr->flush(r);
         m_programs->material_ubr->flush(r);
         m_programs->end_edit_uniforms();
      }
      else
      {
         int model_to_clip_ui = p->uniform_at(m_programs->model_block_access.clip_from_model);
         int color_ui         = p->uniform_at(m_programs->material_block_access.color);

         gl::uniform_matrix_4fv(model_to_clip_ui, 1, GL_FALSE, value_ptr(ortho));
         gl::uniform_4fv(color_ui, 1, value_ptr(white));
      }

      //int texture_ui = p->uniform("font_texture")->index();
      //gl::uniform_1i(texture_ui, 0);

      {
         auto t = m_text_buffer->font()->texture();
         t->set_min_filter(texture_min_filter::nearest);
         t->set_mag_filter(texture_mag_filter::nearest);
         (void)r.set_texture(0, t);
         t->apply(r, 0);
      }

      m_text_buffer->render();
   }

}

void debug_renderer::set_camera(std::shared_ptr<renderstack::scene::camera> camera)
{
   m_camera = camera;
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

void debug_renderer::render()
{
   if (m_index_offset > m_current_draw.first)
   {
      m_current_draw.count = m_index_offset - m_current_draw.first;
      m_draws.push_back(m_current_draw);
   }

   auto &r = *m_renderer;
   auto &t = r.track();
   auto p = m_programs->debug_line;

   t.execute(&m_render_states);
   r.set_program(p);
   r.use_vertex_stream(m_vertex_stream);

   for (auto i = m_draws.cbegin(); i != m_draws.cend(); ++i)
   {
      auto draw = *i;
      mat4 clip_from_model = m_camera->clip_from_world().matrix() * draw.world_from_model;
      //mat4 clip_from_model(1.0f);

      if (m_programs->use_uniform_buffers())
      {
         unsigned char *start = m_programs->begin_edit_uniforms();
         ::memcpy(&start[m_programs->model_ubr->first_byte() + m_programs->model_block_access.clip_from_model], value_ptr(clip_from_model), 16 * sizeof(float));
         m_programs->model_ubr->flush(r);
         m_programs->end_edit_uniforms();
      }
      else
      {
         gl::uniform_matrix_4fv(p->uniform_at(m_programs->model_block_access.clip_from_model), 1, GL_FALSE, value_ptr(clip_from_model));
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


