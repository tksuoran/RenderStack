#ifndef deferred_renderer_hpp_renderers
#define deferred_renderer_hpp_renderers

#include "main/programs.hpp"
#include "renderers/base_renderer.hpp"
#include "renderers/quad_renderer.hpp"
#include "renderstack_toolkit/platform.hpp"
#include "scene/model.hpp"
#include <glm/glm.hpp>
#include <memory>
#include <vector>

#define NUM_UNIFORM_BUFFERS 3

namespace renderstack
{
namespace graphics
{
class uniform_buffer;
class uniform_buffer_range;
} // namespace graphics
namespace mesh
{
class geometry_mesh;
}
namespace scene
{
class camera;
class light;
class viewport;
} // namespace scene
} // namespace renderstack

class light_mesh;
class material;

class deferred_renderer
    : public renderstack::toolkit::service,
      public base_renderer
{
public:
    deferred_renderer();
    /*virtual*/ ~deferred_renderer();

    void connect(std::shared_ptr<renderstack::graphics::renderer> renderer,
                 std::shared_ptr<class programs>                  programs,
                 std::shared_ptr<quad_renderer>                   quad_renderer,
                 std::shared_ptr<class light_mesh>                light_mesh);
    void initialize_service() override;

    void geometry_pass(std::vector<std::shared_ptr<material>> const &materials,
                       std::vector<std::shared_ptr<model>> const &   models,
                       std::shared_ptr<renderstack::scene::camera>   camera);
    void light_pass(std::vector<std::shared_ptr<renderstack::scene::light>> const &lights,
                    std::shared_ptr<renderstack::scene::camera>                    camera);
    void show_rt();

    void resize(int width, int height);

private:
    void fbo_clear();
    void bind_gbuffer_fbo();
    void bind_linear_fbo();

private:
    std::shared_ptr<quad_renderer> m_quad_renderer;

    renderstack::graphics::render_states m_gbuffer_render_states;
    renderstack::graphics::render_states m_light_stencil_render_states;
    renderstack::graphics::render_states m_light_with_stencil_test_render_states;
    renderstack::graphics::render_states m_light_render_states;
    renderstack::graphics::render_states m_show_rt_render_states;
    renderstack::graphics::render_states m_camera_render_states;

    // framebuffer
    unsigned int                                    m_gbuffer_fbo;
    std::shared_ptr<renderstack::graphics::texture> m_gbuffer_rt[4];
    std::shared_ptr<renderstack::graphics::texture> m_depth;

    unsigned int                                    m_linear_fbo;
    std::shared_ptr<renderstack::graphics::texture> m_linear_rt[4];
    unsigned int                                    m_stencil_rbo;
};

#endif
