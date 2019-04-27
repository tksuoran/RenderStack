#ifndef manipulator_hpp
#define manipulator_hpp

#include "renderstack_scene/frame.hpp"
#include "renderstack_toolkit/platform.hpp"
#include "scene/model.hpp"
#include <memory>

class model;
class manipulator_manager;

class manipulator
{
public:
    manipulator();

    std::shared_ptr<model> parent()
    {
        return m_parent;
    }
    void set_parent(std::shared_ptr<model> value)
    {
        m_parent = value;
        m_transform->set_parent(m_parent->frame());
    }
    bool active() const
    {
        return m_active;
    }

    void set_active(bool value)
    {
        if (m_active == value)
            return;
    }

private:
    std::shared_ptr<manipulator_manager>       m_manager;
    bool                                       m_active;
    std::shared_ptr<renderstack::scene::frame> m_transform;

    renderstack::scene::transform m_initial_parent_from_local;
    renderstack::scene::transform m_initial_world_from_local;

    std::shared_ptr<model> m_parent;
    std::shared_ptr<model> m_x_cylinder;
    std::shared_ptr<model> m_x_cone;
    std::shared_ptr<model> m_y_cylinder;
    std::shared_ptr<model> m_y_cone;
    std::shared_ptr<model> m_z_cylinder;
    std::shared_ptr<model> m_z_cone;
};

#endif
