#ifndef model_hpp_scene
#define model_hpp_scene

#include "renderstack_mesh/geometry_mesh.hpp"
#include "renderstack_scene/frame.hpp"
#include "renderstack_toolkit/platform.hpp"
#include "scene/material.hpp"
#include <memory>

struct Material;

class Model
{
public:
    Model() = default;

    Model(const std::string &name)
        : name(name)
    {}

    Model(const std::string &                               name,
          std::shared_ptr<renderstack::mesh::Geometry_mesh> mesh,
          std::shared_ptr<Material>                         material,
          glm::mat4 const &                                 transform);

    ~Model() = default;

    glm::vec3 position() const;

    std::string               name;
    renderstack::scene::Frame frame;
    bool                      selected{false};
    std::shared_ptr<Material> material;

    std::shared_ptr<renderstack::mesh::Geometry_mesh> geometry_mesh;
};

using Model_collection = std::vector<std::shared_ptr<Model>>;

#endif
