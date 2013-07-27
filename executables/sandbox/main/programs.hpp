#ifndef programs_hpp
#define programs_hpp

#include "renderstack_toolkit/platform.hpp"
#include "renderstack_toolkit/service.hpp"
#include "renderstack_graphics/configuration.hpp"
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
      class vertex_stream_mappings;
   }
}

struct uniform_offsets
{
   size_t clip_from_model;       /* mat4 */
   size_t world_from_model;      /* mat4 */
   size_t world_from_view;       /* mat4 */
   size_t view_from_model;       /* mat4 */
   size_t color;                 /* vec4 */
   size_t line_width;            /* vec2 */
   size_t viewport;              /* vec4 */
   size_t material_parameters;   /* vec4 */
   size_t show_rt_transform;     /* mat4 */
   size_t id_offset;             /* vec3 */
};

class programs : public renderstack::toolkit::service
{
public:
   programs();
   /*virtual*/ ~programs();
   void connect(std::shared_ptr<renderstack::graphics::renderer> renderer);
   /*virtual*/ void initialize_service();

private:
   void map(std::shared_ptr<renderstack::graphics::program> program);

   std::shared_ptr<renderstack::graphics::program> make_program(std::string const &name);

public:
   struct uniform_offsets                                         uniform_offsets;
   struct uniform_offsets                                         uniform_keys;

   std::shared_ptr<renderstack::graphics::buffer>                 uniform_buffer;
   std::shared_ptr<renderstack::graphics::uniform_block>          block;
   std::shared_ptr<renderstack::graphics::samplers>               samplers;
   std::shared_ptr<renderstack::graphics::vertex_stream_mappings> mappings;
   std::shared_ptr<renderstack::graphics::program>                font;
   std::shared_ptr<renderstack::graphics::program>                basic;
   std::shared_ptr<renderstack::graphics::program>                debug_line;
   std::shared_ptr<renderstack::graphics::program>                textured;
   std::shared_ptr<renderstack::graphics::program>                gbuffer;
   std::shared_ptr<renderstack::graphics::program>                light;
   std::shared_ptr<renderstack::graphics::program>                show_rt;
   std::shared_ptr<renderstack::graphics::program>                show_rt_spherical;
   std::shared_ptr<renderstack::graphics::program>                id;

public:
   void update_fixed_step();
   int glsl_version() const;
   bool use_uniform_buffers() const;

private:
   std::shared_ptr<renderstack::graphics::renderer>         m_renderer;

   bool                                                     m_poll_shaders;
   size_t                                                   m_poll_ticks;
   std::shared_ptr<renderstack::graphics::shader_monitor>   m_monitor;
   int                                                      m_glsl_version;
   std::string                                              m_shader_path;
};

#endif
