#include "renderstack_toolkit/platform.hpp"
#include "renderstack_toolkit/file.hpp"
#include "renderstack_graphics/configuration.hpp"
#include "renderstack_graphics/fragment_outputs.hpp"
#include "renderstack_graphics/program.hpp"
#include "renderstack_graphics/renderer.hpp"
#include "renderstack_graphics/sampler.hpp"
#include "renderstack_graphics/samplers.hpp"
#include "renderstack_graphics/shader_monitor.hpp"
#include "renderstack_graphics/uniform_block.hpp"
#include "renderstack_graphics/uniform_buffer_range.hpp"
#include "renderstack_graphics/uniform.hpp"

#include "main/application.hpp"
#include "main/programs.hpp"
#include "main/log.hpp"

#include <string>
#include <fstream>
#include <stdexcept>
#include <sstream>
#include <utility>
#include <sys/stat.h>

#define LOG_CATEGORY &log_programs


using namespace renderstack::toolkit;
using namespace renderstack::graphics;
using namespace std;


programs::programs()
:  service("programs")
,  model_block       (nullptr)
,  camera_block      (nullptr)
,  material_block    (nullptr)
,  lights_block      (nullptr)
,  debug_block       (nullptr)

,  samplers          (nullptr)
,  attribute_mappings(nullptr)
,  fragment_outputs  (nullptr)
,  font              (nullptr)
,  basic             (nullptr)
,  debug_line        (nullptr)
,  textured          (nullptr)
,  gbuffer           (nullptr)
,  light             (nullptr)
,  show_rt           (nullptr)
,  show_rt_spherical (nullptr)
,  id                (nullptr)
{
}

/*virtual*/ programs::~programs()
{
}

void programs::connect(
   shared_ptr<renderstack::graphics::renderer>        renderer_,
   shared_ptr<renderstack::graphics::shader_monitor>  shader_monitor_
)
{
   m_renderer = renderer_;
   m_shader_monitor = shader_monitor_;

   initialization_depends_on(renderer_);
   initialization_depends_on(shader_monitor_);
}

