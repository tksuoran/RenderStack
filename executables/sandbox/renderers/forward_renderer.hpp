#ifndef forward_renderer_hpp_renderers
#define forward_renderer_hpp_renderers

#include "renderstack_toolkit/platform.hpp"
#include "renderstack_toolkit/service.hpp"
#include "renderstack_graphics/renderer.hpp"
#include "main/programs.hpp"
#include "scene/model.hpp"
#include <glm/glm.hpp>
#include <vector>
#include <memory>

namespace renderstack { namespace mesh {
   class geometry_mesh;
} }
namespace renderstack { namespace graphics {
   class uniform_buffer;
   class uniform_buffer_range;
} }
namespace renderstack { namespace scene {
   class camera;
   class light;
} }

class debug_renderer;
class model;

class forward_renderer
:  public renderstack::toolkit::service
,  public base_renderer
{
public:
   forward_renderer();
   /*virtual*/ ~forward_renderer();

   void connect(
      std::shared_ptr<renderstack::graphics::renderer> renderer,
      std::shared_ptr<debug_renderer> debug_renderer,
      std::shared_ptr<class programs> programs
   );

   /*virtual*/ void initialize_service();

   void render_pass(
      std::vector<std::shared_ptr<material> > const &materials,
      std::vector<std::shared_ptr<model> > const &models,
      std::vector<std::shared_ptr<renderstack::scene::light> > const &lights,
      std::shared_ptr<renderstack::scene::camera> camera
   );

private:
   void update_light_model(std::shared_ptr<renderstack::scene::light> l);

private:
   void print_matrix(glm::mat4 const &m, std::string const &desc);

private:
   // services
   std::shared_ptr<debug_renderer>           m_debug_renderer;

   renderstack::graphics::render_states      m_first_pass_render_states;
   renderstack::graphics::render_states      m_other_pass_render_states;

   std::vector<std::shared_ptr<material> >   m_placeholder_materials;
};


#endif
