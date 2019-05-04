#include "renderstack_toolkit/platform.hpp"

#include "renderstack_graphics/log.hpp"
#include "renderstack_graphics/renderer.hpp"
#include "renderstack_graphics/vertex_array.hpp"
#include "renderstack_graphics/vertex_format.hpp"

#include <algorithm>
#include <cassert>

#define LOG_CATEGORY &log_renderer

namespace renderstack
{
namespace graphics
{

using namespace std;
using namespace renderstack::toolkit;

Renderer::Renderer()
    : service("renderstack::graphics::Renderer"), m_cache_enabled(true)
{
    static int init_count = 0;

    assert(init_count == 0);
    ++init_count;

    for (size_t i = 0; i < m_mapped_buffer.size(); ++i)
    {
        m_mapped_buffer[i] = nullptr;
    }
}

Renderer::~Renderer()
{
}

void Renderer::initialize_service()
{
    (void)set_vertex_array(&m_default_vertex_array);
}

void Renderer::push()
{
    m_request_stack.push(m_requested);
}

void Renderer::pop()
{
    m_requested = m_request_stack.top();
    m_request_stack.pop();
}

void Renderer::trash()
{
    slog_trace("Renderer::trash()\n");

    gl::use_program(0);
    m_effective.current_program = nullptr;

    for (size_t i = 0; i < m_effective.buffer_binding.size(); ++i)
    {
        Buffer::Target target = static_cast<Buffer::Target>(i);
        gl::bind_buffer(Buffer::gl_buffer_target(target), 0);
        m_effective.buffer_binding[i] = nullptr;
    }

    gl::bind_vertex_array(0);
    m_effective.vertex_array_binding = nullptr;

    for (size_t i = 0; i < m_effective.texture_unit.size(); ++i)
    {
        gl::active_texture(GL_TEXTURE0 + i);
        m_effective.texture_unit[i].trash(i);
    }
    gl::active_texture(GL_TEXTURE0);
    m_effective.active_texture = 0;

    for (size_t i = 0; i < m_effective.uniform_buffer_binding_indexed.size(); ++i)
    {
        gl::bind_buffer_range(gl::buffer_target::uniform_buffer, i, 0, 0, 0);
        m_effective.uniform_buffer_binding_indexed[i] = nullptr;
    }
}

Program *Renderer::set_program(Program *program)
{
    if (!program)
    {
        log_error("Renderer::set_program() program is empty\n");
        throw runtime_error("Renderer::set_program() program is empty");
    }

    auto *old = m_effective.current_program;

    m_requested.current_program = program;
    if (!cache_enabled() || (m_effective.current_program != m_requested.current_program))
    {
        if (m_requested.current_program)
        {
            gl::use_program(program->gl_name());
        }
        else
        {
            gl::use_program(0);
        }

        m_effective.current_program = m_requested.current_program;
    }

    return old;
}

Texture *Renderer::set_texture(unsigned int unit,
                               Texture      *texture,
                               unsigned int *old_unit)
{
    assert(texture);

    auto target = texture->target();
    if (!Texture::is_valid(target))
    {
        throw runtime_error("invalid texture target");
    }

    unsigned int count = std::min(RS_TEXTURE_UNIT_COUNT, configuration::max_combined_texture_image_units);
    if (unit >= count)
    {
        throw runtime_error("texture unit index out of supported range");
    }

    auto old = m_effective.texture_unit[unit].texture_binding[static_cast<size_t>(target)];
    if (old_unit)
    {
        *old_unit = m_effective.active_texture;
    }

    size_t slot = static_cast<size_t>(target);
    auto &requested_texture = m_requested.texture_unit[unit].texture_binding[slot];
    auto &effective_texture = m_effective.texture_unit[unit].texture_binding[slot];

    requested_texture = texture;
    auto               old_texture = effective_texture;
    const std::string &old_label   = old_texture ? old_texture->debug_label() : "";

    slog_trace("set_texture(unit = %d, gl_name = %d %s) old_unit = %d, old_texture = %d %s\n",
               unit,
               texture->gl_name(),
               texture->debug_label().c_str(),
               m_effective.active_texture,
               (old_texture ? old_texture->gl_name() : 0),
               old_label.c_str());

    if (!cache_enabled() || (effective_texture != requested_texture))
    {
        m_requested.active_texture = unit;
        if (!cache_enabled() || (m_effective.active_texture != m_requested.active_texture))
        {
            gl::active_texture(gl::texture_unit::texture0 + m_requested.active_texture);
            m_effective.active_texture = m_requested.active_texture;
        }
        gl::bind_texture(Texture::gl_texture_target(target), texture->gl_name());

        effective_texture = requested_texture;

        // TODO Do this or not?
        texture->apply(*this, unit);
    }

    return old;
}

Texture *Renderer::reset_texture(unsigned int unit, Texture::Target target, unsigned int *old_unit)
{
    size_t slot = static_cast<size_t>(target);

    unsigned int count = std::min(RS_TEXTURE_UNIT_COUNT, configuration::max_combined_texture_image_units);
    if (unit >= count)
    {
        throw runtime_error("texture unit index out of supported range");
    }

    auto old = m_effective.texture_unit[unit].texture_binding[slot];

    slog_trace("reset_texture(unit = %d, target = %s) old_unit = %d\n",
               unit,
               Texture::desc(target),
               old_unit ? *old_unit : -1);

    if (!Texture::is_valid(target))
    {
        throw runtime_error("invalid texture target");
    }

    if (old_unit)
    {
        *old_unit = m_effective.active_texture;
    }

    m_requested.texture_unit[unit].texture_binding[slot] = nullptr;

    if (!cache_enabled() || (m_effective.texture_unit[unit].texture_binding[slot]))
    {
        m_requested.active_texture = unit;
        if (!cache_enabled() || (m_effective.active_texture != m_requested.active_texture))
        {
            gl::active_texture(gl::texture_unit::texture0 + m_requested.active_texture);
            m_effective.active_texture = m_requested.active_texture;
        }
        gl::bind_texture(Texture::gl_texture_target(target), 0);

        m_effective.texture_unit[unit].texture_binding[slot] = nullptr;
    }

    return old;
}

unsigned int Renderer::set_texture_unit(unsigned int unit)
{
    unsigned int old_unit = m_effective.active_texture;

    slog_trace("set_texture_unit(unit = %d) old_unit = %d", unit, old_unit);

    m_requested.active_texture = unit;
    if (!cache_enabled() || (m_effective.active_texture != m_requested.active_texture))
    {
        gl::active_texture(gl::texture_unit::texture0 + m_requested.active_texture);
        m_effective.active_texture = m_requested.active_texture;
    }
    return old_unit;
}

// Restores old texture to effective texture unit and then restores effective texture unit to old_unit
void Renderer::restore_texture(Texture::Target target, Texture *old_texture, unsigned int old_unit)
{
    size_t slot = static_cast<size_t>(target);
    unsigned int unit = m_effective.active_texture;

    slog_trace("restore_texture(target = %s, old_texture = %d %s, old_unit = %d) unit = %d\n",
               Texture::desc(target),
               (old_texture ? old_texture->gl_name() : 0),
               (old_texture ? old_texture->debug_label().c_str() : ""),
               old_unit);

    m_requested.texture_unit[unit].texture_binding[slot] = old_texture;
    if (!cache_enabled() ||
        (m_effective.texture_unit[unit].texture_binding[slot] !=
         m_requested.texture_unit[unit].texture_binding[slot]))
    {
        unsigned int gl_name = old_texture ? old_texture->gl_name() : 0;
        gl::bind_texture(Texture::gl_texture_target(target), gl_name);

        m_requested.active_texture = old_unit;
        if (!cache_enabled() || (m_effective.active_texture != m_requested.active_texture))
        {
            gl::active_texture(gl::texture_unit::texture0 + m_requested.active_texture);
            m_effective.active_texture = m_requested.active_texture;
        }
        m_effective.texture_unit[unit].texture_binding[slot] = m_requested.texture_unit[unit].texture_binding[slot];
    }
}

#if 0
#    if defined(RENDERSTACK_GL_API_OPENGL) || defined(RENDERSTACK_GL_API_OPENGL_ES_3)
Uniform_buffer_range *Renderer::set_uniform_buffer_range(
   unsigned int binding_point,
   Uniform_buffer_range *buffer_range,
   size_t offset
)
{
   unsigned int count = std::min(RS_UNIFORM_BINDING_POINT_COUNT, configuration::max_uniform_buffer_bindings);

   if (binding_point >= count)
      throw runtime_error("uniform buffer binding point index out of supported range");

   //if (!buffer_range)
   //   throw runtime_error("buffer range nullptr");

   if (!configuration::can_use.uniform_buffer_object)
      throw runtime_error("uniform buffer objects not supported by current context");

   Uniform_buffer_range *old = m_requested.uniform_buffer_binding_indexed[binding_point];

   m_requested.uniform_buffer_binding_indexed[binding_point] = buffer_range;

   auto buffer = buffer_range->uniform_buffer().lock();

   if (
      !cache_enabled() ||
      (
         (m_effective.uniform_buffer_binding_indexed[binding_point] != m_requested.uniform_buffer_binding_indexed[binding_point]) ||
         (m_effective.buffer_binding[Buffer::Target::uniform_buffer] != buffer)
      )
   )
   {
      if (m_requested.uniform_buffer_binding_indexed[binding_point])
      {
         // offset and size are assumed not to change once buffer_range is created
         gl::bind_buffer_range(
            gl::buffer_target::uniform_buffer,
            binding_point, 
            buffer_range->uniform_buffer().lock()->gl_name(),
            static_cast<GLsizeiptr>(buffer_range->first_byte()), 
            static_cast<GLsizeiptr>(buffer_range->> byte_count())
         );
      }
      else
      {
         gl::bind_buffer_range(
            gl::buffer_target::uniform_buffer,
            binding_point, 
            0,
            0, 
            0
         );
      }

      auto buffer = buffer_range->uniform_buffer().lock();
      m_effective.buffer_binding[Buffer::Target::uniform_buffer] = Buffer;
      m_effective.uniform_buffer_binding_indexed[binding_point] = m_requested.uniform_buffer_binding_indexed[binding_point];
   }

   return old;
}
#    endif
#endif

Buffer *Renderer::set_buffer(Buffer::Target target, Buffer *buffer)
{
    size_t slot = static_cast<size_t>(target);
    if (!Buffer::is_valid(target))
    {
        throw runtime_error("invalid buffer target");
    }

    auto old = m_effective.buffer_binding[slot];

    m_requested.buffer_binding[slot] = buffer;
    if (!cache_enabled() || (m_effective.buffer_binding[slot] != m_requested.buffer_binding[slot]))
    {
        if (m_requested.buffer_binding[slot])
        {
            unsigned int gl_name = buffer ? buffer->gl_name() : 0;
            gl::bind_buffer(Buffer::gl_buffer_target(target), gl_name);
        }

        m_effective.buffer_binding[slot] = m_requested.buffer_binding[slot];
    }

    return old;
}

Vertex_array *Renderer::set_vertex_array(Vertex_array *vertex_array)
{
    auto old = m_effective.vertex_array_binding;

    assert(vertex_array);

    m_requested.vertex_array_binding = vertex_array;
    if (!cache_enabled() || (m_effective.vertex_array_binding != m_requested.vertex_array_binding))
    {
        if (m_requested.vertex_array_binding)
        {
#if defined(RENDERSTACK_GL_API_OPENGL) || defined(RENDERSTACK_GL_API_OPENGL_ES_3)
            bool use_vao = configuration::must_use_vertex_array_object ||
                           (configuration::can_use.vertex_array_object &&
                            configuration::use_vertex_array_object);

            if (use_vao)
            {
                unsigned int gl_name = vertex_array ? vertex_array->gl_name() : 0;
                gl::bind_vertex_array(gl_name);

                // This helps us validate that VAO is really bound when we try to bind IBO
                if (m_effective.vertex_array_binding)
                {
                    m_effective.vertex_array_binding->set_bound(false);
                }
            }
            else
            {
                if (vertex_array->m_effective.element_array_buffer_binding)
                {
                    gl::bind_buffer(gl::buffer_target::element_array_buffer,
                                    vertex_array->m_effective.element_array_buffer_binding->gl_name());
                }
                else
                {
                    gl::bind_buffer(gl::buffer_target::element_array_buffer,
                                    0);
                }
            }
#endif
            vertex_array->set_bound(true);
        }

        m_effective.vertex_array_binding = m_requested.vertex_array_binding;
    }

    return old;
}

void Renderer::reset_vertex_array()
{
    set_vertex_array(&m_default_vertex_array);
}

void Renderer::setup_attribute_pointers(Vertex_stream &vertex_stream, GLint base_vertex)
{
    if (configuration::use_gl1)
    {
#if defined(RENDERSTACK_GL_API_OPENGL_WITH_LEGACY)
        {
            setup_attribute_pointers_old(basevertex);
        }
#else
        {
            throw runtime_error("GL1 path not supported");
        }
#endif
    }
    else
    {
        setup_attribute_pointers_new(vertex_stream, base_vertex);
    }
}

void Renderer::setup_attribute_pointers_new(Vertex_stream &vertex_stream, GLint base_vertex)
{
    slog_trace("Renderer::setup_attribute_pointers_new(base_vertex = %d\n",
               base_vertex);

    bool use_vao = configuration::must_use_vertex_array_object ||
                   (configuration::can_use.vertex_array_object &&
                    configuration::use_vertex_array_object);

    if (use_vao)
    {
        set_vertex_array(vertex_stream.vertex_array());
    }

    auto va = m_effective.vertex_array_binding;

    va->clear_enabled_attribs();

    unsigned int count = std::min(RS_ATTRIBUTE_COUNT, configuration::max_vertex_attribs);

    for (auto &binding : vertex_stream.bindings())
    {
        auto vbo       = binding.vertex_buffer;
        auto attribute = binding.vertex_attribute;
        auto mapping   = binding.vertex_attribute_mapping;

        assert(vbo);
        assert(attribute);

        if (mapping.dst_index >= count)
        {
            throw runtime_error("vertex attribute index out of supported range");
        }

        set_buffer(Buffer::Target::array_buffer, vbo);

        va->enable_attrib(mapping.dst_index,
                          attribute->dimension,
                          attribute->shader_type,
                          attribute->data_type,
                          binding.stride,
                          base_vertex * binding.stride + attribute->offset,
                          attribute->normalized,
                          attribute->divisor);
    }

    va->apply_attrib_enables();
}

void Renderer::use_vertex_stream(Vertex_stream &vertex_stream)
{
    set_vertex_array(vertex_stream.vertex_array());
}

void Renderer::draw_elements_base_vertex(Vertex_stream &vertex_stream,
    GLenum begin_mode, GLsizei count, GLenum index_type, const GLvoid *indices,
    GLint base_vertex)
{
    bool use_vao = configuration::must_use_vertex_array_object ||
                   (configuration::can_use.vertex_array_object &&
                    configuration::use_vertex_array_object);

    set_vertex_array(vertex_stream.vertex_array());

    if (use_vao)
    {
#if defined(RENDERSTACK_GL_API_OPENGL)
        if (configuration::can_use.draw_elements_base_vertex)
        {
            gl::draw_elements_base_vertex(begin_mode, count, index_type, indices, base_vertex);
        }
        else
#endif
        {
            gl::draw_elements(begin_mode, count, index_type, indices);
        }
    }
    else
    {
        setup_attribute_pointers(vertex_stream, base_vertex);
        gl::draw_elements(begin_mode, count, index_type, indices);
    }
}

bool Renderer::map_buffer(Buffer::Target target, Buffer *buffer)
{
    size_t slot = static_cast<size_t>(target);
    assert(Buffer::is_valid(target));
    bool binding_ok = false;

    if (target == Buffer::Target::element_array_buffer)
    {
        binding_ok = m_effective.vertex_array_binding->index_buffer() == buffer;
    }
    else
    {
        binding_ok = m_effective.buffer_binding[slot] == buffer;
    }

    bool not_mapped   = m_mapped_buffer[slot] == nullptr;
    bool target_match = buffer->target() == target;
    bool map_ok       = not_mapped && target_match;
    bool all_ok       = binding_ok && map_ok;

    if (all_ok)
    {
        m_mapped_buffer[slot] = buffer;
    }

    return all_ok;
}

bool Renderer::buffer_is_bound(Buffer::Target target, Buffer *buffer)
{
    size_t slot = static_cast<size_t>(target);
    assert(Buffer::is_valid(target));
    bool ok = (m_effective.buffer_binding[slot] == buffer) && (buffer->target() == target);
    return ok;
}

bool Renderer::buffer_is_mapped(Buffer::Target target, Buffer *buffer)
{
    size_t slot = static_cast<size_t>(target);
    assert(Buffer::is_valid(target));
    bool ok = (m_mapped_buffer[slot] == buffer) && (buffer->target() == target);
    return ok;
}

bool Renderer::unmap_buffer(Buffer::Target target, Buffer *buffer)
{
    size_t slot = static_cast<size_t>(target);
    assert(Buffer::is_valid(target));
    bool ok = (m_mapped_buffer[slot] == buffer) && (buffer->target() == target);
    m_mapped_buffer[slot] = nullptr;
    return ok;
}

bool Renderer::texture_is_bound(unsigned int unit, Texture::Target target, Texture *texture)
{
    size_t slot = static_cast<size_t>(target);
    unsigned int count = std::min(RS_TEXTURE_UNIT_COUNT, configuration::max_combined_texture_image_units);
    if (unit >= count)
    {
        throw runtime_error("texture unit index out of supported range");
    }

    assert(Texture::is_valid(target));

    bool ok = (m_effective.texture_unit[unit].texture_binding[slot] == texture);
    return ok;
}

unsigned int Renderer::effective_texture_unit() const
{
    return m_effective.active_texture;
}

} // namespace graphics
} // namespace renderstack

