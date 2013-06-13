#ifndef face_cull_state_hpp_renderstack_graphics
#define face_cull_state_hpp_renderstack_graphics

#include "renderstack_toolkit/platform.hpp"
#include "renderstack_toolkit/gl.hpp"
#include "renderstack_toolkit/strong_gl_enums.hpp"
#include <glm/glm.hpp>

namespace renderstack { namespace graphics {

class face_cull_state
{
public:
    face_cull_state();
    face_cull_state(bool enabled);

    void reset();

    bool                            enabled() const;
    gl::cull_face_mode::value       cull_face_mode() const;
    gl::front_face_direction::value front_face_direction() const;
    void                            set_enabled(bool value);
    void                            set_cull_face_mode(gl::cull_face_mode::value value);
    void                            set_front_face_direction(gl::front_face_direction::value value);

private:
    bool                            m_enabled;
    gl::cull_face_mode::value       m_cull_face_mode;
    gl::front_face_direction::value m_front_face_direction;
};

class face_cull_state_tracker
{
public:
   face_cull_state_tracker();

   void reset();
   void execute(face_cull_state const *state);

private:
    face_cull_state const  *m_last;
    face_cull_state        m_cache;
};

} }

#endif
