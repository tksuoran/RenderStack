#include "renderstack_ui/context.hpp"
#include "renderstack_ui/font.hpp"
#include "renderstack_ui/gui_renderer.hpp"
#include "renderstack_ui/ninepatch.hpp"
#include "renderstack_ui/ninepatch_style.hpp"
#include "renderstack_ui/style.hpp"
#include "renderstack_graphics/renderer.hpp"
#include "renderstack_graphics/configuration.hpp"
#include "renderstack_graphics/program.hpp"
#include "renderstack_graphics/uniform_block.hpp"
#include "renderstack_graphics/uniform_buffer_range.hpp"
#include "renderstack_graphics/uniform.hpp"
#include "renderstack_graphics/samplers.hpp"
#include "renderstack_toolkit/logstream.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <cassert>

#define LOG_CATEGORY &log_ui_gui_renderer

namespace renderstack { namespace ui {

using namespace std;
using namespace glm;
using namespace renderstack::graphics;


void gui_renderer::map(shared_ptr<renderstack::graphics::program> program)
{
   assert(program);

   if (!use_uniform_buffers())
   {
      program->map_uniform(m_uniform_keys.clip_from_model, "gui_clip_from_model");
      program->map_uniform(m_uniform_keys.color_add      , "gui_color_add"      );
      program->map_uniform(m_uniform_keys.color_scale    , "gui_color_scale"    );
      program->map_uniform(m_uniform_keys.hsv_matrix     , "gui_hsv_matrix"     );
      program->map_uniform(m_uniform_keys.t              , "gui_t"              );
   }
}

gui_renderer::gui_renderer(shared_ptr<class renderstack::graphics::renderer> renderer)
:  m_start(nullptr)
{
   slog_trace("gui_renderer::gui_renderer()");

   m_blend_alpha.set_enabled(true);
   m_blend_alpha.rgb().set_equation_mode(gl::blend_equation_mode::func_add);
   m_blend_alpha.rgb().set_source_factor(gl::blending_factor_src::src_alpha);
   m_blend_alpha.rgb().set_destination_factor(gl::blending_factor_dest::one_minus_src_alpha);
   m_blend_alpha.alpha().set_equation_mode(gl::blend_equation_mode::func_add);
   m_blend_alpha.alpha().set_source_factor(gl::blending_factor_src::src_alpha);
   m_blend_alpha.alpha().set_destination_factor(gl::blending_factor_dest::one_minus_src_alpha);

   m_blend_add.set_enabled(true);
   m_blend_add.rgb().set_equation_mode(gl::blend_equation_mode::func_add);
   m_blend_add.rgb().set_source_factor(gl::blending_factor_src::one);
   m_blend_add.rgb().set_destination_factor(gl::blending_factor_dest::one);   
   m_blend_add.alpha().set_equation_mode(gl::blend_equation_mode::func_add);
   m_blend_add.alpha().set_source_factor(gl::blending_factor_src::one);
   m_blend_add.alpha().set_destination_factor(gl::blending_factor_dest::one);

   m_renderer = renderer;

   m_mappings = make_shared<renderstack::graphics::vertex_stream_mappings>();
   m_mappings->add("a_position",       vertex_attribute_usage::position,   0, 0);
   m_mappings->add("a_normal",         vertex_attribute_usage::normal,     0, 1);
   m_mappings->add("a_normal_flat",    vertex_attribute_usage::normal,     1, 2);
   m_mappings->add("a_normal_smooth",  vertex_attribute_usage::normal,     2, 3);
   m_mappings->add("a_color",          vertex_attribute_usage::color,      0, 4);
   m_mappings->add("a_texcoord",       vertex_attribute_usage::tex_coord,  1, 5);
   m_mappings->add("a_position_texcoord",
      static_cast<vertex_attribute_usage::value>(
         vertex_attribute_usage::position | vertex_attribute_usage::tex_coord
      ),
      0,
      0
   );

   m_vertex_format = make_shared<renderstack::graphics::vertex_format>();
   m_vertex_format->make_attribute(
      static_cast<vertex_attribute_usage::value>(
         vertex_attribute_usage::position | vertex_attribute_usage::tex_coord
      ),
      gl::vertex_attrib_pointer_type::float_,
      gl::vertex_attrib_pointer_type::float_,
      0, 
      4,
      false
   );

   auto &r = *m_renderer;

   m_vertex_stream = make_shared<renderstack::graphics::vertex_stream>();
   auto va = m_vertex_stream->vertex_array();
   auto old_va = r.set_vertex_array(va);

   m_vertex_buffer = make_shared<renderstack::graphics::buffer>(
      buffer_target::array_buffer,
      128 * 1024,
      4 * sizeof(float)
   );
   m_vertex_buffer->allocate_storage(*m_renderer);

   m_index_buffer = make_shared<renderstack::graphics::buffer>(
      buffer_target::element_array_buffer,
      512 * 1024,
      sizeof(unsigned short)
   );
   m_index_buffer->allocate_storage(*m_renderer);

   m_mappings->add_to_vertex_stream(m_vertex_stream, m_vertex_buffer, m_vertex_format);
   r.setup_attribute_pointers(m_vertex_stream, 0);
   va->set_index_buffer(m_index_buffer);

   // Just one uniform block at index 0 for gui_renderer
   m_uniform_block = make_shared<renderstack::graphics::uniform_block>(0, "gui");
   m_uniform_offsets.clip_from_model   = m_uniform_block->add_mat4 ("clip_from_model")->offset();
   m_uniform_offsets.color_add         = m_uniform_block->add_vec4 ("color_add"      )->offset();
   m_uniform_offsets.color_scale       = m_uniform_block->add_vec4 ("color_scale"    )->offset();
   m_uniform_offsets.hsv_matrix        = m_uniform_block->add_mat4 ("hsv_matrix"     )->offset();
   m_uniform_offsets.t                 = m_uniform_block->add_float("t"              )->offset();
   m_uniform_block->seal();

   m_uniform_keys.clip_from_model   = 0;
   m_uniform_keys.color_add         = 1;
   m_uniform_keys.color_scale       = 2;
   m_uniform_keys.hsv_matrix        = 3;
   m_uniform_keys.t                 = 4;

   try
   {
      string shader_path;

#if defined(RENDERSTACK_GL_API_OPENGL_ES_3)
      shader_path    = "res/shaders/sm4/";
      m_glsl_version = 300;
#else
      if ((renderstack::graphics::configuration::shader_model_version >= 5)
          && (renderstack::graphics::configuration::glsl_version >= 400))
      {
         log_trace("GUI renderer using shader model 5, GLSL 4.00");
         shader_path = "res/shaders/sm5/";
         m_glsl_version = 400;
      }
      if ((renderstack::graphics::configuration::shader_model_version >= 4)
          && (renderstack::graphics::configuration::glsl_version >= 150))
      {
         log_trace("GUI renderer using shader model 4, GLSL 1.50");
         shader_path = "res/shaders/sm4/";
         m_glsl_version = 150;
      }
      else
      {
         log_trace("GUI renderer using shader model 0, GLSL 1.20");
         shader_path = "res/shaders/sm0/";
         m_glsl_version = 120;
      }
#endif

      if (use_uniform_buffers())
      {
         log_trace("GUI renderer using uniform buffers");
         m_uniform_buffer = make_shared<buffer>(
            buffer_target::uniform_buffer,
            m_uniform_block->size(),
            1);
         m_uniform_buffer->allocate_storage(*m_renderer);

         m_uniform_buffer_range = make_shared<uniform_buffer_range>(m_uniform_block, m_uniform_buffer);
      }
      else
         log_trace("GUI renderer NOT using uniform buffers");

      auto nearest_sampler = make_shared<sampler>();
      m_samplers = make_shared<samplers>();
      m_samplers->add("font_texture",       gl::active_uniform_type::sampler_2d, nearest_sampler)->set_texture_unit_index(0);
      m_samplers->add("background_texture", gl::active_uniform_type::sampler_2d, nearest_sampler)->set_texture_unit_index(1);

      m_ninepatch_program = make_shared<renderstack::graphics::program>("ninepatch", m_glsl_version, m_samplers, m_mappings);
      m_ninepatch_program->add(m_uniform_block);
      m_ninepatch_program->load_vs(shader_path + "gui.vs.txt");
      m_ninepatch_program->load_fs(shader_path + "gui.fs.txt");
      m_ninepatch_program->link(); 
      map(m_ninepatch_program);

      m_slider_program = make_shared<renderstack::graphics::program>("slider", m_glsl_version, m_samplers, m_mappings);
      m_slider_program->add(m_uniform_block);
      m_slider_program->load_vs(shader_path + "gui_slider.vs.txt");
      m_slider_program->load_fs(shader_path + "gui_slider.fs.txt");
      m_slider_program->link();
      map(m_slider_program);

      m_font_program = make_shared<renderstack::graphics::program>("font", m_glsl_version, m_samplers, m_mappings);
      m_font_program->add(m_uniform_block);
      m_font_program->load_vs(shader_path + "gui_font.vs.txt");
      m_font_program->load_fs(shader_path + "gui_font.fs.txt");
      m_font_program->link();
      map(m_font_program);

      m_hsv_program = make_shared<renderstack::graphics::program>("hsv", m_glsl_version, m_samplers, m_mappings);
      m_hsv_program->add(m_uniform_block);
      m_hsv_program->load_vs(shader_path + "gui_hsv.vs.txt");
      m_hsv_program->load_fs(shader_path + "gui_hsv.fs.txt");
      m_hsv_program->link();
      map(m_hsv_program);
   }
   catch (...)
   {
      log_error("shaders are broken");
      throw runtime_error("gui_renderer() shaders are broken");
   }

   m_font = make_shared<font>(
      r,
      "res/fonts/Ubuntu-R.ttf",
      11,
      1.0f
   );

   m_button_ninepatch_style = make_shared<ninepatch_style>(
      r,
      "res/images/button_released.png",
      m_ninepatch_program,
      1  // texture unit
   );
   m_menulist_ninepatch_style = make_shared<ninepatch_style>(
      r,
      "res/images/shadow.png",
      m_ninepatch_program,
      1  // texture unit
   );
   m_slider_ninepatch_style = make_shared<ninepatch_style>(
      r,
      "res/images/button_released.png",
      m_slider_program,
      1  // texture unit
   );

   glm::vec2 zero(0.0f, 0.0f);
   glm::vec2 button_padding(26.0f, 6.0f);
   glm::vec2 menulist_padding(16.0f, 16.0f);
   glm::vec2 inner_padding(6.0f, 6.0f);

   m_default_style = make_shared<style>(
      glm::vec2(6.0f, 6.0f), 
      glm::vec2(6.0f, 6.0f)
   );
   m_null_padding_style = make_shared<style>(
      glm::vec2(0.0f, 0.0f), 
      glm::vec2(0.0f, 0.0f)
   );

   // With MSVC this needs _VARIADIC_MAX defined to 6 or more
   m_button_style = make_shared<renderstack::ui::style>(
      button_padding, 
      inner_padding, 
      m_font, 
      m_button_ninepatch_style,
      m_font_program
   );
   m_slider_style = make_shared<style>(
      button_padding, 
      inner_padding, 
      m_font, 
      m_slider_ninepatch_style,
      m_font_program
   );
   m_choice_style = make_shared<style>(
      zero, 
      zero, 
      m_font, 
      m_button_ninepatch_style,
      m_font_program
   );
   m_menulist_style = make_shared<style>(
      menulist_padding, 
      inner_padding, 
      nullptr, 
      m_menulist_ninepatch_style,
      nullptr
   );
   m_colorpicker_style = make_shared<style>(
      menulist_padding, 
      inner_padding, 
      nullptr, 
      m_menulist_ninepatch_style,
      m_hsv_program
   );

   r.set_vertex_array(old_va);
}

void gui_renderer::blend_alpha()
{
   m_renderer->track().blend().execute(&m_blend_alpha);
}

void gui_renderer::blend_add()
{
   m_renderer->track().blend().execute(&m_blend_add);
}

void gui_renderer::blend_disable()
{
   m_renderer->track().blend().execute(&m_blend_disabled);
}

void gui_renderer::edit_vbo()
{
   slog_trace("gui_renderer::edit_vbo()");

   m_renderer->set_buffer(buffer_target::array_buffer, m_vertex_buffer);
}

void gui_renderer::edit_ibo()
{
   slog_trace("gui_renderer::edit_ibo()");

   // Make sure our VAO is bound, then make sure our IBO is bound
   auto va = m_vertex_stream->vertex_array();
   (void)m_renderer->set_vertex_array(va);
   va->set_index_buffer(m_index_buffer);
}

void gui_renderer::prepare()
{
   slog_trace("gui_renderer::prepare()");

   // Default state
   auto &r = *m_renderer;
   state_trackers &t = r.track();
   //m_renderer->track.execute(&m_gui_render_states);
   t.color_mask().execute(&m_gui_render_states.color_mask);
   t.blend().execute(&m_gui_render_states.blend);
   t.depth().execute(&m_gui_render_states.depth);
   t.face_cull().execute(&m_gui_render_states.face_cull);
   t.stencil().execute(&m_gui_render_states.stencil);

   blend_alpha();

   // We bind these for both render and edit uses

   m_renderer->use_vertex_stream(m_vertex_stream);
   m_renderer->set_buffer(buffer_target::array_buffer, m_vertex_buffer);
   m_vertex_stream->vertex_array()->set_index_buffer(m_index_buffer);

   if (use_uniform_buffers())
   {
      assert(m_uniform_buffer_range);
      m_renderer->set_uniform_buffer_range(
         m_uniform_block->binding_point(), 
         m_uniform_buffer_range
      );
   }
}
void gui_renderer::on_resize(int width, int height)
{
   m_ortho = glm::ortho(0.0f, (float)width, 0.0f, (float)height);
}
void gui_renderer::begin_edit()
{
   if (use_uniform_buffers())
   {
      log_trace("gui_renderer::begin_edit() - using uniform buffers");

      assert(m_uniform_buffer_range);
      m_renderer->set_buffer(buffer_target::uniform_buffer, m_uniform_buffer);
      m_start = m_uniform_buffer_range->begin_edit(*m_renderer);
   }
}
void gui_renderer::end_edit()
{
   if (use_uniform_buffers())
   {
      log_trace("gui_renderer::end_edit() - using uniform buffers");
      assert(m_uniform_buffer_range);
      m_uniform_buffer_range->end_edit(*m_renderer);
      m_start = nullptr;
   }
}
void gui_renderer::set_program(shared_ptr<program> value)
{
   slog_trace("gui_renderer::set_program(%s)", value->name().c_str());

   assert(value);
   m_renderer->set_program(value);
   m_program = value;
}
void gui_renderer::set_texture(unsigned int unit, shared_ptr<class texture> texture)
{
   slog_trace("gui_renderer::set_texture(unit = %u, texture = %u)", unit, texture.get());

   (void)m_renderer->set_texture(unit, texture);
}
void gui_renderer::set_transform(glm::mat4 const &value)
{
   // slog_trace("gui_renderer::set_transform(...)"); // TODO format glm::mat4?

   if (use_uniform_buffers())
   {
      assert(m_start);
      ::memcpy(&m_start[m_uniform_offsets.clip_from_model], value_ptr(value), 16 * sizeof(float));
   }
   else
   {
      assert(m_program);
      gl::uniform_matrix_4fv(
         m_program->uniform_at(m_uniform_keys.clip_from_model), 
         1, 
         GL_FALSE, 
         value_ptr(value)
      );
   }
}
void gui_renderer::set_color_add(glm::vec4 const &value)
{
   if (use_uniform_buffers())
   {
      assert(m_start);
      ::memcpy(&m_start[m_uniform_offsets.color_add], value_ptr(value), 4 * sizeof(float));
   }
   else
   {
      assert(m_program);
      gl::uniform_4fv(
         m_program->uniform_at(m_uniform_keys.color_add), 1, value_ptr(value)
      );
   }
}
void gui_renderer::set_color_scale(glm::vec4 const &value)
{
   if (use_uniform_buffers())
   {
      assert(m_start);
      ::memcpy(&m_start[m_uniform_offsets.color_scale], value_ptr(value), 4 * sizeof(float));
   }
   else
   {
      assert(m_program);
      gl::uniform_4fv(
         m_program->uniform_at(m_uniform_keys.color_scale), 1, value_ptr(value)
      );
   }
}
void gui_renderer::set_hsv_matrix(glm::mat4 const &value)
{
   if (use_uniform_buffers())
   {
      assert(m_start);
      ::memcpy(&m_start[m_uniform_offsets.hsv_matrix], value_ptr(value), 16 * sizeof(float));
   }
   else
   {
      assert(m_program);
      gl::uniform_matrix_4fv(
         m_program->uniform_at(
         m_uniform_keys.hsv_matrix
         ), 
         1, 
         GL_FALSE, 
         value_ptr(value)
      );
   }
}
void gui_renderer::set_t(float value)
{
   if (use_uniform_buffers())
   {
      assert(m_start);
      float *data = reinterpret_cast<float*>(&m_start[m_uniform_offsets.t]);
      *data = value;
   }
   else
   {
      assert(m_program);
      gl::uniform_1f(m_program->uniform_at(m_uniform_keys.t), value);
   }
}

void gui_renderer::draw_elements_base_vertex(
   GLenum begin_mode, GLsizei count, GLenum index_type, const GLvoid *indices,
   GLint base_vertex)
{
   // TODO Can we do this in caller somewhere?
   auto va = m_vertex_stream->vertex_array();
   (void)m_renderer->set_vertex_array(va);
   (void)va->set_index_buffer(m_index_buffer);

   m_renderer->draw_elements_base_vertex(m_vertex_stream, begin_mode, count, index_type, indices, base_vertex);
}

} }
