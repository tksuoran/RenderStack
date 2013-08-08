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

namespace renderstack { namespace graphics {

class uniform_buffer;
class uniform_buffer_range;

} }

class model;

class forward_renderer : public renderstack::toolkit::service
{
public:
   forward_renderer();
   /*virtual*/ ~forward_renderer();

   void connect(
      std::shared_ptr<renderstack::graphics::renderer>   renderer,
      std::shared_ptr<programs>                          programs
   );

   /*virtual*/ void initialize_service();

   void render_pass(
      std::shared_ptr<class group> group,
      glm::mat4 const &clip_from_world,
      glm::mat4 const &view_from_world
   );

private:
   std::shared_ptr<renderstack::graphics::renderer>   m_renderer;
   std::shared_ptr<programs>                          m_programs;

   renderstack::graphics::render_states               m_mesh_render_states;
};


#endif
