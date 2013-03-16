#ifndef renderstack_renderer__depth_state_hpp
#define renderstack_renderer__depth_state_hpp

#include "renderstack_toolkit/platform.hpp"
#include "renderstack_toolkit/gl.hpp"
#include "renderstack_toolkit/strong_gl_enums.hpp"
#include "renderstack_renderer/render_state.hpp"
#include <glm/glm.hpp>

namespace renderstack { namespace renderer {

class depth_state : public render_state
{
public:
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

public:
   static depth_state const &default_();
   static depth_state const &disabled();

   depth_state();
   depth_state(bool enabled);
   virtual ~depth_state();

public:
   static void reset_state();

public:
   void reset();
   void execute() const;

private:
   bool                       m_enabled;
   gl::depth_function::value  m_function;
   float                      m_near;
   float                      m_far;
   bool                       m_depth_mask;

private:
   static depth_state         s_default;
   static depth_state         s_disabled;
   static depth_state const   *s_last;
   static depth_state         s_state_cache;
};

} }

#endif
