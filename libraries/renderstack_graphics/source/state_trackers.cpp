#include "renderstack_toolkit/platform.hpp"
#include "renderstack_toolkit/logstream.hpp"
#include "renderstack_graphics/state_trackers.hpp"
#include "renderstack_graphics/sampler.hpp"
#include "renderstack_graphics/buffer.hpp"
#include "renderstack_graphics/vertex_array.hpp"

#define LOG_CATEGORY &log_graphics_renderer

namespace renderstack { namespace graphics {

using namespace std;


texture_unit_state::texture_unit_state()
:  sampler_binding(nullptr)
{
   for (unsigned int target = 0; target < texture_target::count; ++target)
      texture_binding[target].reset();
}

texture_unit_state::texture_unit_state(texture_unit_state const &other)
:  sampler_binding(other.sampler_binding)
{
   for (unsigned int target = 0; target < texture_target::count; ++target)
      texture_binding[target] = other.texture_binding[target];
}


state::state()
:  vertex_array_binding       (nullptr)
,  current_program            (nullptr)
,  transform_feedback_binding (0)
,  active_texture             (0)
,  draw_framebuffer_binding   (0)
,  read_framebuffer_binding   (0)
,  renderbuffer_binding       (0)
,  current_query              (0)
{
   // for (int i = 0; i < RS_TEXTURE_UNIT_COUNT; ++i)
   // texture_unit[i]

   for (int i = 0; i < buffer_target::non_indexed_context_buffer_target_count; ++i)
      buffer_binding[i].reset();

   for (int i = 0; i < RS_UNIFORM_BINDING_POINT_COUNT; ++i)
      uniform_buffer_binding_indexed[i].reset();
}
state::state(state const &other)
:  vertex_array_binding       (other.vertex_array_binding)
,  current_program            (other.current_program)
,  transform_feedback_binding (other.transform_feedback_binding)
,  active_texture             (other.active_texture)
,  draw_framebuffer_binding   (other.draw_framebuffer_binding)
,  read_framebuffer_binding   (other.read_framebuffer_binding)
,  renderbuffer_binding       (other.renderbuffer_binding)
,  current_query              (other.current_query)
{
   for (int i = 0; i < RS_TEXTURE_UNIT_COUNT; ++i)
      texture_unit[i] = other.texture_unit[i];

   for (int i = 0; i < buffer_target::non_indexed_context_buffer_target_count; ++i)
      buffer_binding[i] = other.buffer_binding[i];

   for (int i = 0; i < RS_UNIFORM_BINDING_POINT_COUNT; ++i)
      uniform_buffer_binding_indexed[i] = other.uniform_buffer_binding_indexed[i];
}


void texture_unit_state::trash(unsigned int unit)
{
   for (int i = 0; i < texture_target::count; ++i)
   {
      texture_target::value target = static_cast<texture_target::value>(i);
      gl::bind_texture(texture_target::gl_texture_target(target), 0);
      texture_binding[i].reset();
   }

   gl::bind_sampler(unit, 0);
   sampler_binding.reset();
}

state_trackers::state_trackers()
{
}
state_trackers::~state_trackers()
{
}

void state_trackers::reset()
{
   color_mask().reset();
   blend     ().reset();
   depth     ().reset();
   face_cull ().reset();
   stencil   ().reset();
}
void state_trackers::execute(render_states const *states)
{
   color_mask().execute(&states->color_mask);
   blend     ().execute(&states->blend     );
   depth     ().execute(&states->depth     );
   face_cull ().execute(&states->face_cull );
   stencil   ().execute(&states->stencil   );
}

} }

