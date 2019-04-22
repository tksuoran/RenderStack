#ifndef debug_hpp_renderstack_graphics
#define debug_hpp_renderstack_graphics

#include "renderstack_toolkit/gl.hpp"
#include "renderstack_toolkit/platform.hpp"

namespace renderstack
{
namespace graphics
{

extern void debug_program();
extern void debug_buffer(GLenum target, GLint buffer, GLenum index_type);
extern void debug_vao() extern void debug_gl_state();

} // namespace graphics
} // namespace renderstack

#endif
