#include "renderstack_graphics/state_trackers.hpp"
#include "renderstack_graphics/buffer.hpp"
#include "renderstack_graphics/sampler.hpp"
#include "renderstack_graphics/vertex_array.hpp"
#include "renderstack_toolkit/log.hpp"
#include "renderstack_toolkit/platform.hpp"

#define LOG_CATEGORY &log_renderer

namespace renderstack
{
namespace graphics
{

using namespace std;


Texture_unit_state::Texture_unit_state(const Texture_unit_state &other)
    : sampler_binding(other.sampler_binding)
{
    for (size_t i = 0; i < texture_binding.size(); ++i)
    {
        texture_binding[i] = other.texture_binding[i];
    }
}

State::State(const State &other)
    : vertex_array_binding(other.vertex_array_binding)
    , current_program(other.current_program)
    , transform_feedback_binding(other.transform_feedback_binding)
    , active_texture(other.active_texture)
    , draw_framebuffer_binding(other.draw_framebuffer_binding)
    , read_framebuffer_binding(other.read_framebuffer_binding)
    , renderbuffer_binding(other.renderbuffer_binding)
    , current_query(other.current_query)
{
    for (size_t i = 0; i < texture_unit.size(); ++i)
    {
        texture_unit[i] = other.texture_unit[i];
    }

    for (size_t i = 0; i < buffer_binding.size(); ++i)
    {
        buffer_binding[i] = other.buffer_binding[i];
    }

    for (size_t i = 0; i < uniform_buffer_binding_indexed.size(); ++i)
    {
        uniform_buffer_binding_indexed[i] = other.uniform_buffer_binding_indexed[i];
    }
}

void Texture_unit_state::trash(unsigned int unit)
{
    for (size_t i = 0; i < texture_binding.size(); ++i)
    {
        Texture::Target target = static_cast<Texture::Target>(i);
        gl::bind_texture(Texture::gl_texture_target(target), 0);
        texture_binding[i] = nullptr;
    }

    gl::bind_sampler(unit, 0);
    sampler_binding = nullptr;
}

void State_trackers::reset()
{
    color_mask.reset();
    blend.reset();
    depth.reset();
    face_cull.reset();
    stencil.reset();
}

void State_trackers::execute(Render_states const *states)
{
    color_mask.execute(&states->color_mask);
    blend.execute(&states->blend);
    depth.execute(&states->depth);
    face_cull.execute(&states->face_cull);
    stencil.execute(&states->stencil);
}

} // namespace graphics
} // namespace renderstack
