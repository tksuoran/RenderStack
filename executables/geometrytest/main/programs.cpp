#include "renderstack_toolkit/platform.hpp"
#include "main/application.hpp"
#include "main/programs.hpp"
#include "renderstack_graphics/configuration.hpp"
#include "renderstack_graphics/program.hpp"
#include "renderstack_graphics/renderer.hpp"
#include "renderstack_graphics/sampler.hpp"
#include "renderstack_graphics/samplers.hpp"
#include "renderstack_graphics/shader_monitor.hpp"
#include "renderstack_graphics/uniform_block.hpp"
#include "renderstack_graphics/uniform_buffer_range.hpp"
#include "renderstack_graphics/uniform.hpp"
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
   if (!use_uniform_buffers())
   {
      //program->dump_shaders();
      program->map_uniform(uniform_keys.clip_from_model    , "global_clip_from_model"     );
      program->map_uniform(uniform_keys.world_from_model   , "global_world_from_model"    );
      program->map_uniform(uniform_keys.world_from_view    , "global_world_from_view"     );
      program->map_uniform(uniform_keys.view_from_model    , "global_view_from_model"     );
      program->map_uniform(uniform_keys.viewport           , "global_viewport"            );
      program->map_uniform(uniform_keys.color              , "global_color"               );
      program->map_uniform(uniform_keys.line_width         , "global_line_width"          );
      program->map_uniform(uniform_keys.material_parameters, "global_material_parameters" );
      program->map_uniform(uniform_keys.show_rt_transform  , "global_show_rt_transform"   );
      program->map_uniform(uniform_keys.id_offset          , "global_id_offset"           );
   }
}

int programs::glsl_version() const
{
   return m_glsl_version;
}

