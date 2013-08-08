#include "renderstack_graphics/renderer.hpp"
#include "logic/selection_manager.hpp"
#include "logic/manipulator_manager.hpp"
#include "scene/scene_manager.hpp"
#include "scene/model.hpp"
#include "renderers/id_renderer.hpp"
#include "renderers/debug_renderer.hpp"


using namespace std;


selection_manager::selection_manager()
:  service("selection_manager")
,  m_renderer           (nullptr)
,  m_debug_renderer     (nullptr)
,  m_id_renderer        (nullptr)
,  m_manipulator_manager(nullptr)
,  m_scene_manager      (nullptr)
{
}

selection_manager::~selection_manager()
{
}


/*virtual*/ void selection_manager::initialize_service()
{
}

void selection_manager::connect(
   shared_ptr<renderstack::graphics::renderer>  renderer_,
   shared_ptr<debug_renderer>                   debug_renderer_,
   shared_ptr<id_renderer>                      id_renderer_,
   shared_ptr<manipulator_manager>              manipulator_manager_,
   shared_ptr<scene_manager>                    scene_manager_
)
{
   m_renderer              = renderer_;
   m_debug_renderer        = debug_renderer_;
   m_id_renderer           = id_renderer_;
   m_manipulator_manager   = manipulator_manager_;
   m_scene_manager         = scene_manager_;

   initialization_depends_on(m_manipulator_manager);
}

/*virtual*/ void initialize_service()
{
}

void selection_manager::clear_hover()
{
   m_hover.clear();
}

void selection_manager::clear_selection()
{
   for (auto i = m_models.cbegin(); i != m_models.cend(); ++i)
   {
   }
}

void selection_manager::remove(std::shared_ptr<model> m)
{
}

void selection_manager::add(std::shared_ptr<model> m)
{
}

bool selection_manager::contains(std::shared_ptr<model> m) const
{
   return false;
}

void selection_manager::reset_hover()
{
}

void selection_manager::update()
{
}

void selection_manager::render_bounding_boxes()
{
}

void selection_manager::render()
{
}

