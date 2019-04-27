#ifndef forward_renderer_hpp_renderers
#define forward_renderer_hpp_renderers

#include "main/programs.hpp"
#include "renderers/base_renderer.hpp"
#include "renderstack_graphics/renderer.hpp"
#include "renderstack_toolkit/platform.hpp"
#include "renderstack_toolkit/service.hpp"
#include "scene/model.hpp"
#include <glm/glm.hpp>
#include <memory>
#include <vector>

namespace renderstack
{
namespace mesh
{
class Geometry_mesh;
}
} // namespace renderstack

namespace renderstack
{
namespace graphics
{
class Uniform_buffer;
class Uniform_buffer_range;
} // namespace graphics
} // namespace renderstack

namespace renderstack
{
namespace scene
{
class Camera;
class Light;
} // namespace scene
} // namespace renderstack

class Debug_renderer;
class Model;

class Forward_renderer
    : public renderstack::toolkit::service,
      public Base_renderer
{
public:
    Forward_renderer();

    virtual ~Forward_renderer() = default;

    void connect(std::shared_ptr<renderstack::graphics::Renderer> renderer,
                 std::shared_ptr<Debug_renderer>                  debug_renderer,
                 std::shared_ptr<Programs>                        programs);

    void initialize_service() override;

    void render_pass(const Material_collection        &materials,
                     const Model_collection           &models,
                     const Light_collection           &lights,
                     const renderstack::scene::Camera &camera);

private:
    void update_light_model(renderstack::scene::Light *l);

    void print_matrix(glm::mat4 const &m, const std::string &desc);

    // services
    std::shared_ptr<Debug_renderer> m_debug_renderer;

    renderstack::graphics::Render_states m_first_pass_render_states;
    renderstack::graphics::Render_states m_other_pass_render_states;

    std::vector<std::shared_ptr<Material>> m_placeholder_materials;
};

#endif