void programs::prepare_gl_resources()
{
   slog_trace("programs::prepare_gl_resources()");

#if 0
   try
   {
      string src_path = read("res/src_path.txt");
      string dst_path = read("res/dst_path.txt");
      //auto monitor = context::current()->shader_monitor();
      //monitor.set_dst_path(dst_path);
      //monitor.set_src_path(src_path);
   }
   catch (...)
   {
   }
#endif

   mappings = make_shared<renderstack::graphics::vertex_stream_mappings>();
   mappings->add("a_position",            vertex_attribute_usage::position,   0, 0);
   mappings->add("a_normal",              vertex_attribute_usage::normal,     0, 1);
   mappings->add("a_normal_flat",         vertex_attribute_usage::normal,     1, 2);
   mappings->add("a_normal_smooth",       vertex_attribute_usage::normal,     2, 3);
   mappings->add("a_color",               vertex_attribute_usage::color,      0, 4);
   mappings->add("a_texcoord",            vertex_attribute_usage::tex_coord,  1, 5);
   mappings->add("a_id",                  vertex_attribute_usage::id,         3, 6);
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

   block = make_shared<uniform_block>(1, "global");
   uniform_offsets.clip_from_model     = block->add_mat4("clip_from_model"    )->offset();
   uniform_offsets.world_from_model    = block->add_mat4("world_from_model"   )->offset();
   uniform_offsets.world_from_view     = block->add_mat4("world_from_view"    )->offset();
   uniform_offsets.view_from_model     = block->add_mat4("view_from_model"    )->offset();
   uniform_offsets.color               = block->add_vec4("color"              )->offset();
   uniform_offsets.line_width          = block->add_vec4("line_width"         )->offset();
   uniform_offsets.viewport            = block->add_vec4("viewport"           )->offset();
   uniform_offsets.material_parameters = block->add_vec4("material_parameters")->offset();
   uniform_offsets.show_rt_transform   = block->add_vec4("show_rt_transform"  )->offset();
   uniform_offsets.id_offset           = block->add_vec3("id_offset"          )->offset();
   block->seal();

   uniform_keys.clip_from_model     = 0;
   uniform_keys.world_from_model    = 1;
   uniform_keys.world_from_view     = 2;
   uniform_keys.view_from_model     = 3;
   uniform_keys.viewport            = 4;
   uniform_keys.color               = 5;
   uniform_keys.line_width          = 6;
   uniform_keys.material_parameters = 7;
   uniform_keys.show_rt_transform   = 8;
   uniform_keys.id_offset           = 9;

   auto nearest_sampler = make_shared<sampler>();
   auto show_rt_sampler = make_shared<sampler>();
   show_rt_sampler->set_mag_filter(gl::texture_mag_filter::nearest);
   show_rt_sampler->set_min_filter(gl::texture_min_filter::linear);

   samplers = make_shared<class samplers>();
   samplers->add("font_texture",             gl::active_uniform_type::sampler_2d, nearest_sampler)->set_texture_unit_index(0);
   samplers->add("background_texture",       gl::active_uniform_type::sampler_2d, nearest_sampler)->set_texture_unit_index(1);
   samplers->add("emission_texture",         gl::active_uniform_type::sampler_2d, nearest_sampler)->set_texture_unit_index(0);
   samplers->add("albedo_texture",           gl::active_uniform_type::sampler_2d, nearest_sampler)->set_texture_unit_index(1);
   samplers->add("normal_tangent_texture",   gl::active_uniform_type::sampler_2d, nearest_sampler)->set_texture_unit_index(2);
   samplers->add("material_texture",         gl::active_uniform_type::sampler_2d, nearest_sampler)->set_texture_unit_index(3);
   samplers->add("show_rt_texture",          gl::active_uniform_type::sampler_2d, show_rt_sampler)->set_texture_unit_index(0);

   try
   {
#if defined(RENDERSTACK_GL_API_OPENGL_ES_3)
      m_shader_path  = "res/shaders/sm4/";
      m_glsl_version = 300;
#else
      if ((renderstack::graphics::configuration::shader_model_version >= 5)
          && (renderstack::graphics::configuration::glsl_version >= 400))
      {
         log_trace("Using shader model 5, GLSL 4.00");

         m_shader_path  = "res/shaders/sm5/";
         m_glsl_version = 400;
      }
      else if ((renderstack::graphics::configuration::shader_model_version >= 4)
          && (renderstack::graphics::configuration::glsl_version >= 150))
      {
         log_trace("Using shader model 4, GLSL 1.50");

         m_shader_path  = "res/shaders/sm4/";
         m_glsl_version = 150;
      }
      else
      {
         log_trace("Using shader model 0, GLSL 1.20");

         m_shader_path  = "res/shaders/sm0/";
         m_glsl_version = 120;
      }
#endif

      font              = make_program("font");
      basic             = make_program("basic");
      gbuffer           = make_program("gbuffer");
      light             = make_program("light");
      show_rt           = make_program("show_rt");
      show_rt_spherical = make_program("show_rt_spherical");
      textured          = make_program("textured");
      id                = make_program("id");
   }
   catch (...)
   {
      log_error("shaders are broken\n");
      throw runtime_error("shaders are broken");
   }
}

shared_ptr<renderstack::graphics::program> programs::make_program(string const &name)
{
   auto p = make_shared<program>(name, m_glsl_version, samplers, mappings);
   p->add(block);
   p->load_vs(m_shader_path + name + ".vs.txt");
   p->load_fs(m_shader_path + name + ".fs.txt");
   p->link(); 
   map(p);
   return p;
}

bool programs::use_uniform_buffers() const
{
   // Test all conditions; can_use.uniform_buffer_object can be forced to false
   bool use_uniform_buffers = 
      renderstack::graphics::configuration::can_use.uniform_buffer_object &&
      (glsl_version() >= 140) &&
      (renderstack::graphics::configuration::shader_model_version >= 4);
   return use_uniform_buffers;
}

void programs::update_fixed_step()
{
#if 0
   ++m_poll_ticks;
   if (m_poll_ticks == 30)
   {
      auto monitor = context::current()->shader_monitor();
      monitor.poll();
      m_poll_ticks = 0;
   }
#endif
}
