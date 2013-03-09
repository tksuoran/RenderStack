#ifndef programs_hpp
#define programs_hpp

#include "renderstack_toolkit/platform.hpp"
#include "renderstack_graphics/program.hpp"
#include <memory>

namespace renderstack
{
   namespace graphics
   {
      class shader_monitor;
      class sampler;
      class samplers;
      class program;
      class uniform_block;
      class uniform_buffer;
   }
}

struct uniform_offsets
{
   size_t  model_to_clip;  /* mat4 */
   size_t  color;          /* vec4 */
   size_t  line_width;     /* vec2 */
   size_t  viewport;       /* vec4 */
};

class programs
{
private:
   bool                                                     m_poll_shaders;
   size_t                                                   m_poll_ticks;
   std::shared_ptr<renderstack::graphics::shader_monitor>   m_monitor;
   int                                                      m_glsl_version;

private:
   void map(std::shared_ptr<renderstack::graphics::program> program);

public:
   std::shared_ptr<renderstack::graphics::uniform_block>   block;
   std::shared_ptr<renderstack::graphics::samplers>        samplers;
   struct uniform_offsets                                  uniform_offsets;
   struct uniform_offsets                                  uniform_keys;
   std::shared_ptr<renderstack::graphics::program>         font;
   std::shared_ptr<renderstack::graphics::program>         basic;
   std::shared_ptr<renderstack::graphics::program>         textured_gui;

public:
   void prepare_gl_resources();
   void update_fixed_step();
   int glsl_version() const;
};

#endif
