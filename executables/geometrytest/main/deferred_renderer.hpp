#ifndef deferred_renderer__hpp
#define deferred_renderer__hpp

#include "renderstack_toolkit/platform.hpp"
#include "main/programs.hpp"
#include "main/quad_renderer.hpp"
#include "renderer/model.hpp"
#include <glm/glm.hpp>
#include <vector>
#include <memory>

extern log_category log_game;

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

class deferred_renderer
{
public:
   deferred_renderer(
      std::shared_ptr<renderstack::graphics::renderer>   renderer,
      std::shared_ptr<programs>                          programs,
      std::shared_ptr<renderstack::graphics::buffer>     uniform_buffer
   );

   void geometry_pass(
      std::vector<std::shared_ptr<class model>> const &models,
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
   std::shared_ptr<renderstack::graphics::renderer>               m_renderer;
   std::shared_ptr<programs>                                      m_programs;
   std::shared_ptr<renderstack::graphics::uniform_buffer_range>   m_mesh_render_uniform_buffer_range;
   renderstack::graphics::render_states                           m_mesh_render_states;
   renderstack::graphics::render_states                           m_light_render_states;
   renderstack::graphics::render_states                           m_show_rt_render_states;

   // framebuffer
   unsigned int                                    m_fbo;
   std::shared_ptr<renderstack::graphics::texture> m_rt[4];
   std::shared_ptr<renderstack::graphics::texture> m_depth;

   std::shared_ptr<quad_renderer>                  m_quad_renderer;
};


#endif
