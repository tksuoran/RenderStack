#ifndef light_debug_renderer_hpp_renderers
#define light_debug_renderer_hpp_renderers

#include "renderstack_toolkit/platform.hpp"
#include "renderstack_toolkit/service.hpp"
#include "main/programs.hpp"
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

class light_debug_renderer : public renderstack::toolkit::service
{
public:
   light_debug_renderer();
   /*virtual*/ ~light_debug_renderer();

   void connect(
      std::shared_ptr<renderstack::graphics::renderer>   renderer,
      std::shared_ptr<programs>                          programs
   );
   /*virtual/*/ void initialize_service();

   void set_max_lights(int max_lights);

   void light_pass(
      std::shared_ptr<std::vector<std::shared_ptr<renderstack::scene::light> > > lights,
      std::shared_ptr<renderstack::scene::camera> camera
   );

private:
   void update_light_model(std::shared_ptr<renderstack::scene::light> l);

private:
   std::shared_ptr<renderstack::graphics::renderer>   m_renderer;
   std::shared_ptr<programs>                          m_programs;

   renderstack::graphics::render_states               m_debug_light_render_states;

   std::shared_ptr<renderstack::graphics::buffer>                 m_uniform_buffer;
   ubr_pos                                                        m_ubr_sizes;
   std::shared_ptr<renderstack::graphics::uniform_buffer_range>   m_model_ubr;
   std::shared_ptr<renderstack::graphics::uniform_buffer_range>   m_camera_ubr;
   std::shared_ptr<renderstack::graphics::uniform_buffer_range>   m_material_ubr;
   std::shared_ptr<renderstack::graphics::uniform_buffer_range>   m_lights_ubr;

   std::map<
      std::shared_ptr<renderstack::scene::light>,
      std::shared_ptr<renderstack::mesh::geometry_mesh>
   >                                                  m_light_meshes; 
   int m_max_lights;
};


#endif
