#ifndef renderstack_graphics__state_trackers_hpp
#define renderstack_graphics__state_trackers_hpp

#include "renderstack_toolkit/platform.hpp"
#include "renderstack_graphics/blend_state.hpp"
#include "renderstack_graphics/buffer.hpp"
#include "renderstack_graphics/color_mask_state.hpp"
#include "renderstack_graphics/configuration.hpp"
#include "renderstack_graphics/depth_state.hpp"
#include "renderstack_graphics/face_cull_state.hpp"
#include "renderstack_graphics/stencil_state.hpp"

#include "renderstack_graphics/texture.hpp"
#include "renderstack_graphics/uniform_buffer_range.hpp"
#include "renderstack_graphics/program.hpp"

#include <memory>
#include <stack>

namespace renderstack { namespace graphics {


#define RS_TEXTURE_UNIT_COUNT          32 // TODO Get rid of this kind of defines?
#define RS_UNIFORM_BINDING_POINT_COUNT 16 // TODO Get rid of this kind of defines?


class texture_unit_state
{
public:
   texture_unit_state();
   texture_unit_state(texture_unit_state const &other);

   void trash(unsigned int unit);

   // table 6.8: Textures (selector, state per texture unit)
   std::shared_ptr<class texture>   texture_binding[texture_target::count];
   std::shared_ptr<class sampler>   sampler_binding;
};

class state // context_state
{
public:
   state();
   state(state const &other);

   // Buffer binding points in context, non indexed
   std::shared_ptr<buffer>                buffer_binding[buffer_target::non_indexed_context_buffer_target_count];

   // Buffer binding points in context, indexed
#if defined(RENDERSTACK_GL_API_OPENGL) || defined(RENDERSTACK_GL_API_OPENGL_ES_3)
   std::shared_ptr<uniform_buffer_range>  uniform_buffer_binding_indexed[RS_UNIFORM_BINDING_POINT_COUNT]; // 6.25
#endif

   std::shared_ptr<vertex_array>          vertex_array_binding;               // 6.3   (bound VAO)
   std::shared_ptr<program>               current_program;                    // 6.18

   texture_unit_state                     texture_unit[RS_TEXTURE_UNIT_COUNT];

   // Not yet objectified

   unsigned int transform_feedback_binding;  // 6.5
   unsigned int active_texture;              // 6.8
   unsigned int draw_framebuffer_binding;    // 6.12
   unsigned int read_framebuffer_binding;    // 6.12
   unsigned int renderbuffer_binding;        // 6.12
   unsigned int current_query;               // 6.36
};

class transform_feedback_state
{
public:
   // Table 6.24: Transform feedback state
   unsigned int transform_feedback_buffer_binding;
   unsigned int transform_feedback_buffer_binding_indexed[4];
};

struct render_states
{
   color_mask_state  color_mask;
   blend_state       blend;
   depth_state       depth;
   face_cull_state   face_cull;
   stencil_state     stencil;
};

class state_trackers
{
public:
   state_trackers();
   ~state_trackers();

   void reset();
   void execute(render_states const *states);

   color_mask_state_tracker   &color_mask  () { return m_color_mask; }
   blend_state_tracker        &blend       () { return m_blend; }
   depth_state_tracker        &depth       () { return m_depth; }
   face_cull_state_tracker    &face_cull   () { return m_face_cull; }
   stencil_state_tracker      &stencil     () { return m_stencil; }

private:
   color_mask_state_tracker   m_color_mask;
   blend_state_tracker        m_blend;
   depth_state_tracker        m_depth;
   face_cull_state_tracker    m_face_cull;
   stencil_state_tracker      m_stencil;

};


} }

#endif

