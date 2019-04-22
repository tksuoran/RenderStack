#include "renderers/base_renderer.hpp"
#include "main/log.hpp"
#include "renderers/light_mesh.hpp"
#include "renderstack_graphics/buffer.hpp"
#include "renderstack_graphics/configuration.hpp"
#include "renderstack_graphics/program.hpp"
#include "renderstack_graphics/renderer.hpp"
#include "renderstack_graphics/uniform.hpp"
#include "renderstack_graphics/uniform_block.hpp"
#include "renderstack_graphics/vertex_format.hpp"
#include "renderstack_mesh/mesh.hpp"
#include "renderstack_scene/camera.hpp"
#include "renderstack_scene/light.hpp"
#include "renderstack_scene/viewport.hpp"
#include "renderstack_toolkit/gl.hpp"
#include "renderstack_toolkit/math_util.hpp"
#include "renderstack_toolkit/platform.hpp"
#include "renderstack_toolkit/strong_gl_enums.hpp"
#include <cmath>
#include <fstream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iomanip>
#include <sstream>
#include <sys/stat.h>

#define LOG_CATEGORY &log_renderer

using namespace renderstack::toolkit;
using namespace renderstack::graphics;
using namespace renderstack::mesh;
using namespace renderstack::scene;
using namespace renderstack;
using namespace gl;
using namespace glm;
using namespace std;

base_renderer::base_renderer()
    : m_width(0), m_height(0), m_width_full(0), m_height_full(0), m_use_stencil(true), m_scale(1)
{
}

base_renderer::~base_renderer()
{
}

int base_renderer::width()
{
    return m_width;
}

int base_renderer::height()
{
    return m_height;
}

renderstack::graphics::renderer &base_renderer::renderer()
{
    return *m_renderer;
}

std::shared_ptr<class programs> base_renderer::programs()
{
    return m_programs;
}

void base_renderer::bind_default_framebuffer()
{
    log_trace("base_renderer::bind_default_framebuffer()\n");

    gl::bind_framebuffer(gl::framebuffer_target::draw_framebuffer, 0);

#if defined(RENDERSTACK_GL_API_OPENGL)
    gl::draw_buffer(GL_BACK);
#endif

#if defined(RENDERSTACK_GL_API_OPENGL_ES_3)
    GLenum draw_buffer = GL_BACK;
    gl::draw_buffers(1, &draw_buffer);
#endif
}

void base_renderer::bind_camera()
{
    log_trace("base_renderer::bind_camera()\n");

    auto &b = m_uniform_buffers[uniform_buffer_usage::camera].current_buffer();
    b->bind_range(m_programs->camera_block->binding_point(), 0, 1);
}

void base_renderer::bind_model(size_t model_index)
{
    log_trace("base_renderer::bind_model(model_index = %u)\n", static_cast<unsigned int>(model_index));

    auto &b = m_uniform_buffers[uniform_buffer_usage::models].current_buffer();
    b->bind_range(m_programs->models_block->binding_point(), model_index, 1);
}

void base_renderer::bind_light_model(std::size_t light_index)
{
    log_trace("base_renderer::bind_light_model(light_index = %u)\n", static_cast<unsigned int>(light_index));

    auto &b = m_uniform_buffers[uniform_buffer_usage::light_models].current_buffer();
    b->bind_range(m_programs->models_block->binding_point(), light_index, 1);
}

void base_renderer::bind_light(size_t light_index)
{
    log_trace("base_renderer::bind_light(light_index = %u)\n", static_cast<unsigned int>(light_index));

    auto &b = m_uniform_buffers[uniform_buffer_usage::lights].current_buffer();
    b->bind_range(m_programs->lights_block->binding_point(), light_index, 1);
}

void base_renderer::bind_material(size_t material_index)
{
    log_trace("base_renderer::bind_material(material_index = %u)\n", static_cast<unsigned int>(material_index));

    auto &b = m_uniform_buffers[uniform_buffer_usage::materials].current_buffer();
    b->bind_range(m_programs->materials_block->binding_point(), material_index, 1);
}

