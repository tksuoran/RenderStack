#include "renderstack_toolkit/platform.hpp"
#include "main/application.hpp"
#include "main/programs.hpp"
#include "renderstack_graphics/configuration.hpp"
#include "renderstack_graphics/context.hpp"
#include "renderstack_graphics/shader_monitor.hpp"
#include "renderstack_graphics/program.hpp"
#include "renderstack_graphics/sampler.hpp"
#include "renderstack_graphics/uniform_block.hpp"
#include "renderstack_graphics/uniform_buffer_range.hpp"
#include "renderstack_graphics/uniform.hpp"
#include "renderstack_graphics/samplers.hpp"
#include "renderstack_renderer/renderer.hpp"
#include "renderstack_toolkit/file.hpp"
#include "renderstack_toolkit/logstream.hpp"
#include <string>
#include <fstream>
#include <stdexcept>
#include <sstream>
#include <sys/stat.h>

#define LOG_CATEGORY &log_programs

using namespace std;
using namespace renderstack::graphics;
using namespace renderstack::toolkit;

void programs::map(shared_ptr<renderstack::graphics::program> program)
{
   // Test both conditions; m_glsl_version >= 140 is strict requirement,
   // can_use.uniform_buffer_object can be forced to false
   bool use_uniform_buffers = renderstack::graphics::configuration::can_use.uniform_buffer_object && glsl_version() >= 140;

   if (!use_uniform_buffers)
   {
      //program->dump_shaders();
      program->map_uniform("global_model_to_clip");
      program->map_uniform("global_viewport"     );
      program->map_uniform("global_color"        );
      program->map_uniform("global_line_width"   );
      program->map_uniform("global_line_color"   );
   }
}

int programs::glsl_version() const
{
   return m_glsl_version;
}

void programs::prepare_gl_resources()
{
   slog_trace("programs::prepare_gl_resources()");

   string src_path = read("res/src_path.txt");
   string dst_path = read("res/dst_path.txt");

   auto monitor = context::current()->shader_monitor();
   monitor.set_dst_path(dst_path);
   monitor.set_src_path(src_path);

   mappings = renderstack::graphics::context::current()->make_vertex_stream_mappings("renderer s_mappings");
   mappings->add("a_position",            vertex_attribute_usage::position,   0, 0);
   mappings->add("a_normal",              vertex_attribute_usage::normal,     0, 1);
   mappings->add("a_normal_flat",         vertex_attribute_usage::normal,     1, 2);
   mappings->add("a_normal_smooth",       vertex_attribute_usage::normal,     2, 3);
   mappings->add("a_color",               vertex_attribute_usage::color,      0, 4);
   mappings->add("a_texcoord",            vertex_attribute_usage::tex_coord,  1, 5);
   mappings->add(
      "a_position_texcoord",
      static_cast<vertex_attribute_usage::value>(
         vertex_attribute_usage::position | vertex_attribute_usage::tex_coord
      ),
      0,
      0
   );

   //mappings->add("a_position_texcoord",   vertex_attribute_usage::position,   0, 0);
   /*mappings->add(
      "a_position_texcoord",
      vertex_attribute_usage::position_texcoord,
      0, 6);*/

   m_poll_shaders = false;
   m_poll_ticks = 0; 

   block = renderstack::graphics::context::current()->make_uniform_block("global");
   uniform_offsets.model_to_clip = block->add_mat4("model_to_clip")->offset();
   uniform_offsets.viewport      = block->add_vec4("viewport"     )->offset();
   uniform_offsets.color         = block->add_vec4("color"        )->offset();
   uniform_offsets.line_width    = block->add_vec4("line_width"   )->offset();
   block->seal();

   uniform_keys.model_to_clip = 0;
   uniform_keys.viewport      = 1;
   uniform_keys.color         = 2;
   uniform_keys.line_width    = 3;

   auto nearest_sampler = make_shared<sampler>();
   this->samplers = context::current()->global_samplers();
   samplers->add("font_texture",          gl::active_uniform_type::sampler_2d, nearest_sampler)->set_texture_unit_index(0);
   samplers->add("background_texture",    gl::active_uniform_type::sampler_2d, nearest_sampler)->set_texture_unit_index(1);

   try
   {
      std::string shader_path;

      if ((renderstack::graphics::configuration::shader_model_version >= 5)
          && (renderstack::graphics::configuration::glsl_version >= 400))
      {
         log_trace("Using shader model 5, GLSL 4.00");

         shader_path    = "res/shaders/sm5/";
         m_glsl_version = 400;
      }
      if ((renderstack::graphics::configuration::shader_model_version >= 4)
          && (renderstack::graphics::configuration::glsl_version >= 150))
      {
         log_trace("Using shader model 4, GLSL 1.50");

         shader_path    = "res/shaders/sm4/";
         m_glsl_version = 150;
      }
      else
      {
         log_trace("Using shader model 0, GLSL 1.20");

         shader_path    = "res/shaders/sm0/";
         m_glsl_version = 120;
      }

      font = make_shared<program>("font", m_glsl_version, samplers, mappings);
      font->add(block);
      font->load_vs(shader_path + "font.vs.txt");
      font->load_fs(shader_path + "font.fs.txt");
      font->link();
      map(font);

      basic = make_shared<program>("basic", m_glsl_version, samplers, mappings);
      basic->add(block);
      basic->load_vs(shader_path + "basic.vs.txt");
      basic->load_fs(shader_path + "basic.fs.txt");
      basic->link();
      map(basic);

      textured = make_shared<program>("textured", m_glsl_version, samplers, mappings);
      textured->add(block);
      textured->load_vs(shader_path + "textured.vs.txt");
      textured->load_fs(shader_path + "textured.fs.txt");
      textured->link(); 
      map(textured);

   }
   catch (...)
   {
      log_error("shaders are broken\n");
      throw runtime_error("shaders are broken");
   }
}

void programs::update_fixed_step()
{
   ++m_poll_ticks;
   if (m_poll_ticks == 30)
   {
      auto monitor = context::current()->shader_monitor();
      monitor.poll();
      m_poll_ticks = 0;
   }
}
