#ifndef selection_manager_hpp_logic
#define selection_manager_hpp_logic

#include "renderstack_toolkit/platform.hpp"
#include "renderstack_toolkit/service.hpp"
#include <glm/glm.hpp>
#include <set>

namespace renderstack
{
namespace geometry
{
class Polygon;
class Point;
} // namespace geometry
namespace graphics
{
class Renderer;
}
} // namespace renderstack

class Debug_renderer;
class Id_renderer;
class manipulator_manager;
class Scene_manager;
class Model;
class Group;

class selection_manager : public renderstack::toolkit::service
{
public:
    selection_manager();

    virtual ~selection_manager() = default;

    void connect(std::shared_ptr<renderstack::graphics::Renderer> Renderer,
                 std::shared_ptr<Debug_renderer>                  debug_renderer_,
                 std::shared_ptr<Id_renderer>                     id_renderer_,
                 std::shared_ptr<manipulator_manager>             manipulator_manager_,
                 std::shared_ptr<Scene_manager>                   scene_manager_);

    void initialize_service() override;

    void clear_hover();

    void clear_selection();

    void remove(std::shared_ptr<Model> m);

    void add(std::shared_ptr<Model> m);
    
    bool contains(std::shared_ptr<Model> m) const;

    void reset_hover();

    void update();

    void render_bounding_boxes();

    void render();

private:
    std::shared_ptr<renderstack::graphics::Renderer> m_renderer;
    std::shared_ptr<Debug_renderer>                  m_debug_renderer;
    std::shared_ptr<Id_renderer>                     m_id_renderer;
    std::shared_ptr<manipulator_manager>             m_manipulator_manager;
    std::shared_ptr<Scene_manager>                   m_scene_manager;

private:
    struct hover
    {
        std::shared_ptr<Model>          Model;
        std::shared_ptr<Group>          group;
        renderstack::geometry::Polygon  *polygon{nullptr};
        renderstack::geometry::Point    *point{nullptr};
        float                           depth;
        glm::vec3                       position;

        void clear()
        {
            Model.reset();
            group.reset();
            polygon  = nullptr;
            point    = nullptr;
            depth    = 0.0f;
            position = glm::vec3(0.0f);
        }
    };
    std::set<std::shared_ptr<Model>> m_models;
    hover                            m_hover;
};

#endif
