#include "renderers/quad_renderer.hpp"
#include "main/programs.hpp"
#include "renderstack_graphics/buffer.hpp"
#include "renderstack_graphics/configuration.hpp"
#include "renderstack_graphics/renderer.hpp"
#include "renderstack_graphics/vertex_format.hpp"
#include "renderstack_toolkit/math_util.hpp"
#include "renderstack_toolkit/platform.hpp"
#include "renderstack_toolkit/strong_gl_enums.hpp"
#include <cassert>

using namespace std;
using namespace gl;
using namespace glm;
using namespace renderstack::graphics;

Quad_renderer::Quad_renderer()
    : service("Quad_renderer")
{
}

void Quad_renderer::connect(shared_ptr<renderstack::graphics::Renderer> renderer)
{
    m_renderer = renderer;

    initialization_depends_on(renderer);
}

void Quad_renderer::initialize_service()
{
    assert(m_renderer);

    // Specify vertex stream attribute mapping
    m_vertex_attribute_mappings.add("a_position_texcoord",
                                    static_cast<vertex_attribute_usage::value>(
                                        vertex_attribute_usage::position | vertex_attribute_usage::tex_coord),
                                    0,
                                    0);

    // Specify vertex format (single attribute with X, Y, U, V)
    m_vertex_format.make_attribute(static_cast<vertex_attribute_usage::value>(
                                       vertex_attribute_usage::position | vertex_attribute_usage::tex_coord),
                                   gl::vertex_attrib_pointer_type::float_,
                                   gl::vertex_attrib_pointer_type::float_,
                                   0,
                                   4,
                                   false);

    // Create vertex buffer with 4 vertices
    m_vertex_buffer = make_shared<renderstack::graphics::Buffer>(
        renderstack::graphics::Buffer::Target::array_buffer,
        8,
        m_vertex_format.stride());
    m_vertex_buffer->allocate_storage(*m_renderer);

    // Create vertex stream
    auto va         = m_vertex_stream.vertex_array();
    auto old_va     = m_renderer->set_vertex_array(va);
    m_vertex_attribute_mappings.add_to_vertex_stream(m_vertex_stream, m_vertex_buffer.get(), m_vertex_format);
    m_renderer->setup_attribute_pointers(m_vertex_stream, 0);

    // Create vertex buffer
    auto   old_vbo = m_renderer->set_buffer(renderstack::graphics::Buffer::Target::array_buffer, m_vertex_buffer.get());
    float *ptr     = static_cast<float *>(
        m_vertex_buffer->map(
            *m_renderer,
            0,
            8,
            (gl::buffer_access_mask::value)(
                gl::buffer_access_mask::map_write_bit |
                gl::buffer_access_mask::map_invalidate_range_bit)));

    //         X               Y              U              V
    *ptr++ = -1.0f; *ptr++ = -1.0f; *ptr++ = 0.0f; *ptr++ = 0.0f;
    *ptr++ =  1.0f; *ptr++ = -1.0f; *ptr++ = 1.0f; *ptr++ = 0.0f;
    *ptr++ =  1.0f; *ptr++ =  1.0f; *ptr++ = 1.0f; *ptr++ = 1.0f;
    *ptr++ = -1.0f; *ptr++ =  1.0f; *ptr++ = 0.0f; *ptr++ = 1.0f;

    //         X               Y              U              V
    *ptr++ = 0.0f; *ptr++ = 0.0f; *ptr++ = 0.0f; *ptr++ = 0.0f;
    *ptr++ = 1.0f; *ptr++ = 0.0f; *ptr++ = 1.0f; *ptr++ = 0.0f;
    *ptr++ = 1.0f; *ptr++ = 1.0f; *ptr++ = 1.0f; *ptr++ = 1.0f;
    *ptr++ = 0.0f; *ptr++ = 1.0f; *ptr++ = 0.0f; *ptr++ = 1.0f;

    m_vertex_buffer->unmap(*m_renderer);

    (void)m_renderer->set_vertex_array(old_va);
    (void)m_renderer->set_buffer(renderstack::graphics::Buffer::Target::array_buffer, old_vbo);
}

void Quad_renderer::render_minus_one_to_one()
{
    m_renderer->use_vertex_stream(m_vertex_stream);
    gl::draw_arrays(gl::begin_mode::triangle_fan, 0, 4);
}

void Quad_renderer::render_zero_to_one()
{
    m_renderer->use_vertex_stream(m_vertex_stream);
    gl::draw_arrays(gl::begin_mode::triangle_fan, 4, 4);
}
