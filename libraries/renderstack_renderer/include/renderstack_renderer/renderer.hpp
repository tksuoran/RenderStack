#ifndef renderstack_renderer__renderer_hpp
#define renderstack_renderer__renderer_hpp

#include "renderstack_toolkit/platform.hpp"
#include "renderstack_renderer/blend_state.hpp"
#include "renderstack_renderer/depth_state.hpp"
#include "renderstack_renderer/face_cull_state.hpp"
#include "renderstack_renderer/color_mask_state.hpp"
#include "renderstack_renderer/stencil_state.hpp"
#include "renderstack_graphics/configuration.hpp"
#include "renderstack_graphics/program.hpp"
#include "renderstack_graphics/index_buffer.hpp"
#include "renderstack_graphics/uniform_buffer.hpp"
#include "renderstack_graphics/uniform_buffer_range.hpp"
#include "renderstack_graphics/vertex_buffer.hpp"
#include "renderstack_graphics/vertex_stream.hpp"
#include "renderstack_graphics/vertex_stream_mappings.hpp"
#include "renderstack_mesh/normal_style.hpp"
#include <memory>
#include <stack>

namespace renderstack { namespace mesh { 

   class   mesh;
   class   geometrymesh;
   struct  index_range;

} }

namespace renderstack { namespace renderer {

using namespace renderstack::graphics;

#define TEXTURE_UNIT_COUNT          32 // TODO Get rid of this kind of defines?
#define UNIFORM_BINDING_POINT_COUNT 16 // TODO Get rid of this kind of defines?

class render_states
{
private:
   blend_state       *m_blend_state;
   face_cull_state   *m_face_cull_state;
   depth_state       *m_depth_state;
   color_mask_state  *m_color_mask_state;
   stencil_state     *m_stencil_state;

public:
   static bool lock_blend_state;
   static bool lock_face_cull_state;
   static bool lock_depth_state;
   static bool lock_color_mask_state;
   static bool lock_stencil_state;

public:
   void use();
};

struct state
{
   std::shared_ptr<class render_states>                           render_states;
   std::shared_ptr<renderstack::graphics::program>                program;
   std::shared_ptr<renderstack::graphics::vertex_buffer>          vertex_buffer;
   std::shared_ptr<renderstack::graphics::index_buffer>           index_buffer;
   std::shared_ptr<renderstack::graphics::uniform_buffer_range>   uniform_buffer_ranges[UNIFORM_BINDING_POINT_COUNT];
   std::shared_ptr<renderstack::graphics::vertex_stream>          vertex_stream;
   unsigned int                                                   textures       [TEXTURE_UNIT_COUNT];
   gl::texture_target::value                                      texture_target [TEXTURE_UNIT_COUNT];

   state();
   state(state const &other);
};

class renderer
{
private:
   std::stack<state>                                              s_request_stack;
   state                                                          s_requested;
   state                                                          s_effective;
   bool                                                           s_lock_material;
   unsigned int                                                   s_active_texture_unit;
   std::shared_ptr<renderstack::graphics::vertex_stream_mappings> s_mappings;

public:
   std::shared_ptr<renderstack::graphics::vertex_stream_mappings> mappings();

public:
   renderer();

   void push                     ();
   void pop                      ();
   void lock_material            (bool value);
   void update_render_states     ();
   void trash                    ();
   void set_program              (std::shared_ptr<class program> program);
   void set_vertex_buffer        (std::shared_ptr<class vertex_buffer> vertex_buffer);
   void set_index_buffer         (std::shared_ptr<class index_buffer> index_buffer);
   void set_texture              (unsigned int unit, gl::texture_target::value target, unsigned int texture);
   void set_uniform_buffer_range (unsigned int binding_point, std::shared_ptr<uniform_buffer_range> buffer_range);
   void set_vertex_stream        (std::shared_ptr<vertex_stream> vertex_stream);
   void draw_elements_base_vertex(
      gl::begin_mode::value           mode, 
      size_t                          count, 
      gl::draw_elements_type::value   type, 
      size_t                          indices, 
      int                             basevertex
   );
};

} }

#endif
