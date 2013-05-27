#ifndef renderstack_graphics__renderer_hpp
#define renderstack_graphics__renderer_hpp

#include "renderstack_toolkit/platform.hpp"
#include "renderstack_graphics/blend_state.hpp"
#include "renderstack_graphics/buffer.hpp"
#include "renderstack_graphics/color_mask_state.hpp"
#include "renderstack_graphics/configuration.hpp"
#include "renderstack_graphics/depth_state.hpp"
#include "renderstack_graphics/face_cull_state.hpp"
#include "renderstack_graphics/program.hpp"
#include "renderstack_graphics/sampler.hpp"
#include "renderstack_graphics/stencil_state.hpp"
#include "renderstack_graphics/texture.hpp"
#include "renderstack_graphics/uniform_buffer_range.hpp"
#include "renderstack_graphics/vertex_stream.hpp"
#include "renderstack_graphics/vertex_stream_mappings.hpp"
#include <memory>
#include <stack>

namespace renderstack { namespace graphics {


#define RS_TEXTURE_UNIT_COUNT          32 // TODO Get rid of this kind of defines?
#define RS_UNIFORM_BINDING_POINT_COUNT 16 // TODO Get rid of this kind of defines?


class render_states
{
public:
   blend_state       blend;
   face_cull_state   face_cull;
   depth_state       depth;
   color_mask_state  color_mask;
   stencil_state     stencil;
   // TODO: Scissor
};


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


class renderer
{
public:
   renderer();

   std::shared_ptr<class render_states>   request();
   void                                   push                    ();
   void                                   pop                     ();
   void                                   trash                   ();
   std::shared_ptr<class program>         set_program             (std::shared_ptr<class program> program);

   // Set specified texture to specified texture unit.
   // Returns what was previously active texture unit, and what texture was bound to the specified unit
   std::shared_ptr<class texture>         set_texture             (unsigned int unit, std::shared_ptr<class texture> texture, unsigned int *old_unit = nullptr);

   // Switch active texture unit without touching bound texture
   unsigned int                           set_texture_unit        (unsigned int unit);

   // Restores old texture to effective texture unit and then restores effective texture unit to old_unit
   void                                   restore_texture         (texture_target::value target, std::shared_ptr<class texture> old_texture, unsigned int old_unit);

#if defined(RENDERSTACK_GL_API_OPENGL) || defined(RENDERSTACK_GL_API_OPENGL_ES_3)
   std::shared_ptr<uniform_buffer_range>  set_uniform_buffer_range(unsigned int binding_point, std::shared_ptr<uniform_buffer_range> buffer_range);
#endif

   std::shared_ptr<class buffer>          set_buffer              (buffer_target::value target, std::shared_ptr<class buffer> buffer);
   std::shared_ptr<class vertex_array>    set_vertex_array        (std::shared_ptr<class vertex_array> vertex_array);
   void                                   reset_vertex_array      ();

   void setup_attribute_pointers(
      std::shared_ptr<class vertex_stream> vertex_stream,
      GLint basevertex);

   void use_vertex_stream(std::shared_ptr<class vertex_stream> vertex_stream);

   void draw_elements_base_vertex(
      std::shared_ptr<class vertex_stream> vertex_stream,
      GLenum begin_mode, GLsizei count, GLenum index_type, const GLvoid *indices,
      GLint base_vertex);

   std::shared_ptr<class vertex_array> vertex_array() const
   {
      return m_effective.vertex_array_binding;
   }

friend class buffer;
friend class uniform_buffer_range;
friend class texture;
private:
   bool map_buffer(buffer_target::value target, std::shared_ptr<class buffer> buffer);
   bool buffer_is_bound(buffer_target::value target, std::shared_ptr<class buffer> buffer);
   bool buffer_is_mapped(buffer_target::value target, std::shared_ptr<class buffer> buffer);
   bool unmap_buffer(buffer_target::value target, std::shared_ptr<class buffer> buffer);
   bool texture_is_bound(unsigned int unit, texture_target::value target, std::shared_ptr<class texture> texture);
   unsigned int effective_texture_unit() const;

private:
   //void setup_attribute_pointers_old(std::shared_ptr<class vertex_stream> vertex_stream, GLint basevertex);
   void setup_attribute_pointers_new(std::shared_ptr<class vertex_stream> vertex_stream, GLint basevertex);

private:
   std::stack<state>          m_request_stack;
   state                      m_requested;
   state                      m_effective;
   std::shared_ptr<buffer>    m_mapped_buffer[buffer_target::all_buffer_target_count];

   // When vertex array objects are used, we use this to track which attributes are enabled,
   // and set vertex attribute pointers through
   std::shared_ptr<renderstack::graphics::vertex_array> m_default_vertex_array;
};

} }

#endif

