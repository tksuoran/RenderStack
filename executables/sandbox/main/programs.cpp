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
#include <sys/stat.h>

#define LOG_CATEGORY &log_programs


using namespace renderstack::toolkit;
using namespace renderstack::graphics;
using namespace std;


int programs::glsl_version() const
{
   return m_glsl_version;
}

programs::programs()
:  service("programs")
,  uniform_buffer    (nullptr)
,  default_block     (nullptr)
,  model_block       (nullptr)
,  camera_block      (nullptr)
,  material_block    (nullptr)
,  lights_block      (nullptr)
,  debug_block       (nullptr)

,  model_ubr         (nullptr)
,  camera_ubr        (nullptr)
,  material_ubr      (nullptr)
,  lights_ubr        (nullptr)
,  debug_ubr         (nullptr)

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

   size_t ubo_size = 0;

   default_block = make_shared<uniform_block>("");
   default_block->seal();

   model_block = make_shared<uniform_block>(0, "model");
   model_block_access.clip_from_model  = model_block->add_mat4("clip_from_model"   )->access();
   model_block_access.world_from_model = model_block->add_mat4("world_from_model"  )->access();
   model_block_access.view_from_model  = model_block->add_mat4("view_from_model"   )->access();
   model_block_access.id_offset        = model_block->add_vec3("id_offset"         )->access();
   model_block->seal();
   ubo_size += model_block->size_bytes();

   camera_block = make_shared<uniform_block>(1, "camera");
   camera_block_access.world_from_view = camera_block->add_mat4("world_from_view" )->access();
   camera_block_access.world_from_clip = camera_block->add_mat4("world_from_clip" )->access();
   camera_block_access.viewport        = camera_block->add_vec4("viewport"        )->access();
   camera_block->seal();
   ubo_size += camera_block->size_bytes();

   material_block = make_shared<uniform_block>(2, "material");
   material_block_access.color      = material_block->add_vec4 ("color"    )->access();
   material_block_access.roughness  = material_block->add_float("roughness")->access();
   material_block_access.isotropy   = material_block->add_float("isotropy" )->access();
   material_block->seal();
   ubo_size += material_block->size_bytes();

   lights_block = make_shared<uniform_block>(3, "lights");
   lights_block_access.exposure  = lights_block->add_float("exposure")->access();
   lights_block_access.position  = lights_block->add_vec3("position")->access();
   lights_block_access.direction = lights_block->add_vec3("direction")->access();
   lights_block_access.radiance  = lights_block->add_vec3("radiance")->access();
   lights_block->seal();
   ubo_size += lights_block->size_bytes();

   debug_block = make_shared<uniform_block>(4, "debug");
   debug_block_access.line_width         = debug_block->add_vec4("line_width"         )->access();
   debug_block_access.show_rt_transform  = debug_block->add_vec4("show_rt_transform"  )->access();
   ubo_size += debug_block->size_bytes();

   auto &r = *m_renderer;

   uniform_buffer = make_shared<buffer>(
      renderstack::graphics::buffer_target::uniform_buffer,
      ubo_size,
      1
   );
   uniform_buffer->allocate_storage(r);

   if (renderstack::graphics::configuration::can_use.uniform_buffer_object)
   {
      model_ubr      = make_shared<uniform_buffer_range>(model_block,      this->uniform_buffer);
      camera_ubr     = make_shared<uniform_buffer_range>(camera_block,     this->uniform_buffer);
      material_ubr   = make_shared<uniform_buffer_range>(material_block,   this->uniform_buffer);
      lights_ubr     = make_shared<uniform_buffer_range>(lights_block,     this->uniform_buffer);
      debug_ubr      = make_shared<uniform_buffer_range>(debug_block,      this->uniform_buffer);
   }

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

void programs::bind_uniforms()
{
   auto &r = *m_renderer;
   r.set_uniform_buffer_range(model_block->binding_point(),    model_ubr);
   r.set_uniform_buffer_range(camera_block->binding_point(),   camera_ubr);
   r.set_uniform_buffer_range(material_block->binding_point(), material_ubr);
   r.set_uniform_buffer_range(lights_block->binding_point(),   lights_ubr);
   r.set_uniform_buffer_range(debug_block->binding_point(),    debug_ubr);
}

unsigned char *programs::begin_edit_uniforms()
{
   auto &r = *m_renderer;
   bind_uniforms();
   void *start = uniform_buffer->map(
      r, 
      0, 
      uniform_buffer->capacity(), 
      static_cast<gl::buffer_access_mask::value>(
         gl::buffer_access_mask::map_write_bit | gl::buffer_access_mask::map_flush_explicit_bit
      )
   );

   return static_cast<unsigned char*>(start);
}

void programs::end_edit_uniforms()
{
   auto &r = *m_renderer;
   uniform_buffer->unmap(r);
}

shared_ptr<renderstack::graphics::program> programs::make_program(string const &name)
{
   auto p = make_shared<program>(name, m_glsl_version, samplers, attribute_mappings, fragment_outputs);
   p->add(default_block);
   p->add(model_block);
   p->add(camera_block);
   p->add(material_block);
   p->add(lights_block);
   p->add(debug_block);
   p->load_vs(m_shader_path + name + ".vs.txt");
   p->load_fs(m_shader_path + name + ".fs.txt");
   p->link(); 

   if (m_shader_monitor)
   {
      m_shader_monitor->add(m_shader_path + name + ".vs.txt", p);
      m_shader_monitor->add(m_shader_path + name + ".fs.txt", p);
   }
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
