#ifndef light_mesh_hpp_renderers
#define light_mesh_hpp_renderers

#include "renderstack_toolkit/platform.hpp"
#include "renderstack_toolkit/service.hpp"
#include <glm/glm.hpp>
#include <map>
#include <memory>

namespace renderstack
{
namespace graphics
{
class Renderer;
}
namespace mesh
{
class Geometry_mesh;
}
namespace scene
{
class Light;
}
} // namespace renderstack

class Programs;

class Light_mesh : public renderstack::toolkit::service
{
public:
    Light_mesh();

    virtual ~Light_mesh() = default;

    void connect(std::shared_ptr<renderstack::graphics::Renderer> renderer,
                 std::shared_ptr<Programs>                  programs);

    void initialize_service() override;

    glm::mat4 get_light_transform(std::shared_ptr<renderstack::scene::Light> l);

    bool point_in_light(glm::vec3 point_in_world, std::shared_ptr<renderstack::scene::Light> l);

    std::shared_ptr<renderstack::mesh::Geometry_mesh> get_light_mesh(std::shared_ptr<renderstack::scene::Light> l);

private:
    void update_light_model(std::shared_ptr<renderstack::scene::Light> l);

private:
    std::shared_ptr<renderstack::graphics::Renderer> m_renderer;
    std::shared_ptr<Programs>                        m_programs;

    std::shared_ptr<renderstack::mesh::Geometry_mesh> m_quad_mesh;
    std::shared_ptr<renderstack::mesh::Geometry_mesh> m_cone_mesh;

    int m_light_cone_sides;
};

#endif
