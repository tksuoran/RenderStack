#ifndef scene_manager_hpp_scene
#define scene_manager_hpp_scene

#include "renderstack_mesh/build_info.hpp"
#include "renderstack_mesh/geometry_mesh.hpp"
#include "renderstack_toolkit/platform.hpp"
#include "renderstack_toolkit/service.hpp"
#include "scene/material_manager.hpp"
#include "util/frame_controller.hpp"
#include <memory>
#include <vector>

namespace renderstack
{
namespace geometry
{
class Geometry;
}
} // namespace renderstack

namespace renderstack
{
namespace mesh
{
class mesh;
} // namespace mesh
} // namespace renderstack

namespace renderstack
{
namespace scene
{
class Camera;
class Light;
} // namespace scene
} // namespace renderstack

struct Material;
class Model;
class Programs;

class Scene_manager : public renderstack::toolkit::service
{
public:
    Scene_manager();

    virtual ~Scene_manager() = default;

    void connect(std::shared_ptr<Programs>                        programs_,
                 std::shared_ptr<renderstack::graphics::Renderer> renderer_);

    void initialize_service() override;

    void add_floor(float size);

    void add_simple_scene();

    void initialize_cameras();

    std::shared_ptr<Model> add(std::shared_ptr<Model> m);

    std::shared_ptr<renderstack::scene::Light> add(std::shared_ptr<renderstack::scene::Light> l);

    renderstack::scene::Camera& camera()
    {
        auto camera = m_camera.get();
        assert(camera);
        return *camera;
    }

    const renderstack::scene::Camera& camera() const
    {
        auto camera = m_camera.get();
        assert(camera);
        return *camera;
    }

    std::vector<std::shared_ptr<Material>> &materials()
    {
        return m_materials;
    }

    const std::vector<std::shared_ptr<Material>> &materials() const
    {
        return m_materials;
    }

    std::vector<std::shared_ptr<Model>> &      models();
    std::vector<std::shared_ptr<Model>> const &models() const;

    std::vector<std::shared_ptr<renderstack::scene::Light>> &      lights();
    std::vector<std::shared_ptr<renderstack::scene::Light>> const &lights() const;

private:
    void reset_build_info();

    std::shared_ptr<renderstack::mesh::Geometry_mesh> make_geometry_mesh(const renderstack::geometry::Geometry &geometry);

    std::shared_ptr<Model> make_model(
        const std::string                                 &name,
        std::shared_ptr<renderstack::scene::Frame>        parent,
        std::shared_ptr<renderstack::mesh::Geometry_mesh> gm,
        std::shared_ptr<Material>                         material,
        glm::vec3                                         position = glm::vec3(0.0f));

private:
    // services
    std::shared_ptr<Programs>                        m_programs;
    std::shared_ptr<renderstack::graphics::Renderer> m_renderer;

    // self owned parts
    std::vector<std::shared_ptr<Material>>                  m_materials;
    std::vector<std::shared_ptr<Model>>                     m_models;
    std::vector<std::shared_ptr<renderstack::scene::Light>> m_lights;
    std::shared_ptr<renderstack::scene::Camera>             m_camera;
    std::shared_ptr<frame_controller>                       m_camera_controls;

    renderstack::mesh::Geometry_mesh::Format_info m_format_info;
    renderstack::mesh::Geometry_mesh::Buffer_info m_buffer_info;
};

#endif
