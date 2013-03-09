#ifndef renderstack_renderer__face_cull_state_hpp
#define renderstack_renderer__face_cull_state_hpp

#include "renderstack_toolkit/platform.hpp"
#include "renderstack_toolkit/gl.hpp"
#include "renderstack_toolkit/strong_gl_enums.hpp"
#include "renderstack_renderer/render_state.hpp"
#include <glm/glm.hpp>

namespace renderstack { namespace renderer {

class face_cull_state : public render_state
{
private:
    bool                            m_enabled;
    gl::cull_face_mode::value       m_cull_face_mode;
    gl::front_face_direction::value m_front_face_direction;

public:
    bool                            enabled() const;
    gl::cull_face_mode::value       cull_face_mode() const;
    gl::front_face_direction::value front_face_direction() const;
    void                            set_enabled(bool value);
    void                            set_cull_face_mode(gl::cull_face_mode::value value);
    void                            set_front_face_direction(gl::front_face_direction::value value);

private:
    static face_cull_state s_default;
    static face_cull_state s_disabled;
    static face_cull_state *s_last;
    static face_cull_state s_state_cache;

public:
    static face_cull_state const &default_();
    static face_cull_state const &disabled();

    face_cull_state();
    face_cull_state(bool enabled);

    static void reset_state();

    void reset();
    void execute();
};

} }

#endif
