#include "logic/manipulator_manager.hpp"


using namespace std;


manipulator_manager::manipulator_manager()
:  service("manipulator_manager")
,  m_renderer(nullptr)
,  m_material_manager(nullptr)
,  m_debug_renderer(nullptr)
,  m_scene_manager(nullptr)
,  m_selection_manager(nullptr)
{
}

/* virtual */ manipulator_manager::~manipulator_manager()
{
}

void manipulator_manager::connect(
   shared_ptr<renderstack::graphics::renderer>  renderer_,
   shared_ptr<material_manager>                 material_manager_,
   shared_ptr<debug_renderer>                   debug_renderer_,
   shared_ptr<scene_manager>                    scene_manager_,
   shared_ptr<selection_manager>                selection_manager_
)
{
   m_renderer           = renderer_;
   m_material_manager   = material_manager_;
   m_debug_renderer     = debug_renderer_;
   m_scene_manager      = scene_manager_;
   m_selection_manager  = selection_manager_;
      
   initialization_depends_on(renderer_);
   initialization_depends_on(material_manager_);
   initialization_depends_on(scene_manager_);
}

/*virtual*/ void manipulator_manager::initialize_service()
{
}
