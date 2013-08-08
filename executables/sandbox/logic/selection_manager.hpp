#ifndef selection_manager_hpp_logic
#define selection_manager_hpp_logic

#include "renderstack_toolkit/platform.hpp"
#include "renderstack_toolkit/service.hpp"
#include <glm/glm.hpp>
#include <set>

namespace renderstack {
   namespace geometry {
      class polygon;
      class point;
   }
   namespace graphics {
      class renderer;
   }
}

class debug_renderer;
class id_renderer;
class manipulator_manager;
class scene_manager;
class model;
class group;

class selection_manager : public renderstack::toolkit::service
{
public:
   selection_manager();
   /*virtual*/ ~selection_manager();

   void connect(
      std::shared_ptr<renderstack::graphics::renderer>   renderer,
      std::shared_ptr<debug_renderer>                    debug_renderer_,
      std::shared_ptr<id_renderer>                       id_renderer_,
      std::shared_ptr<manipulator_manager>               manipulator_manager_,
      std::shared_ptr<scene_manager>                     scene_manager_
   );

   /*virtual*/ void initialize_service();

   void clear_hover();
   void clear_selection();
   void remove(std::shared_ptr<model> m);
   void add(std::shared_ptr<model> m);
   bool contains(std::shared_ptr<model> m) const;
   void reset_hover();
   void update();
   void render_bounding_boxes();
   void render();

private: /* services */
   std::shared_ptr<renderstack::graphics::renderer>   m_renderer;
   std::shared_ptr<debug_renderer>                    m_debug_renderer;
   std::shared_ptr<id_renderer>                       m_id_renderer;
   std::shared_ptr<manipulator_manager>               m_manipulator_manager;
   std::shared_ptr<scene_manager>                     m_scene_manager;

private:
   struct hover
   {
      std::shared_ptr<class model>  model;
      std::shared_ptr<class group>  group;
      class polygon                 *polygon;
      class point                   *point;
      float                         depth;
      glm::vec3                        position;

      void clear()
      {
         model.reset();
         group.reset();
         polygon = nullptr;
         point = nullptr;
         depth = 0.0f;
         position = glm::vec3(0.0f);
      }
   };
   std::set<std::shared_ptr<model> >   m_models;
   hover                               m_hover;
};



#endif

