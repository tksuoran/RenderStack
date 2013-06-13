#ifndef stencil_state_hpp_renderstack_graphics
#define stencil_state_hpp_renderstack_graphics

#include "renderstack_toolkit/platform.hpp"
#include "renderstack_toolkit/gl.hpp"
#include "renderstack_toolkit/strong_gl_enums.hpp"
#include <glm/glm.hpp>

namespace renderstack { namespace graphics {

class stencil_state;

class stencil_state_component
{
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
   
   void apply(gl::stencil_face::value face, stencil_state_component &cache) const;
   void apply_shared(stencil_state &cache) const;

private:
   gl::stencil_op_enum::value       m_stencil_fail_op;
   gl::stencil_op_enum::value       m_z_fail_op;
   gl::stencil_op_enum::value       m_z_pass_op;
   gl::stencil_function_enum::value m_function;
   int                              m_reference;
   GLuint                           m_test_mask;
   GLuint                           m_write_mask;
};

class stencil_state
{
public:
   stencil_state();

   void reset();
   
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
   bool                    m_enabled;
   bool                    m_separate;
   stencil_state_component m_front;
   stencil_state_component m_back;
};

class stencil_state_tracker
{
public:
   stencil_state_tracker();

   void reset();
   void execute(stencil_state const * state);

private:
   void execute_component(
      gl::stencil_face::value face,
      stencil_state_component const &state,
      stencil_state_component &cache
   );
   void execute_shared(stencil_state_component const &state, stencil_state &cache);

private:
   stencil_state const  *m_last;
   stencil_state        m_cache;
};

} } 

#endif
