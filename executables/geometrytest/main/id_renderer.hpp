#ifndef id_renderer__hpp
#define id_renderer__hpp

#include "renderstack_toolkit/platform.hpp"
#include "renderstack_graphics/renderer.hpp"
#include "main/programs.hpp"
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
}

class model;

class id_renderer
{
public:
   id_renderer(
      std::shared_ptr<renderstack::graphics::renderer>   renderer,
      std::shared_ptr<programs>                          programs,
      std::shared_ptr<renderstack::graphics::buffer>     uniform_buffer
   );

   void clear();

   void render_pass(
      std::vector<std::shared_ptr<class model>> const &models,
      glm::mat4 const &clip_from_world
   );

private:
   std::shared_ptr<renderstack::graphics::renderer>               m_renderer;
   std::shared_ptr<programs>                                      m_programs;
   std::shared_ptr<renderstack::graphics::uniform_buffer_range>   m_id_render_uniform_buffer_range;
   renderstack::graphics::render_states                           m_id_render_states;
};


#endif
