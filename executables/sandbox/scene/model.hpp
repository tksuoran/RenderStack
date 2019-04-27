#ifndef model_hpp_scene
#define model_hpp_scene

#include "renderstack_mesh/geometry_mesh.hpp"
#include "renderstack_scene/frame.hpp"
#include "renderstack_toolkit/platform.hpp"
#include "scene/material.hpp"
#include <memory>

class material;

class model
{
public:
    model();
    model(std::string const &name);
    model(
        std::string const &                               name,
        std::shared_ptr<renderstack::mesh::geometry_mesh> mesh,
        std::shared_ptr<material>                         material,
        glm::mat4 const &                                 transform);

    std::shared_ptr<renderstack::scene::frame> frame() const
    {
        return m_frame;
    }
    std::string const &name()
    {
        return m_name;
    }

    std::shared_ptr<class renderstack::mesh::geometry_mesh> geometry_mesh()
    {
        return m_geometry_mesh;
    }
    void set_geometry_mesh(std::shared_ptr<renderstack::mesh::geometry_mesh> value)
    {
        m_geometry_mesh = value;
    }

    std::shared_ptr<class material> material() const
    {
        return m_material;
    }
    void set_material(std::shared_ptr<class material> value)
    {
        m_material = value;
    }

    bool selected() const
    {
        return m_selected;
    }
    void select()
    {
        m_selected = true;
    }
    void deselect()
    {
        m_selected = false;
    }

    glm::vec3 position() const;

private:
    std::string                                             m_name;
    std::shared_ptr<renderstack::scene::frame>              m_frame;
    std::shared_ptr<class renderstack::mesh::geometry_mesh> m_geometry_mesh;
    std::shared_ptr<class material>                         m_material;
    bool                                                    m_selected;
};

#endif
