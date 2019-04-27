#ifndef manipulator_manager_hpp_logic
#define manipulator_manager_hpp_logic

#include "logic/selection_manager.hpp"
#include "renderers/debug_renderer.hpp"
#include "renderstack_graphics/renderer.hpp"
#include "renderstack_toolkit/platform.hpp"
#include "renderstack_toolkit/service.hpp"
#include "scene/scene_manager.hpp"

class Material_manager;
class Debug_renderer;

class manipulator_manager : public renderstack::toolkit::service
{
public:
    manipulator_manager();
    virtual ~manipulator_manager();

    void connect(std::shared_ptr<renderstack::graphics::Renderer> renderer_,
                 std::shared_ptr<Material_manager>                material_manager_,
                 std::shared_ptr<Debug_renderer>                  debug_renderer_,
                 std::shared_ptr<Scene_manager>                   scene_manager_,
                 std::shared_ptr<selection_manager>               selection_manager_);

    void initialize_service() override;

private:
    std::shared_ptr<renderstack::graphics::Renderer> m_renderer;
    std::shared_ptr<Material_manager>                m_material_manager;
    std::shared_ptr<Debug_renderer>                  m_debug_renderer;
    std::shared_ptr<Scene_manager>                   m_scene_manager;
    std::shared_ptr<selection_manager>               m_selection_manager;
};

#endif
