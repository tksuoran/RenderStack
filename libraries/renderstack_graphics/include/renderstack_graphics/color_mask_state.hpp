#ifndef renderstack_graphics__color_mask_state_hpp
#define renderstack_graphics__color_mask_state_hpp

#include "renderstack_toolkit/platform.hpp"
#include "renderstack_toolkit/gl.hpp"
#include "renderstack_toolkit/strong_gl_enums.hpp"
#include <glm/glm.hpp>

namespace renderstack { namespace graphics {

class color_mask_state
{
public:
   bool dirty     () const;
   void reset     ();

   bool red       () const;
   bool green     () const;
   bool blue      () const;
   bool alpha     () const;
   void set_red   (bool value);
   void set_green (bool value);
   void set_blue  (bool value);
   void set_alpha (bool value);

private:
   bool m_red;
   bool m_green;
   bool m_blue;
   bool m_alpha;
};


struct color_mask_state_tracker
{
public:
   color_mask_state_tracker();

   void reset();
   void execute(color_mask_state const *state);

private:
   color_mask_state const  *m_last;
   color_mask_state        m_cache;
};

} }

#endif
