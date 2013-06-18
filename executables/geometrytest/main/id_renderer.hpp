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

namespace id_render_state
{
   enum value
   {
      unused = 0,
      waiting_for_read = 1,
      read_complete = 2
   };
};

struct id_render
{
   std::shared_ptr<renderstack::graphics::buffer> pixel_pack_buffer;
   double                  time;
   GLsync                  sync;
   glm::mat4               clip_from_world;
   int                     x_offset;
   int                     y_offset;
   id_render_state::value  state;
   uint8_t                 data[16 * 16 * 8];
};

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
      glm::mat4 const &clip_from_world,
      double time,
      int x,
      int y
   );

   bool get(int x, int y, uint32_t &id, float &depth);

private:
   std::shared_ptr<renderstack::graphics::renderer>               m_renderer;
   std::shared_ptr<programs>                                      m_programs;
   std::shared_ptr<renderstack::graphics::uniform_buffer_range>   m_id_render_uniform_buffer_range;
   renderstack::graphics::render_states                           m_id_render_states;

   // pixel pack buffers
   int         m_last_render;
   id_render   m_renders[4];
};


#endif