void base_renderer::base_connect(
    shared_ptr<renderstack::graphics::renderer> renderer_,
    shared_ptr<class programs>                  programs_,
    shared_ptr<class light_mesh>                light_mesh_)
{
    m_renderer   = renderer_;
    m_programs   = programs_;
    m_light_mesh = light_mesh_;
}

void base_renderer::base_initialize_service()
{
    assert(m_renderer);
    assert(m_programs);

    if (renderstack::graphics::configuration::can_use.uniform_buffer_object)
    {
        size_t num_models    = 400;
        size_t num_materials = 100;
        size_t num_lights    = 400;

        auto &r = *m_renderer;
        m_uniform_buffers[uniform_buffer_usage::models].initialize(r, num_models, m_programs->models_block->size_bytes());
        m_uniform_buffers[uniform_buffer_usage::lights].initialize(r, num_lights, m_programs->lights_block->size_bytes());
        m_uniform_buffers[uniform_buffer_usage::light_models].initialize(r, num_lights, m_programs->models_block->size_bytes());
        m_uniform_buffers[uniform_buffer_usage::materials].initialize(r, num_materials, m_programs->materials_block->size_bytes());
        m_uniform_buffers[uniform_buffer_usage::camera].initialize(r, 1, m_programs->camera_block->size_bytes());
    }
}

void base_renderer::set_use_stencil(bool value)
{
    m_use_stencil = value;
}

void base_renderer::set_scale(int value)
{
    m_scale = value;
}

void base_renderer::base_resize(int width, int height)
{
    log_trace("base_renderer::base_resize(width = %d, height = %d)\n", width, height);

    m_width       = width / m_scale;
    m_height      = height / m_scale;
    m_width_full  = width;
    m_height_full = height;
}

void base_renderer::set_max_lights(size_t max_lights)
{
    log_trace("base_renderer::set_max_lights(max_lights = %u)\n", static_cast<unsigned int>(max_lights));

    m_max_lights = max_lights;
}

unsigned char *base_renderer::begin_edit(uniform_buffer_usage buffer, size_t count)
{
    log_trace("base_renderer::begin_edit(buffer_usage = %u, count = %u)\n", static_cast<unsigned int>(buffer), static_cast<unsigned int>(count));

    bool  explicit_flush = (count == 0);
    auto &r              = *m_renderer;
    auto &b              = m_uniform_buffers[buffer].current_buffer();
    if (explicit_flush)
    {
        count = b->capacity();
    }

    assert(count <= b->capacity());
    r.set_buffer(renderstack::graphics::buffer_target::uniform_buffer, b);
    unsigned char *ptr = (unsigned char *)b->map(
        r,
        0,
        count,
        static_cast<gl::buffer_access_mask::value>(
            gl::buffer_access_mask::map_write_bit |
            gl::buffer_access_mask::map_invalidate_buffer_bit |
            (explicit_flush ? gl::buffer_access_mask::map_flush_explicit_bit : 0)));
    m_edit_start      = (intptr_t)(ptr);
    m_edit_byte_count = count * b->stride();
    return ptr;
}

void base_renderer::write(unsigned char *const dst, const float *const src, size_t byte_count)
{
    intptr_t start = (intptr_t)(dst);
    intptr_t end   = (intptr_t)(dst) + byte_count;

    log_trace("base_renderer::write(offset = %u): { ", static_cast<unsigned int>(start - m_edit_start));
    {
        size_t       float_count = byte_count / sizeof(float);
        const float *src_f       = reinterpret_cast<const float *>(src);
        for (size_t i = 0; i < float_count; ++i)
        {
            log_trace_ni("%1.3f", *src_f++);
            if (i < float_count - 1)
            {
                log_trace_ni(", ");
            }
        }
        log_trace_ni(" }\n");
    }

    assert(start >= m_edit_start);
    assert(start < m_edit_start + m_edit_byte_count);
    assert(end >= m_edit_start);
    assert(end <= m_edit_start + m_edit_byte_count);

    ::memcpy(dst, src, byte_count);
}

