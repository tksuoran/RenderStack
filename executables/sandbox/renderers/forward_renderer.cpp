#include "renderers/forward_renderer.hpp"
#include "renderers/debug_renderer.hpp"
#include "renderstack_geometry/shapes/cone.hpp"
#include "renderstack_graphics/buffer.hpp"
#include "renderstack_graphics/configuration.hpp"
#include "renderstack_graphics/program.hpp"
#include "renderstack_graphics/renderer.hpp"
#include "renderstack_graphics/uniform.hpp"
#include "renderstack_graphics/uniform_block.hpp"
#include "renderstack_graphics/uniform_buffer_range.hpp"
#include "renderstack_graphics/vertex_format.hpp"
#include "renderstack_mesh/geometry_mesh.hpp"
#include "renderstack_mesh/mesh.hpp"
#include "renderstack_scene/camera.hpp"
#include "renderstack_scene/light.hpp"
#include "renderstack_toolkit/gl.hpp"
#include "renderstack_toolkit/math_util.hpp"
#include "renderstack_toolkit/platform.hpp"
#include "renderstack_toolkit/strong_gl_enums.hpp"
#include <fstream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iomanip>
#include <sstream>
#include <sys/stat.h>

using namespace renderstack::toolkit;
using namespace renderstack::graphics;
using namespace renderstack::mesh;
using namespace renderstack::scene;
using namespace renderstack;
using namespace gl;
using namespace glm;
using namespace std;

Forward_renderer::Forward_renderer()
    : service("Forward_renderer")
{
}

void Forward_renderer::connect(
    shared_ptr<Renderer>       renderer,
    shared_ptr<Debug_renderer> debug_renderer,
    shared_ptr<Programs>       programs)
{
    base_connect(renderer, programs, nullptr);

    m_debug_renderer = debug_renderer;

    initialization_depends_on(renderer);
    initialization_depends_on(programs);
}

void Forward_renderer::initialize_service()
{
    base_initialize_service();

    m_first_pass_render_states.depth.enabled     = true;
    m_other_pass_render_states.depth.function    = gl::depth_function::less;
    m_first_pass_render_states.face_cull.enabled = true;
    m_first_pass_render_states.blend.enabled     = false;

    m_other_pass_render_states.depth.enabled                  = true;
    m_other_pass_render_states.depth.function                 = gl::depth_function::l_equal;
    m_other_pass_render_states.face_cull.enabled              = true;
    m_other_pass_render_states.blend.enabled                  = true;
    m_other_pass_render_states.blend.rgb.equation_mode        = gl::blend_equation_mode::func_add;
    m_other_pass_render_states.blend.rgb.source_factor        = gl::blending_factor_src::one;
    m_other_pass_render_states.blend.rgb.destination_factor   = gl::blending_factor_dest::one;
    m_other_pass_render_states.blend.alpha.equation_mode      = gl::blend_equation_mode::func_add;
    m_other_pass_render_states.blend.alpha.source_factor      = gl::blending_factor_src::one;
    m_other_pass_render_states.blend.alpha.destination_factor = gl::blending_factor_dest::one;

    auto m = make_shared<Material>(0,                            // index
                                   "dummy",                      // name
                                   vec4(1.0f, 1.0f, 1.0f, 1.0f), // color
                                   0.10f,                        // roughness
                                   0.02f);                       // isotropy
    m_placeholder_materials.push_back(m);
}

void Forward_renderer::print_matrix(mat4 const &m, const std::string &desc)
{
    m_debug_renderer->printf("Matrix %s:\n"
                             "\t% 6.4f  % 6.4f  % 6.4f  % 6.4f\n"
                             "\t% 6.4f  % 6.4f  % 6.4f  % 6.4f\n"
                             "\t% 6.4f  % 6.4f  % 6.4f  % 6.4f\n"
                             "\t% 6.4f  % 6.4f  % 6.4f  % 6.4f\n",
                             desc.c_str(),
                             m[0][0], m[1][0], m[2][0], m[3][0],
                             m[0][1], m[1][1], m[2][1], m[3][1],
                             m[0][2], m[1][2], m[2][2], m[3][2],
                             m[0][3], m[1][3], m[2][3], m[3][3]);
}

void Forward_renderer::render_pass(const Material_collection &materials,
                                   const Model_collection    &models,
                                   const Light_collection    &lights,
                                   const Camera              &camera)
{
    if (models.size() == 0)
    {
        return;
    }

    auto &r = renderer();
    auto &t = r.track();

    update_camera(camera);
    update_models(models, camera);
    update_lights(lights, camera);
    update_materials(materials);

    bind_camera();

    gl::clear_color(0.0f, 0.0f, 0.0f, 1.0f);
    gl::clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    //gl::enable(GL_FRAMEBUFFER_SRGB);

    size_t light_index = 0;
    for (auto l : lights)
    {
        if (light_index == max_lights())
        {
            break;
        }

        if (light_index == 0)
        {
            t.execute(&m_first_pass_render_states);
        }
        else if (light_index == 1)
        {
            t.execute(&m_other_pass_render_states);
        }

        bind_light(light_index);

        switch (l->type)
        {
            case Light::Type::directional:
            {
                r.set_program(programs()->anisotropic_directional.get());
                break;
            }
            case Light::Type::spot:
            {
                r.set_program(programs()->anisotropic_spot.get());
                break;
            }
            default:
            {
                // Not implemented
                assert(0);
                ++light_index;
                continue;
            }
        }

        size_t model_index = 0;
        for (auto model : models)
        {
            auto   geometry_mesh  = model->geometry_mesh;
            auto   vertex_stream  = geometry_mesh->vertex_stream().get();
            auto   mesh           = geometry_mesh->get_mesh();
            auto   material       = model->material;
            size_t material_index = material->index;
            assert(vertex_stream);

            gl::begin_mode::value         begin_mode    = gl::begin_mode::triangles;
            Index_range const &           index_range   = geometry_mesh->fill_indices();
            GLsizei                       count         = static_cast<GLsizei>(index_range.index_count);
            gl::draw_elements_type::value index_type    = gl::draw_elements_type::unsigned_int;
            GLvoid *                      index_pointer = reinterpret_cast<GLvoid *>((index_range.first_index + mesh->first_index()) * mesh->index_buffer()->stride());
            GLint                         base_vertex   = configuration::can_use.draw_elements_base_vertex
                                    ? static_cast<GLint>(mesh->first_vertex())
                                    : 0;

            bind_model(model_index);
            bind_material(material_index);

            r.draw_elements_base_vertex(*vertex_stream,
                                        begin_mode,
                                        count,
                                        index_type,
                                        index_pointer,
                                        base_vertex);

            ++model_index;
        }

        ++light_index;
    }
    //gl::disable(GL_FRAMEBUFFER_SRGB);
}
