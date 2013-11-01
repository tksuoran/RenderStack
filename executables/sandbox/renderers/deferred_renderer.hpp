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
   namespace scene
   {
      class camera;
      class light;
      class viewport;
   }
}

class light_mesh;
class material;

class deferred_renderer : public renderstack::toolkit::service
{
public:
   deferred_renderer();
   /*virtual*/ ~deferred_renderer();

   void connect(
      std::shared_ptr<renderstack::graphics::renderer>   renderer,
      std::shared_ptr<programs>                          programs,
      std::shared_ptr<quad_renderer>                     quad_renderer,
      std::shared_ptr<light_mesh>                        light_mesh
   );
   /*virtual/*/ void initialize_service();

   void set_max_lights(int max_lights);

   void geometry_pass(
      std::shared_ptr<std::vector<std::shared_ptr<material> > > materials,
      std::shared_ptr<std::vector<std::shared_ptr<model> > > models,
      std::shared_ptr<renderstack::scene::camera> camera
   );
   void light_pass(
      std::shared_ptr<std::vector<std::shared_ptr<renderstack::scene::light> > > lights,
      std::shared_ptr<renderstack::scene::camera> camera
   );
   void show_rt();

   void resize(int width, int height);

private:
   void fbo_clear();
   void bind_gbuffer_fbo();
   void bind_linear_fbo();
   void bind_default_framebuffer();

private:
   std::shared_ptr<renderstack::graphics::renderer>   m_renderer;
   std::shared_ptr<programs>                          m_programs;
   std::shared_ptr<quad_renderer>                     m_quad_renderer;
   std::shared_ptr<light_mesh>                        m_light_mesh;

   std::shared_ptr<renderstack::graphics::buffer>                 m_uniform_buffer;
   ubr_pos                                                        m_ubr_sizes;
   std::shared_ptr<renderstack::graphics::uniform_buffer_range>   m_model_ubr;
   std::shared_ptr<renderstack::graphics::uniform_buffer_range>   m_camera_ubr;
   std::shared_ptr<renderstack::graphics::uniform_buffer_range>   m_material_ubr;
   std::shared_ptr<renderstack::graphics::uniform_buffer_range>   m_lights_ubr;

   renderstack::graphics::render_states               m_mesh_render_states;
   renderstack::graphics::render_states               m_light_render_states;
   renderstack::graphics::render_states               m_show_rt_render_states;
   renderstack::graphics::render_states               m_camera_render_states;

   // framebuffer
   int                                                m_width;
   int                                                m_height;
   unsigned int                                       m_gbuffer_fbo;
   std::shared_ptr<renderstack::graphics::texture>    m_gbuffer_rt[3];
   std::shared_ptr<renderstack::graphics::texture>    m_depth;

   unsigned int                                       m_linear_fbo;
   std::shared_ptr<renderstack::graphics::texture>    m_linear_rt[3];

   int                                                m_max_lights;
};


#endif