void base_renderer::end_edit(uniform_buffer_usage buffer)
{
    log_trace("base_renderer::end_edit(buffer_usage = %u)\n", static_cast<unsigned int>(buffer));

    auto &b = m_uniform_buffers[buffer].current_buffer();
    b->unmap(*m_renderer);
    m_edit_start      = 0;
    m_edit_byte_count = 0;
}

void base_renderer::update_models(
    vector<shared_ptr<model>> const &      models,
    shared_ptr<renderstack::scene::camera> camera)
{
    slog_trace("base_renderer::update_models(models.size() = %u)\n", static_cast<unsigned int>(models.size()));

    mat4 const &clip_from_world = camera->clip_from_world().matrix();
    mat4 const &view_from_world = camera->frame()->world_from_local().inverse_matrix();

    size_t         block_size  = m_programs->models_block->size_bytes();
    unsigned char *start       = begin_edit(uniform_buffer_usage::models, models.size());
    size_t         offset      = 0;
    size_t         model_index = 0;
    for (auto model : models)
    {
        log_trace("model_index = %u\n", static_cast<unsigned int>(model_index));

        model->frame()->update_hierarchical_no_cache(); // TODO

        mat4 world_from_model = model->frame()->world_from_local().matrix();
        mat4 clip_from_model  = clip_from_world * world_from_model;
        mat4 view_from_model  = view_from_world * world_from_model;

        write(start + offset + m_programs->models_block_access.clip_from_model, value_ptr(clip_from_model), 16 * sizeof(float));
        write(start + offset + m_programs->models_block_access.view_from_model, value_ptr(view_from_model), 16 * sizeof(float));
        write(start + offset + m_programs->models_block_access.world_from_model, value_ptr(world_from_model), 16 * sizeof(float));
        offset += block_size;
        ++model_index;
    }
    end_edit(uniform_buffer_usage::models);
}

void base_renderer::update_lights_models(
    vector<shared_ptr<renderstack::scene::light>> const &lights,
    shared_ptr<renderstack::scene::camera>               camera)
{
    slog_trace("base_renderer::update_lights_models(lights.size() = %u)\n", static_cast<unsigned int>(lights.size()));

    mat4 const &clip_from_world = camera->clip_from_world().matrix();
    mat4 const &view_from_world = camera->frame()->world_from_local().inverse_matrix();

    assert(m_light_mesh);

    size_t         block_size  = m_programs->models_block->size_bytes();
    unsigned char *start       = begin_edit(uniform_buffer_usage::light_models, lights.size());
    size_t         offset      = 0;
    int            light_index = 0;
    for (auto l : lights)
    {
        log_trace("light_index = %u\n", static_cast<unsigned int>(light_index));

        l->frame()->update_hierarchical_no_cache(); // TODO

        mat4 scale            = m_light_mesh->get_light_transform(l);
        mat4 world_from_light = l->frame()->world_from_local().matrix() * scale;
        mat4 view_from_light  = view_from_world * world_from_light;
        mat4 clip_from_light;

        switch (l->type())
        {
            case light_type::spot:
            {
                clip_from_light = clip_from_world * world_from_light;
                break;
            }

            case light_type::directional:
            {
                clip_from_light = mat4(1.0f);
                break;
            }

            default:
            {
                assert(0);
                break;
            }
        }

        write(start + offset + m_programs->models_block_access.clip_from_model, value_ptr(clip_from_light), 16 * sizeof(float));
        write(start + offset + m_programs->models_block_access.world_from_model, value_ptr(world_from_light), 16 * sizeof(float));
        write(start + offset + m_programs->models_block_access.view_from_model, value_ptr(view_from_light), 16 * sizeof(float));

        offset += block_size;
        ++light_index;
    }
    end_edit(uniform_buffer_usage::light_models);
}

