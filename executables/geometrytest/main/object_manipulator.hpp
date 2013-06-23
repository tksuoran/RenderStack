#ifndef object_manipulator_hpp
#define object_manipulator_hpp

#include "renderstack_toolkit/platform.hpp"
#include "renderstack_graphics/renderer.hpp"
#include "main/programs.hpp"
#include <glm/glm.hpp>
#include <vector>
#include <memory>

class object_manipulator
{
public:
   object_manipulator(
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

   std::shared_ptr<class model> get(int x, int y);

private:
   std::shared_ptr<renderstack::graphics::renderer>               m_renderer;
   std::shared_ptr<programs>                                      m_programs;
   std::shared_ptr<renderstack::graphics::uniform_buffer_range>   m_id_render_uniform_buffer_range;
   renderstack::graphics::render_states                           m_id_render_states;

   // pixel pack buffers
   int         m_last_render;
   int         m_radius;
   id_render   m_renders[4];

   struct id_range
   {
      uint32_t                      offset;
      uint32_t                      length;
      std::shared_ptr<class model>  model;
   };

   std::vector<id_range>   m_ranges;
};


#endif