/*virtual*/ void programs::initialize_service()
{
   assert(m_renderer);

   slog_trace("programs::initialize_service()");

   if (m_shader_monitor)
   {
      try
      {
         string src_path;
         string dst_path;

         if (exists("res/src_path.txt"))
         {
            src_path = read("res/src_path.txt");
            dst_path = read("res/dst_path.txt");
         }
         else
         {
            src_path = "C:/git/RenderStack/executables/sandbox";
            dst_path = "";
         }

         m_shader_monitor->set_dst_path(dst_path);
         m_shader_monitor->set_src_path(src_path);
      }
      catch (...)
      {
         slog_warn("programs::initialize_service() setting up shader monitor failed");
      }
   }

   fragment_outputs = make_shared<class fragment_outputs>();
   fragment_outputs->add("out_id",              gl::fragment_output_type::float_vec4, 0);
   fragment_outputs->add("out_color",           gl::fragment_output_type::float_vec4, 0);
   fragment_outputs->add("out_emission",        gl::fragment_output_type::float_vec4, 0);
   fragment_outputs->add("out_albedo",          gl::fragment_output_type::float_vec4, 1);
   fragment_outputs->add("out_normal_tangent",  gl::fragment_output_type::float_vec4, 2);
   fragment_outputs->add("out_material",        gl::fragment_output_type::float_vec4, 3);

   attribute_mappings = make_shared<renderstack::graphics::vertex_attribute_mappings>();
   attribute_mappings->add("a_position",            vertex_attribute_usage::position,   0, 0);
   attribute_mappings->add("a_normal",              vertex_attribute_usage::normal,     0, 1);
   attribute_mappings->add("a_normal_flat",         vertex_attribute_usage::normal,     1, 2);
   attribute_mappings->add("a_normal_smooth",       vertex_attribute_usage::normal,     2, 3);
   attribute_mappings->add("a_tangent",             vertex_attribute_usage::tangent,    0, 4);
   attribute_mappings->add("a_color",               vertex_attribute_usage::color,      0, 5);
   attribute_mappings->add("a_texcoord",            vertex_attribute_usage::tex_coord,  0, 6);
   attribute_mappings->add("a_id",                  vertex_attribute_usage::id,         0, 7);
   attribute_mappings->add(
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

   model_block = make_shared<uniform_block>(0, "model");
   model_block_access.clip_from_model  = model_block->add_mat4("clip_from_model"   )->access();
   model_block_access.world_from_model = model_block->add_mat4("world_from_model"  )->access();
   model_block_access.view_from_model  = model_block->add_mat4("view_from_model"   )->access();
   model_block_access.id_offset        = model_block->add_vec3("id_offset"         )->access();
   model_block->seal();

   camera_block = make_shared<uniform_block>(1, "camera");
   camera_block_access.world_from_view = camera_block->add_mat4 ("world_from_view" )->access();
   camera_block_access.world_from_clip = camera_block->add_mat4 ("world_from_clip" )->access();
   camera_block_access.viewport        = camera_block->add_vec4 ("viewport"        )->access();
   camera_block_access.exposure        = camera_block->add_float("exposure"        )->access();
   camera_block->seal();

   material_block = make_shared<uniform_block>(2, "material");
   material_block_access.color      = material_block->add_vec4 ("color"    )->access();
   material_block_access.roughness  = material_block->add_float("roughness")->access();
   material_block_access.isotropy   = material_block->add_float("isotropy" )->access();
   material_block->seal();

   lights_block = make_shared<uniform_block>(3, "lights");
   lights_block_access.position  = lights_block->add_vec3("position" )->access();
   lights_block_access.direction = lights_block->add_vec3("direction")->access();
   lights_block_access.radiance  = lights_block->add_vec3("radiance" )->access();
   lights_block->seal();

   debug_block = make_shared<uniform_block>(4, "debug");
   debug_block_access.line_width         = debug_block->add_vec4("line_width"         )->access();
   debug_block_access.show_rt_transform  = debug_block->add_vec4("show_rt_transform"  )->access();

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
   samplers->add("depth_texture",            gl::active_uniform_type::sampler_2d, nearest_sampler)->set_texture_unit_index(4);
   samplers->add("show_rt_texture",          gl::active_uniform_type::sampler_2d, show_rt_sampler)->set_texture_unit_index(0);

   m_shader_path = "res/shaders/";

#if defined(RENDERSTACK_GL_API_OPENGL_ES_3)
      m_shader_versions.push_back(make_pair("4", 300));
#else
   if (
      (renderstack::graphics::configuration::shader_model_version >= 5) &&
      (renderstack::graphics::configuration::glsl_version >= 400)
   )
      m_shader_versions.push_back(make_pair("5", 400));

   if (
      (renderstack::graphics::configuration::shader_model_version >= 4) &&
      (renderstack::graphics::configuration::glsl_version >= 150)
   )
      m_shader_versions.push_back(make_pair("4", 150));

   m_shader_versions.push_back(make_pair("0", 120));
#endif

   try
   {
      font              = make_program("font");
      basic             = make_program("basic");
      gbuffer           = make_program("gbuffer");
      light             = make_program("light");
      show_rt           = make_program("show_rt");
      show_rt_spherical = make_program("show_rt_spherical");
      textured          = make_program("textured");
      id                = make_program("id");
      debug_line        = make_program("debug_line");
      debug_light       = make_program("debug_light");
      anisotropic       = make_program("anisotropic");
   }
   catch (runtime_error const &e)
   {
      log_error("programs::initialize_service() - aborting");
      throw e;
   }
}

shared_ptr<renderstack::graphics::program> programs::make_program(string const &name)
{
   for (auto i = m_shader_versions.cbegin(); i != m_shader_versions.cend(); ++i)
   {
      string vs_path = m_shader_path + name + ".vs" + i->first + ".txt";
      string fs_path = m_shader_path + name + ".fs" + i->first + ".txt";

      if (!exists(vs_path) || !exists(fs_path))
         continue;

      auto p = make_shared<program>(name, i->second, samplers, attribute_mappings, fragment_outputs);
      p->add(model_block);
      p->add(camera_block);
      p->add(material_block);
      p->add(lights_block);
      p->add(debug_block);
      p->load_vs(vs_path);
      p->load_fs(fs_path);
      p->link(); 

      if (m_shader_monitor)
      {
         m_shader_monitor->add(vs_path, p);
         m_shader_monitor->add(fs_path, p);
      }
      return p;
   }

   stringstream ss;
   ss << "programs::make_program(" << name << ") failed";
   log_error("%s", ss.str().c_str());
   throw runtime_error(ss.str());
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
