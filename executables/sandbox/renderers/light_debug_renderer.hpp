#ifndef light_debug_renderer_hpp_renderers
#define light_debug_renderer_hpp_renderers

#include "renderstack_toolkit/platform.hpp"
#include "renderers/base_renderer.hpp"
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

class light_mesh;

class light_debug_renderer
:  public renderstack::toolkit::service
,  public base_renderer
{
public:
   light_debug_renderer();
   /*virtual*/ ~light_debug_renderer();

   void connect(
      std::shared_ptr<renderstack::graphics::renderer>   renderer,
      std::shared_ptr<class programs>                    programs,
      std::shared_ptr<class light_mesh>                  light_mesh
   );
   /*virtual/*/ void initialize_service();

   void light_pass(
      std::vector<std::shared_ptr<renderstack::scene::light> > const &lights,
      std::shared_ptr<renderstack::scene::camera> camera
   );

private:
   renderstack::graphics::render_states m_debug_light_render_states;

};


#endif
