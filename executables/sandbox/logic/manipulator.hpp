#ifndef manipulator_hpp
#define manipulator_hpp

#include "renderstack_scene/frame.hpp"
#include "renderstack_toolkit/platform.hpp"
#include "scene/model.hpp"
#include <memory>

class Model;
class manipulator_manager;

class manipulator
{
public:
    manipulator();

    Model *parent;
    bool   active;

private:
    std::shared_ptr<manipulator_manager>       m_manager;
    std::shared_ptr<renderstack::scene::Frame> m_transform;

    renderstack::scene::Transform m_initial_parent_from_local;
    renderstack::scene::Transform m_initial_world_from_local;

    std::shared_ptr<Model> m_x_cylinder;
    std::shared_ptr<Model> m_x_cone;
    std::shared_ptr<Model> m_y_cylinder;
    std::shared_ptr<Model> m_y_cone;
    std::shared_ptr<Model> m_z_cylinder;
    std::shared_ptr<Model> m_z_cone;
};

#endif