#if defined(RENDERSTACK_GL_API_OPENGL_WITH_LEGACY)
// This path is outdated
void Vertex_stream::setup_attribute_pointers_old(GLint basevertex)
{
    for (auto i = m_vertex_stream_bindings.begin(); i != m_vertex_stream_bindings.end(); ++i)
    {
        Vertex_stream_binding *binding   = *i;
        Vertex_attribute *     attribute = binding->vertex_attribute();
        vertex_stream_mapping *mapping   = binding->vertex_stream_mapping();

        switch (mapping->dst_usage())
        {
            case vertex_attribute_usage::position:
            {
                gl::enable_client_state(gl::array_cap::vertex_array);
                gl::vertex_pointer(
                    attribute->dimension(),
                    attribute->type(),
                    binding->stride(),
                    reinterpret_cast<char *>(basevertex * binding->stride() + attribute->offset()));
            }
            break;
            case vertex_attribute_usage::normal:
            {
                gl::enable_client_state(gl::array_cap::normal_array);
                gl::normal_pointer(
                    attribute->type(),
                    binding->stride(),
                    reinterpret_cast<char *>(basevertex * binding->stride() + attribute->offset()));
            }
            break;
            case vertex_attribute_usage::color:
            {
                if ((attribute->dimension() != 3) && (attribute->dimension() != 4))
                {
                    throw std::exception();
                    continue;
                }
                gl::enable_client_state(gl::array_cap::color_array);
                gl::color_pointer(
                    attribute->dimension(),
                    attribute->type(),
                    binding->stride(),
                    reinterpret_cast<char *>(basevertex * binding->stride() + attribute->offset()));
            }
            break;
            case vertex_attribute_usage::tex_coord:
            {
                gl::client_active_texture(gl::texture_unit::texture0 + binding->vertex_stream_mapping()->dst_index());
                gl::enable_client_state(gl::array_cap::texture_coord_array);
                gl::texcoord_pointer(
                    attribute->dimension(),
                    attribute->type(),
                    binding->stride(),
                    reinterpret_cast<char *>(basevertex * binding->stride() + attribute->offset()));
            }
            break;
            default:
                break;
        }
    }
}
void Vertex_stream::disable_attributes_old()
{
    for (auto i = vertex_stream_bindings().begin(); i != vertex_stream_bindings().end(); ++i)
    {
        Vertex_stream_binding *binding   = *i;
        Vertex_attribute *     attribute = binding->vertex_attribute();
        vertex_stream_mapping *mapping   = binding->vertex_stream_mapping();

        switch (mapping->dst_usage())
        {
            case vertex_attribute_usage::position:
                gl::disable_client_state(gl::array_cap::vertex_array);
                break;
            case vertex_attribute_usage::normal:
                gl::disable_client_state(gl::array_cap::normal_array);
                break;
            case vertex_attribute_usage::color:
                gl::disable_client_state(gl::array_cap::color_array);
                break;
            case vertex_attribute_usage::tex_coord:
                gl::client_active_texture(gl::texture_unit::texture0 + binding->vertex_stream_mapping()->dst_index());
                gl::disable_client_state(gl::array_cap::texture_coord_array);
                break;
            default:
                break;
        }
    }
}
#endif
