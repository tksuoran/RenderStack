#ifndef renderstack_graphics__depth_state_hpp
#define renderstack_graphics__depth_state_hpp

#include "renderstack_toolkit/platform.hpp"
#include "renderstack_toolkit/gl.hpp"
#include "renderstack_toolkit/strong_gl_enums.hpp"
#include <glm/glm.hpp>

namespace renderstack { namespace graphics {

class depth_state
{
public:
   depth_state();
   depth_state(bool enabled);

   void reset();

   bool                       enabled     () const;
   gl::depth_function::value  function    () const;
   float                      near_       () const;
   float                      far_        () const;
   bool                       depth_mask  () const;

   void set_enabled     (bool value);
   void set_function    (gl::depth_function::value value);
   void set_near        (float value);
   void set_far         (float value);
   void set_depth_mask  (bool value);

private:
   bool                       m_enabled;
   gl::depth_function::value  m_function;
   float                      m_near;
   float                      m_far;
   bool                       m_depth_mask;
};

class depth_state_tracker
{
public:
   depth_state_tracker();

   void reset();
   void execute(depth_state const *state);

private:
   depth_state const *m_last;
   depth_state       m_cache;
};

} }

#endif
