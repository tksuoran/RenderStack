#ifndef deferred_renderer_hpp_renderers
#define deferred_renderer_hpp_renderers

#include "renderstack_toolkit/platform.hpp"
#include "renderstack_toolkit/service.hpp"
#include "main/programs.hpp"
#include "renderers/quad_renderer.hpp"
#include "scene/model.hpp"
#include <glm/glm.hpp>
#include <vector>
#include <memory>

namespace renderstack
{
   namespace graphics
   {
      class uniform_buffer;
      class uniform_buffer_range;
   }
   namespace mesh
   {
      class geometry_mesh;
   }
}

class deferred_renderer : public renderstack::toolkit::service
{
public:
   deferred_renderer();
   /*virtual*/ ~deferred_renderer();

   void connect(
      std::shared_ptr<renderstack::graphics::renderer>   renderer,
      std::shared_ptr<programs>                          programs,
      std::shared_ptr<quad_renderer>                     quad_renderer
   );
   /*virtual/*/ void initialize_service();

   void geometry_pass(
      std::shared_ptr<class group> group,
      glm::mat4 const &clip_from_world,
      glm::mat4 const &view_from_world
   );
   void light_pass(glm::mat4 const &world_from_view);
   void show_rt();

   void resize(int width, int height);

private:
   void fbo_clear();
   void bind_fbo();
   void bind_default_framebuffer();

private:
   std::shared_ptr<renderstack::graphics::renderer>   m_renderer;
   std::shared_ptr<programs>                          m_programs;
   std::shared_ptr<quad_renderer>                     m_quad_renderer;

   renderstack::graphics::render_states               m_mesh_render_states;
   renderstack::graphics::render_states               m_light_render_states;
   renderstack::graphics::render_states               m_show_rt_render_states;

   // framebuffer
   unsigned int                                    m_fbo;
   std::shared_ptr<renderstack::graphics::texture> m_rt[4];
   std::shared_ptr<renderstack::graphics::texture> m_depth;
};


#endif
