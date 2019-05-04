#include "logic/manipulator_manager.hpp"

using namespace std;

manipulator_manager::manipulator_manager()
    : service("manipulator_manager"), m_renderer(nullptr), m_material_manager(nullptr), m_debug_renderer(nullptr), m_scene_manager(nullptr), m_selection_manager(nullptr)
{
}

manipulator_manager::~manipulator_manager()
{
}

void manipulator_manager::connect(
    shared_ptr<renderstack::graphics::Renderer> renderer,
    shared_ptr<Material_manager>                material_manager,
    shared_ptr<Debug_renderer>                  debug_renderer,
    shared_ptr<Scene_manager>                   scene_manager,
    shared_ptr<Selection_manager>               selection_manager)
{
    m_renderer          = renderer;
    m_material_manager  = material_manager;
    m_debug_renderer    = debug_renderer;
    m_scene_manager     = scene_manager;
    m_selection_manager = selection_manager;

    initialization_depends_on(renderer);
    initialization_depends_on(material_manager);
    initialization_depends_on(scene_manager);
}

void manipulator_manager::initialize_service()
{
}
