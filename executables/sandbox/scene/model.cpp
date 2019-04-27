#include "scene/model.hpp"

using namespace renderstack::mesh;
using namespace std;
using namespace glm;

Model::Model(const std::string &       name,
             shared_ptr<Geometry_mesh> Geometry_mesh,
             shared_ptr<Material>      material,
             const mat4                &transform)
    : name(name)
    , material(material)
    , geometry_mesh(Geometry_mesh)
{
    frame.parent_from_local.set(transform);
}

glm::vec3 Model::position() const
{
    return glm::vec3(frame.world_from_local.matrix() * vec4(0.0f, 0.0f, 0.0f, 1.0f));
}
