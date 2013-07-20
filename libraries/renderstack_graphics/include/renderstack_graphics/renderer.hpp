#ifndef renderer_hpp_renderstack_graphics
#define renderer_hpp_renderstack_graphics

#include "renderstack_toolkit/platform.hpp"
#include "renderstack_toolkit/service.hpp"
#include "renderstack_graphics/buffer.hpp"
#include "renderstack_graphics/color_mask_state.hpp"
#include "renderstack_graphics/configuration.hpp"
#include "renderstack_graphics/program.hpp"
#include "renderstack_graphics/sampler.hpp"
#include "renderstack_graphics/state_trackers.hpp"
#include "renderstack_graphics/texture.hpp"
#include "renderstack_graphics/uniform_buffer_range.hpp"
#include "renderstack_graphics/vertex_stream.hpp"
#include "renderstack_graphics/vertex_stream_mappings.hpp"
#include <memory>
#include <stack>

namespace renderstack { namespace graphics {


class renderer : public renderstack::toolkit::service
{
public:
   renderer();
   /*virtual*/ ~renderer();

   /*virtual*/ void initialize_service();

   state_trackers &track() { return m_track; }

   //std::shared_ptr<class render_states>   request();
   void                                   push                    ();
   void                                   pop                     ();
   void                                   trash                   ();
   std::shared_ptr<class program>         set_program             (std::shared_ptr<class program> program);

   // Set specified texture to specified texture unit.
   // Returns what was previously active texture unit, and what texture was bound to the specified unit
   std::shared_ptr<class texture>         set_texture             (unsigned int unit, std::shared_ptr<class texture> texture, unsigned int *old_unit = nullptr);
   std::shared_ptr<class texture>         reset_texture           (unsigned int unit, texture_target::value target, unsigned int *old_unit = nullptr);

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
   bool cache_enabled() const { return m_cache_enabled; }
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
   bool                    m_cache_enabled;

   state_trackers          m_track;
   std::stack<state>       m_request_stack;
   state                   m_requested;
   state                   m_effective;
   std::shared_ptr<buffer> m_mapped_buffer[buffer_target::all_buffer_target_count];

   // When vertex array objects are used, we use this to track which attributes are enabled,
   // and set vertex attribute pointers through
   std::shared_ptr<renderstack::graphics::vertex_array> m_default_vertex_array;
};

} }

#endif

