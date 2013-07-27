#ifndef manipulator_manager_hpp_logic
#define manipulator_manager_hpp_logic

#include "renderstack_toolkit/platform.hpp"
#include "renderstack_toolkit/service.hpp"
#include "renderstack_graphics/renderer.hpp"
#include "logic/selection_manager.hpp"
#include "scene/scene_manager.hpp"
#include "renderers/debug_renderer.hpp"

class material_manager;
class debug_renderer;

class manipulator_manager : public renderstack::toolkit::service
{
public:
   manipulator_manager();
   /* virtual */ ~manipulator_manager();

   void connect(
      std::shared_ptr<renderstack::graphics::renderer>   renderer_,
      std::shared_ptr<material_manager>                  material_manager_,
      std::shared_ptr<debug_renderer>                    debug_renderer_,
      std::shared_ptr<scene_manager>                     scene_manager_,
      std::shared_ptr<selection_manager>                 selection_manager_
   );

   /*virtual*/ void initialize_service();

private:
   std::shared_ptr<renderstack::graphics::renderer>   m_renderer;
   std::shared_ptr<material_manager>                  m_material_manager;
   std::shared_ptr<debug_renderer>                    m_debug_renderer;
   std::shared_ptr<scene_manager>                     m_scene_manager;
   std::shared_ptr<selection_manager>                 m_selection_manager;
};



#endif