void base_renderer::update_lights(
    vector<shared_ptr<renderstack::scene::light>> const &lights,
    shared_ptr<renderstack::scene::camera>               camera)
{
    slog_trace("base_renderer::update_lights(lights.size() = %u)\n", static_cast<unsigned int>(lights.size()));

    size_t         block_size  = m_programs->lights_block->size_bytes();
    unsigned char *start       = begin_edit(uniform_buffer_usage::lights, lights.size());
    size_t         offset      = 0;
    int            light_index = 0;
    for (auto l : lights)
    {
        log_trace("light_index = %u\n", static_cast<unsigned int>(light_index));

        glm::vec3 position  = vec3(l->frame()->world_from_local().matrix() * vec4(0.0f, 0.0f, 0.0f, 1.0f));
        glm::vec3 direction = vec3(l->frame()->world_from_local().matrix() * vec4(0.0f, 0.0f, 1.0f, 0.0f));
        glm::vec3 radiance  = l->intensity() * l->color();

        direction = normalize(direction);

        // Somewhat unneeded for other than spot lights
        float spot_angle  = l->spot_angle() * 0.5f;
        float spot_cutoff = std::cos(spot_angle);

        write(start + offset + m_programs->lights_block_access.position, value_ptr(position), 3 * sizeof(float));
        write(start + offset + m_programs->lights_block_access.direction, value_ptr(direction), 3 * sizeof(float));
        write(start + offset + m_programs->lights_block_access.radiance, value_ptr(radiance), 3 * sizeof(float));
        write(start + offset + m_programs->lights_block_access.spot_cutoff, &spot_cutoff, 1 * sizeof(float));

        offset += block_size;

        ++light_index;
    }
    end_edit(uniform_buffer_usage::lights);
}

void base_renderer::update_materials(vector<shared_ptr<material>> const &materials)
{
    slog_trace("base_renderer::update_materials(materials.size() = %u)\n", static_cast<unsigned int>(materials.size()));

    size_t         block_size     = m_programs->materials_block->size_bytes();
    unsigned char *start          = begin_edit(uniform_buffer_usage::materials, materials.size());
    size_t         offset         = 0;
    size_t         material_index = 0;
    for (auto material : materials)
    {
        log_trace("material_index = %u\n", static_cast<unsigned int>(material_index));

        vec4  color = material->color();
        float r     = material->roughness();
        float p     = material->isotropy();
        write(start + offset + m_programs->materials_block_access.color, value_ptr(color), 4 * sizeof(float));
        write(start + offset + m_programs->materials_block_access.roughness, &r, sizeof(float));
        write(start + offset + m_programs->materials_block_access.isotropy, &p, sizeof(float));
        offset += block_size;
        ++material_index;
    }
    end_edit(uniform_buffer_usage::materials);
}

void base_renderer::update_camera(shared_ptr<renderstack::scene::camera> camera)
{
    slog_trace("base_renderer::update_camera()\n");
    unsigned char *start = begin_edit(uniform_buffer_usage::camera, 1);
    vec4           vp;
    vp.x                        = static_cast<float>(0);
    vp.y                        = static_cast<float>(0);
    vp.z                        = static_cast<float>(m_width);
    vp.w                        = static_cast<float>(m_height);
    mat4 const &world_from_view = camera->frame()->world_from_local().matrix();
    mat4 const &world_from_clip = camera->clip_from_world().inverse_matrix();
    mat4 const &clip_from_world = camera->clip_from_world().matrix();
    //glm::vec3 view_position_in_world = vec3(world_from_view * vec4(0.0f, 0.0f, 0.0f, 1.0f));
    float exposure = 1.0f;
    write(start + m_programs->camera_block_access.world_from_view, value_ptr(world_from_view), 16 * sizeof(float));
    write(start + m_programs->camera_block_access.world_from_clip, value_ptr(world_from_clip), 16 * sizeof(float));
    write(start + m_programs->camera_block_access.clip_from_world, value_ptr(clip_from_world), 16 * sizeof(float));
    write(start + m_programs->camera_block_access.viewport, value_ptr(vp), 4 * sizeof(float));
    write(start + m_programs->camera_block_access.exposure, &exposure, sizeof(float));
    end_edit(uniform_buffer_usage::camera);
}
