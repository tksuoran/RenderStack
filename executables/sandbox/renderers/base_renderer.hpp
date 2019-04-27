#ifndef base_renderer_hpp_renderers
#define base_renderer_hpp_renderers

#include "main/programs.hpp"
#include "renderers/light_mesh.hpp"
#include "renderers/quad_renderer.hpp"
#include "renderstack_toolkit/platform.hpp"
#include "renderstack_toolkit/service.hpp"
#include "scene/model.hpp"
#include <array>
#include <glm/glm.hpp>
#include <memory>
#include <vector>

#define NUM_UNIFORM_BUFFERS 3

namespace renderstack
{
namespace graphics
{
class Uniform_buffer;
class Uniform_buffer_range;
} // namespace graphics
namespace mesh
{
class Geometry_mesh;
}
namespace scene
{
class Camera;
class Light;
struct Viewport;
} // namespace scene
} // namespace renderstack

class Ring_uniform_buffer
{
public:
    void initialize(renderstack::graphics::Renderer &renderer,
                    size_t                           num,
                    size_t                           stride)
    {
        for (size_t i = 0; i < m_storage.size(); ++i)
        {
            m_storage[i] = std::make_shared<renderstack::graphics::Buffer>(
                renderstack::graphics::Buffer::Target::uniform_buffer,
                num,
                stride,
                gl::buffer_usage_hint::stream_draw);
            m_storage[i]->allocate_storage(renderer);
        }
    }

    std::shared_ptr<renderstack::graphics::Buffer> &current_buffer()
    {
        return m_storage[m_current_buffer];
    }

    void advance()
    {
        m_current_buffer = (m_current_buffer + 1) % NUM_UNIFORM_BUFFERS;
    }

private:
    std::array<std::shared_ptr<renderstack::graphics::Buffer>, NUM_UNIFORM_BUFFERS> m_storage;
    size_t m_current_buffer{0};
};

enum uniform_buffer_usage
{
    models       = 0,
    lights       = 1,
    light_models = 2,
    materials    = 3,
    camera       = 4,
    count        = 5
};

class Light_mesh;
struct Material;
class Programs;

class Base_renderer
{
public:
    using Light_collection = std::vector<std::shared_ptr<renderstack::scene::Light>>;
    using Material_collection = std::vector<std::shared_ptr<Material>>;

    virtual ~Base_renderer() = default;

    static void bind_default_framebuffer();

    void set_max_lights(size_t max_lights);

    size_t max_lights()
    {
        return m_max_lights;
    }

    bool use_stencil() const
    {
        return m_use_stencil;
    }

    int scale() const
    {
        return m_scale;
    }

    void set_use_stencil(bool value);

    void set_scale(int value);

    void base_resize(int width, int height);

    int  width();

    int  height();

    int  width_full()
    {
        return m_width_full;
    }

    int height_full()
    {
        return m_height_full;
    }

    renderstack::graphics::Renderer &renderer();

    std::shared_ptr<Programs> programs();

    std::shared_ptr<Light_mesh> light_mesh()
    {
        return m_light_mesh;
    }

    void bind_camera();

    void bind_model(size_t model_index);

    void bind_light_model(size_t light_index);

    void bind_light(size_t light_index);

    void bind_material(size_t material_index);

    bool point_in_light(std::shared_ptr<renderstack::scene::Light> l);

protected:
    void base_connect(std::shared_ptr<renderstack::graphics::Renderer> renderer,
                      std::shared_ptr<Programs>                        programs,
                      std::shared_ptr<Light_mesh>                      light_mesh);

    void base_initialize_service();

protected:
    unsigned char *begin_edit(uniform_buffer_usage buffer, size_t count);

    void write(unsigned char *const dst, const float *const src, size_t byte_count);

    void end_edit(uniform_buffer_usage buffer);

    void update_models(const Model_collection &models, const renderstack::scene::Camera &camera);

    void update_lights_models(const Light_collection &lights, const renderstack::scene::Camera &camera);

    void update_lights(const Light_collection &lights, const renderstack::scene::Camera &camera);

    void update_materials(const Material_collection &materials);

    void update_camera(const renderstack::scene::Camera &camera);

private:
    std::shared_ptr<renderstack::graphics::Renderer> m_renderer;
    std::shared_ptr<Programs>                        m_programs;
    std::shared_ptr<Light_mesh>                      m_light_mesh;

    std::array<Ring_uniform_buffer, uniform_buffer_usage::count> m_uniform_buffers;

    int m_width{0};
    int m_height{0};
    int m_width_full{0};
    int m_height_full{0};

    size_t m_max_lights{0};
    bool   m_use_stencil{true};
    int    m_scale{1};

    intptr_t m_edit_start{0};
    intptr_t m_edit_byte_count{0};
};

#endif
