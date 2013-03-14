#include "renderstack_toolkit/platform.hpp"

#include "renderstack_graphics/context.hpp"
#include "renderstack_graphics/vertex_format.hpp"
#include "renderstack_renderer/renderer.hpp"
#include "renderstack_mesh/mesh.hpp"
#include "renderstack_mesh/geometry_mesh.hpp"

#include "renderstack_toolkit/logstream.hpp"

#define LOG_CATEGORY &log_renderer

namespace renderstack { namespace renderer {

using namespace std;
using namespace renderstack::graphics;
using namespace renderstack::mesh;

bool render_states::lock_blend_state;
bool render_states::lock_face_cull_state;
bool render_states::lock_depth_state;
bool render_states::lock_color_mask_state;
bool render_states::lock_stencil_state;

void render_states::use()
{
   if (!lock_blend_state      ) m_blend_state->execute();
   if (!lock_face_cull_state  ) m_face_cull_state->execute();
   if (!lock_depth_state      ) m_depth_state->execute();
   if (!lock_color_mask_state ) m_color_mask_state->execute();
   if (!lock_stencil_state    ) m_stencil_state->execute();
}

state::state()
{
   for (int i = 0; i < TEXTURE_UNIT_COUNT; ++i)
   {
      textures[i] = 0;
      texture_target[i] = gl::texture_target::texture_2d;
   }
}
state::state(state const &other)
:  render_states  (other.render_states)
,  program        (other.program)
,  vertex_buffer  (other.vertex_buffer)
,  index_buffer   (other.index_buffer)
{
   for (int i = 0; i < TEXTURE_UNIT_COUNT; ++i)
   {
      textures[i] = other.textures[i];
      texture_target[i] = other.texture_target[i];
   }
   for (int i = 0; i < UNIFORM_BINDING_POINT_COUNT; ++i)
   {
      uniform_buffer_ranges[i] = other.uniform_buffer_ranges[i];
   }
}



renderer::renderer()
:  s_active_texture_unit(0)
{
}

shared_ptr<renderstack::graphics::vertex_stream_mappings> renderer::mappings()
{ 
   if (!s_mappings)
   {
      s_mappings = renderstack::graphics::context::current()->make_vertex_stream_mappings("renderer s_mappings");
      // if (vertex_format.has_attribute((*mapping)->src_usage(), (*mapping)->src_index())) {
      s_mappings->add("a_position",       vertex_attribute_usage::position,   0, 0);
      s_mappings->add("a_normal",         vertex_attribute_usage::normal,     0, 1);
      s_mappings->add("a_normal_flat",    vertex_attribute_usage::normal,     1, 2);
      s_mappings->add("a_normal_smooth",  vertex_attribute_usage::normal,     2, 3);
      s_mappings->add("a_color",          vertex_attribute_usage::color,      0, 4);
      s_mappings->add("a_texcoord",       vertex_attribute_usage::tex_coord,  1, 5);
   }
   return s_mappings; 
}

void renderer::push()
{
   s_request_stack.push(s_requested);
}
void renderer::pop()
{
   s_requested = s_request_stack.top();
   s_request_stack.pop();
}
void renderer::lock_material(bool value)
{
   s_lock_material = value; 
}
void renderer::update_render_states()
{
   if (s_effective.render_states != s_requested.render_states)
   {
      s_requested.render_states->use();
      s_effective.render_states = s_requested.render_states;
   }
}
void renderer::trash()
{
   slog_trace("renderer::trash()");

   s_active_texture_unit = ~0u;
   s_effective.program.reset();
   s_effective.vertex_buffer.reset();
   s_effective.index_buffer.reset();
   for (int i = 0; i < TEXTURE_UNIT_COUNT; ++i)
   {
      s_effective.textures[i] = 0;
      s_effective.texture_target[i] = static_cast<gl::texture_target::value>(0);
   }
   for (int i = 0; i < UNIFORM_BINDING_POINT_COUNT; ++i)
   {
      s_effective.uniform_buffer_ranges[i].reset();
   }
}
void renderer::set_program(shared_ptr<class program> program)
{
   assert(program);

   s_requested.program = program;
   if (s_effective.program != s_requested.program)
   {
      if (s_requested.program)
         s_requested.program->use();

      s_effective.program = s_requested.program;
   }
}
void renderer::set_vertex_buffer(shared_ptr<class vertex_buffer> vertex_buffer)
{
   assert(vertex_buffer);

   s_requested.vertex_buffer = vertex_buffer;
   if (s_effective.vertex_buffer != s_requested.vertex_buffer)
   {
      if (s_requested.vertex_buffer)
         s_requested.vertex_buffer->bind();

      s_effective.vertex_buffer = s_requested.vertex_buffer;
      //  This makes sure we always set vbo pool before setting vertex stream.
      //  We need to first bind VBO before calling vertex attribute pointers,
      //  because they memorize the current VBO.
      s_effective.vertex_stream = nullptr;
   }
}
void renderer::set_index_buffer(std::shared_ptr<class index_buffer> index_buffer)
{
   assert(index_buffer);

   s_requested.index_buffer = index_buffer;
   if (s_effective.index_buffer != s_requested.index_buffer)
   {
      if (s_requested.index_buffer)
         s_requested.index_buffer->bind();

      s_effective.index_buffer = s_requested.index_buffer;
   }
}
void renderer::set_texture(unsigned int unit, gl::texture_target::value target, unsigned int texture)
{
   s_requested.textures      [unit] = texture;
   s_requested.texture_target[unit] = target;

   if (
      (s_effective.textures      [unit] != s_requested.textures      [unit]) ||
      (s_effective.texture_target[unit] != s_requested.texture_target[unit])
   )
   {
      if (s_active_texture_unit != unit)
      {
         gl::active_texture(gl::texture_unit::texture0 + unit);
         s_active_texture_unit = unit;
      }
      gl::bind_texture(s_requested.texture_target[unit], s_requested.textures[unit]);
      s_effective.textures      [unit] = s_requested.textures[unit];
      s_effective.texture_target[unit] = s_requested.texture_target[unit];
   }
}
void renderer::set_uniform_buffer_range(unsigned int binding_point, shared_ptr<uniform_buffer_range> buffer_range)
{
   assert(buffer_range);

   if (renderstack::graphics::configuration::can_use.uniform_buffer_object)
   {
      s_requested.uniform_buffer_ranges[binding_point] = buffer_range;

      if (s_effective.uniform_buffer_ranges[binding_point] != s_requested.uniform_buffer_ranges[binding_point])
      {
         if (s_requested.uniform_buffer_ranges[binding_point])
            s_requested.uniform_buffer_ranges[binding_point]->bind(binding_point);

         s_effective.uniform_buffer_ranges[binding_point] = s_requested.uniform_buffer_ranges[binding_point];
      }
   }
   else
      throw runtime_error("uniform buffers are not supported");
}
void renderer::set_vertex_stream(shared_ptr<vertex_stream> vertex_stream)
{
   assert(vertex_stream);

   s_requested.vertex_stream = vertex_stream;

   if (s_effective.vertex_stream != s_requested.vertex_stream)
   {
      if (s_requested.vertex_stream)
         s_requested.vertex_stream->setup_attribute_pointers();

      s_effective.vertex_stream = s_requested.vertex_stream;
   }
}


} }
