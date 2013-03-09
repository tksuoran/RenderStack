#ifndef renderstack_renderer__stencil_state_hpp
#define renderstack_renderer__stencil_state_hpp

#include "renderstack_toolkit/platform.hpp"
#include "renderstack_toolkit/gl.hpp"
#include "renderstack_toolkit/strong_gl_enums.hpp"
#include "renderstack_renderer/render_state.hpp"
#include <glm/glm.hpp>

namespace renderstack { namespace renderer {

class stencil_state;

class stencil_state_component
{
private:
   gl::stencil_op_enum::value       m_stencil_fail_op;
   gl::stencil_op_enum::value       m_z_fail_op;
   gl::stencil_op_enum::value       m_z_pass_op;
   gl::stencil_function_enum::value m_function;
   int                              m_reference;
   GLuint                           m_test_mask;
   GLuint                           m_write_mask;

public:
   gl::stencil_op_enum::value       stencil_fail_op      () const;
   gl::stencil_op_enum::value       z_fail_op            () const;
   gl::stencil_op_enum::value       z_pass_op            () const;
   gl::stencil_function_enum::value function             () const;
   int                              reference            () const;
   GLuint                           test_mask            () const;
   GLuint                           write_mask           () const;
   void                             set_stencil_fail_op  (gl::stencil_op_enum::value         value);
   void                             set_z_fail_op        (gl::stencil_op_enum::value         value);
   void                             set_z_pass_op        (gl::stencil_op_enum::value         value);
   void                             set_function         (gl::stencil_function_enum::value   value);
   void                             set_reference        (int                                value);
   void                             set_test_mask        (GLuint                             value);
   void                             set_write_mask       (GLuint                             value);

   stencil_state_component();
   
   void apply(gl::stencil_face::value face, stencil_state_component &cache);
   void apply_shared(stencil_state &cache);
};

class stencil_state : public render_state
{
private:
   bool                    m_enabled;
   bool                    m_separate;
   stencil_state_component m_front;
   stencil_state_component m_back;

public:
   stencil_state();
   
   bool                          enabled     () const;
   bool                          separate    () const;
   stencil_state_component const &front      () const;
   stencil_state_component       &front      ();
   stencil_state_component const &back       () const;
   stencil_state_component       &back       ();
   void                          set_enabled (bool value);
   void                          set_separate(bool value);
   void                          set_front   (stencil_state_component const &value);
   void                          set_back    (stencil_state_component const &value);

private:
   static stencil_state *s_last;
   static stencil_state s_default;
   static stencil_state s_state_cache;

public:
   static stencil_state const &default_();   
   static void reset_state();
   
   void reset();
   void execute();
};

} } 

#endif
