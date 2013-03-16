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
:  m_active_texture_unit(~0u)
{
}
void renderer::push()
{
   m_request_stack.push(m_requested);
}
void renderer::pop()
{
   m_requested = m_request_stack.top();
   m_request_stack.pop();
}
void renderer::lock_material(bool value)
{
   m_lock_material = value; 
}
void renderer::update_render_states()
{
   if (m_effective.render_states != m_requested.render_states)
   {
      m_requested.render_states->use();
      m_effective.render_states = m_requested.render_states;
   }
}
void renderer::trash()
{
   slog_trace("renderer::trash()");

   m_active_texture_unit = ~0u;
   m_effective.program.reset();
   m_effective.vertex_stream.reset();
   m_effective.vertex_buffer.reset();
   m_effective.index_buffer.reset();
   for (int i = 0; i < TEXTURE_UNIT_COUNT; ++i)
   {
      m_effective.textures[i] = 0;
      m_effective.texture_target[i] = static_cast<gl::texture_target::value>(0);
   }
   for (int i = 0; i < UNIFORM_BINDING_POINT_COUNT; ++i)
   {
      m_effective.uniform_buffer_ranges[i].reset();
   }
}
void renderer::set_program(shared_ptr<class program> program)
{
   assert(program);

   m_requested.program = program;
   if (m_effective.program != m_requested.program)
   {
      if (m_requested.program)
         m_requested.program->use();

      m_effective.program = m_requested.program;
   }
}
void renderer::set_texture(unsigned int unit, gl::texture_target::value target, unsigned int texture)
{
   m_requested.textures      [unit] = texture;
   m_requested.texture_target[unit] = target;

   if (
      (m_effective.textures      [unit] != m_requested.textures      [unit]) ||
      (m_effective.texture_target[unit] != m_requested.texture_target[unit])
   )
   {
      if (m_active_texture_unit != unit)
      {
         gl::active_texture(gl::texture_unit::texture0 + unit);
         m_active_texture_unit = unit;
      }
      gl::bind_texture(m_requested.texture_target[unit], m_requested.textures[unit]);
      m_effective.textures      [unit] = m_requested.textures[unit];
      m_effective.texture_target[unit] = m_requested.texture_target[unit];
   }
}
void renderer::set_uniform_buffer_range(unsigned int binding_point, shared_ptr<uniform_buffer_range> buffer_range)
{
   assert(buffer_range);

   if (renderstack::graphics::configuration::can_use.uniform_buffer_object)
   {
      m_requested.uniform_buffer_ranges[binding_point] = buffer_range;

      if (m_effective.uniform_buffer_ranges[binding_point] != m_requested.uniform_buffer_ranges[binding_point])
      {
         if (m_requested.uniform_buffer_ranges[binding_point])
            m_requested.uniform_buffer_ranges[binding_point]->bind(binding_point);

         m_effective.uniform_buffer_ranges[binding_point] = m_requested.uniform_buffer_ranges[binding_point];
      }
   }
   else
      throw runtime_error("uniform buffers are not supported");
}
void renderer::set_vertex_stream(
   shared_ptr<vertex_stream>        vertex_stream,
   shared_ptr<class vertex_buffer>  vertex_buffer,
   shared_ptr<class index_buffer>   index_buffer
)
{
   assert(vertex_stream);
   assert(vertex_buffer);
   assert(index_buffer);   // TODO draw arrays won't need index buffer

   m_requested.vertex_stream  = vertex_stream;
   m_requested.vertex_buffer  = vertex_buffer;
   m_requested.index_buffer   = index_buffer;

   bool vertex_stream_change  = (m_effective.vertex_stream != m_requested.vertex_stream);
   bool vbo_change            = (m_effective.vertex_buffer != m_requested.vertex_buffer);
   bool ibo_change            = (m_effective.index_buffer != m_requested.index_buffer);

   if (vbo_change)
   {
      assert(vertex_stream_change); // TODO
      vertex_stream_change = true;
   }

   // Index buffer is context state and does not interact with VAO
   if (ibo_change)
   {
      if (m_requested.index_buffer)
         m_requested.index_buffer->bind();

      m_effective.index_buffer = m_requested.index_buffer;
   }

   if (vertex_stream_change)
   {
      if (m_requested.vertex_stream)
      {
         bool vao_path = m_requested.vertex_stream->use();

         if (!vao_path)
         {
            if (vbo_change)
            {
               if (m_requested.vertex_buffer)
                  m_requested.vertex_buffer->bind();

               vertex_stream->setup_attribute_pointers(0);

               m_effective.vertex_buffer = m_requested.vertex_buffer;
            }
         }
      }

      m_effective.vertex_stream = m_requested.vertex_stream;
   }
}

} }
