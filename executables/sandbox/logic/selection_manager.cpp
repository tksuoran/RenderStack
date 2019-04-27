#include "logic/selection_manager.hpp"
#include "logic/manipulator_manager.hpp"
#include "renderers/debug_renderer.hpp"
#include "renderers/id_renderer.hpp"
#include "renderstack_graphics/renderer.hpp"
#include "scene/model.hpp"
#include "scene/scene_manager.hpp"

using namespace std;

selection_manager::selection_manager()
    : service("selection_manager")
{
}

void selection_manager::initialize_service()
{
}

void selection_manager::connect(shared_ptr<renderstack::graphics::Renderer> renderer_,
                                shared_ptr<Debug_renderer>                  debug_renderer_,
                                shared_ptr<Id_renderer>                     id_renderer_,
                                shared_ptr<manipulator_manager>             manipulator_manager_,
                                shared_ptr<Scene_manager>                   scene_manager_)
{
    m_renderer            = renderer_;
    m_debug_renderer      = debug_renderer_;
    m_id_renderer         = id_renderer_;
    m_manipulator_manager = manipulator_manager_;
    m_scene_manager       = scene_manager_;

    initialization_depends_on(m_manipulator_manager);
}

void initialize_service()
{
}

void selection_manager::clear_hover()
{
    m_hover.clear();
}

void selection_manager::clear_selection()
{
    for (auto i : m_models)
    {
    }
}

void selection_manager::remove(std::shared_ptr<Model> m)
{
}

void selection_manager::add(std::shared_ptr<Model> m)
{
}

bool selection_manager::contains(std::shared_ptr<Model> m) const
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
