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
      class uniform_buffer_range;
      class vertex_stream_mappings;
   }
}

struct model_block_i
{
   std::size_t clip_from_model;       /* mat4 */
   std::size_t world_from_model;      /* mat4 */
   std::size_t view_from_model;       /* mat4 */
   std::size_t id_offset;             /* vec3 */
};

struct camera_block_i
{
   std::size_t world_from_view;       /* mat4 */
   std::size_t world_from_clip;       /* mat4 */
   std::size_t viewport;              /* vec4 */
   std::size_t exposure;
};

struct lights_block_i
{
   std::size_t position;
   std::size_t direction;
   std::size_t radiance;
   std::size_t spot_cutoff;
};

struct material_block_i
{
   std::size_t color;               /* vec4 */
   std::size_t roughness;           /* float aka 1.0 - smoothness */
   std::size_t isotropy;            /* float aka 1.0 - anisotropy */
   // std::size_t translucency;        /* float */
   // std::size_t metalness;           /* float */
   // std::size_t specular_coverage;   /* float */
   // std::size_t wet_roughness;       /* float */
};

struct debug_block_i
{
   size_t line_width;            /* vec2 */
   size_t show_rt_transform;     /* mat4 */
};

struct ubr_pos
{
   ubr_pos()
   :  model(0)
   ,  camera(0)
   ,  material(0)
   ,  lights(0)
   ,  debug(0)
   {
   }
   int model;
   int camera;
   int material;
   int lights;
   int debug;
};

struct ubr_ptr
{
   unsigned char *model;
   unsigned char *camera;
   unsigned char *material;
   unsigned char *lights;
   unsigned char *debug;
};

/*struct draw_v1
{
   std::shared_ptr<renderstack::graphics::vertex_stream> vertex_stream;
   gl::begin_mode::value                                 begin_mode;
   GLsizei                                               count;
   gl::draw_elements_type::value                         index_type;
   GLvoid                                                *index_pointer;
   GLint                                                 base_vertex
};*/

class programs : public renderstack::toolkit::service
{
public:
   programs();
   /*virtual*/ ~programs();
   void connect(
      std::shared_ptr<renderstack::graphics::renderer>         renderer_,
      std::shared_ptr<renderstack::graphics::shader_monitor>   shader_monitor_
   );
   /*virtual*/ void initialize_service();

private:
   std::shared_ptr<renderstack::graphics::program> make_program(
      std::string const &name,
      std::vector<std::string> const &defines
   );
   std::shared_ptr<renderstack::graphics::program> make_program(
      std::string const &name
   );

public:
   model_block_i                                                     model_block_access;
   camera_block_i                                                    camera_block_access;
   material_block_i                                                  material_block_access;
   lights_block_i                                                    lights_block_access;
   debug_block_i                                                     debug_block_access;

   std::shared_ptr<renderstack::graphics::uniform_block>             model_block;
   std::shared_ptr<renderstack::graphics::uniform_block>             camera_block;
   std::shared_ptr<renderstack::graphics::uniform_block>             material_block;
   std::shared_ptr<renderstack::graphics::uniform_block>             lights_block;
   std::shared_ptr<renderstack::graphics::uniform_block>             debug_block;

   std::shared_ptr<renderstack::graphics::samplers>                  samplers;
   std::shared_ptr<renderstack::graphics::vertex_attribute_mappings> attribute_mappings;
   std::shared_ptr<renderstack::graphics::fragment_outputs>          fragment_outputs;
   std::shared_ptr<renderstack::graphics::program>                   font;
   std::shared_ptr<renderstack::graphics::program>                   basic;
   std::shared_ptr<renderstack::graphics::program>                   anisotropic;
   std::shared_ptr<renderstack::graphics::program>                   debug_line;
   std::shared_ptr<renderstack::graphics::program>                   debug_light;
   std::shared_ptr<renderstack::graphics::program>                   textured;
   std::shared_ptr<renderstack::graphics::program>                   gbuffer;
   std::shared_ptr<renderstack::graphics::program>                   light_spot;
   std::shared_ptr<renderstack::graphics::program>                   light_directional;
   std::shared_ptr<renderstack::graphics::program>                   stencil;
   std::shared_ptr<renderstack::graphics::program>                   show_rt;
   std::shared_ptr<renderstack::graphics::program>                   show_rt_spherical;
   std::shared_ptr<renderstack::graphics::program>                   id;
   std::shared_ptr<renderstack::graphics::program>                   camera;

public:
   void update_fixed_step();

private:
   std::shared_ptr<renderstack::graphics::renderer>         m_renderer;
   std::shared_ptr<renderstack::graphics::shader_monitor>   m_shader_monitor;

   bool                                                     m_poll_shaders;
   size_t                                                   m_poll_ticks;

   std::vector<std::pair<std::string, int> >                m_shader_versions;
   std::string                                              m_shader_path;
};

#endif
