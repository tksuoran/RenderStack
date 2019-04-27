#ifndef state_trackers_hpp_renderstack_graphics
#define state_trackers_hpp_renderstack_graphics

#include "renderstack_graphics/blend_state.hpp"
#include "renderstack_graphics/buffer.hpp"
#include "renderstack_graphics/color_mask_state.hpp"
#include "renderstack_graphics/configuration.hpp"
#include "renderstack_graphics/depth_state.hpp"
#include "renderstack_graphics/face_cull_state.hpp"
#include "renderstack_graphics/stencil_state.hpp"
#include "renderstack_graphics/vertex_array.hpp"
#include "renderstack_toolkit/platform.hpp"

#include "renderstack_graphics/program.hpp"
#include "renderstack_graphics/texture.hpp"
#include "renderstack_graphics/uniform_buffer_range.hpp"

#include <memory>
#include <stack>

namespace renderstack
{
namespace graphics
{

class Sampler;

#define RS_TEXTURE_UNIT_COUNT 32          // TODO Get rid of this kind of defines?
#define RS_UNIFORM_BINDING_POINT_COUNT 16 // TODO Get rid of this kind of defines?

class Texture_unit_state
{
public:
    Texture_unit_state()
    {
        std::fill(begin(texture_binding), end(texture_binding), nullptr);
    }

    Texture_unit_state(const Texture_unit_state &other);

    void trash(unsigned int unit);

    // table 6.8: Textures (selector, state per texture unit)
    std::array<Texture *, Texture::target_count> texture_binding;

    Sampler *sampler_binding{nullptr};
};

class State
{
public:
    State()
    {
        std::fill(begin(buffer_binding), end(buffer_binding), nullptr);
        std::fill(begin(uniform_buffer_binding_indexed), end(uniform_buffer_binding_indexed), nullptr);
    }

    ~State() = default;

    State(const State &other);

    // Buffer binding points in context, non indexed
    std::array<Buffer *, Buffer::all_buffer_target_count> buffer_binding;

    // Buffer binding points in context, indexed
#if defined(RENDERSTACK_GL_API_OPENGL) || defined(RENDERSTACK_GL_API_OPENGL_ES_3)
    std::array<Uniform_buffer_range *, RS_UNIFORM_BINDING_POINT_COUNT> uniform_buffer_binding_indexed;
#endif

    Vertex_array *vertex_array_binding{nullptr}; // 6.3   (bound VAO)
    Program      *current_program{nullptr};      // 6.18

    std::array<Texture_unit_state, RS_TEXTURE_UNIT_COUNT> texture_unit;

    // Not yet objectified

    unsigned int transform_feedback_binding{0U}; // 6.5
    unsigned int active_texture{0U};             // 6.8
    unsigned int draw_framebuffer_binding{0U};   // 6.12
    unsigned int read_framebuffer_binding{0U};   // 6.12
    unsigned int renderbuffer_binding{0U};       // 6.12
    unsigned int current_query{0U};              // 6.36
};

class Transform_feedback_state
{
public:
    // Table 6.24: Transform feedback state
    unsigned int transform_feedback_buffer_binding;
    unsigned int transform_feedback_buffer_binding_indexed[4];
};

struct Render_states
{
    Color_mask_state color_mask;
    Blend_state      blend;
    Depth_state      depth;
    Face_cull_state  face_cull;
    Stencil_state    stencil;
};

class State_trackers
{
public:
    ~State_trackers() = default;

    void reset();

    void execute(Render_states const *states);

    Color_mask_state_tracker color_mask;
    Blend_state_tracker      blend;
    Depth_state_tracker      depth;
    Face_cull_state_tracker  face_cull;
    Stencil_state_tracker    stencil;
};

} // namespace graphics
} // namespace renderstack

#endif
