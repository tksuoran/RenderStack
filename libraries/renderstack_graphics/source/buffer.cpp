#include "renderstack_graphics/buffer.hpp"
#include "renderstack_graphics/configuration.hpp"
#include "renderstack_graphics/log.hpp"
#include "renderstack_graphics/renderer.hpp"
#include "renderstack_toolkit/gl.hpp"
#include "renderstack_toolkit/platform.hpp"
#include <cassert>
#include <cstdint>
#include <cstring>
#include <stdexcept>

#define LOG_CATEGORY &log_buffer

namespace renderstack
{
namespace graphics
{

using namespace renderstack::toolkit;
using namespace gl;
using namespace std;

#define GUARD_BYTES 32
#define DEBUG_BUFFER 1

bool Buffer::is_valid(Buffer::Target target)
{
    switch (target)
    {
        case Target::array_buffer:
        {
            return configuration::can_use.vertex_array_object;
        }

        case Target::pixel_pack_buffer:
        case Target::pixel_unpack_buffer:
        {
            return configuration::can_use.pixel_buffer_object;
        }

        case Target::uniform_buffer:
        {
            return configuration::can_use.uniform_buffer_object;
        }

        case Target::texture_buffer:
        {
            return configuration::can_use.texture_buffer_object;
        }

        case Target::copy_read_buffer:
        case Target::copy_write_buffer:
        {
            return true; // TODO configuration
        }

        case Target::draw_indirect_buffer:
        {
            return true; // TODO configuration
        }

        case Target::element_array_buffer:
        {
            return true;
        }

        default:
        {
            return false;
        }
    }
}

gl::buffer_target::value Buffer::gl_buffer_target(Buffer::Target rs_target)
{
    switch (rs_target)
    {
        case Buffer::Target::array_buffer: return gl::buffer_target::array_buffer;
        case Buffer::Target::element_array_buffer: return gl::buffer_target::element_array_buffer;
        case Buffer::Target::pixel_pack_buffer: return gl::buffer_target::pixel_pack_buffer;
        case Buffer::Target::pixel_unpack_buffer: return gl::buffer_target::pixel_unpack_buffer;
        case Buffer::Target::copy_read_buffer: return gl::buffer_target::copy_read_buffer;
        case Buffer::Target::copy_write_buffer: return gl::buffer_target::copy_write_buffer;

        case Buffer::Target::uniform_buffer:
        {
            if (!configuration::can_use.uniform_buffer_object)
            {
                throw runtime_error("uniform buffer not supported");
            }
            return gl::buffer_target::uniform_buffer;
        }

        case Buffer::Target::texture_buffer:
        {
            if (!configuration::can_use.texture_buffer_object)
            {
                throw runtime_error("texture buffer not supported");
            }
            return gl::buffer_target::texture_buffer;
        }

        case Buffer::Target::draw_indirect_buffer:
        {
            // TODO can use
            return gl::buffer_target::draw_indirect_buffer;
        }

        default:
        {
            throw runtime_error("invalid buffer target");
        }
    }
}

const char *const Buffer::desc(Buffer::Target target)
{
    switch (target)
    {
        case Buffer::Target::array_buffer: return "array_buffer";
        case Buffer::Target::element_array_buffer: return "element_array_buffer";
        case Buffer::Target::pixel_pack_buffer: return "pixel_pack_buffer";
        case Buffer::Target::pixel_unpack_buffer: return "pixel_unpack_buffer";
        case Buffer::Target::copy_read_buffer: return "copy_read_buffer";
        case Buffer::Target::copy_write_buffer: return "copy_write_buffer";
        case Buffer::Target::uniform_buffer: return "uniform_buffer";
        case Buffer::Target::texture_buffer: return "texture_buffer";
        case Buffer::Target::draw_indirect_buffer: return "draw_indirect_buffer";

        default:
            throw runtime_error("invalid buffer target");
    }
}

Buffer::Buffer(Buffer::Target               target,
               size_t                       capacity,
               size_t                       stride,
               gl::buffer_usage_hint::value usage)
    : m_target(target)
    , m_stride(stride)
    , m_capacity(capacity)
    , m_usage(usage)
{
    log_trace("Buffer::Buffer(target = %s, capacity = %u, stride = %u, usage = %s) name = %u\n",
              desc(target),
              static_cast<unsigned int>(capacity),
              static_cast<unsigned int>(stride),
              gl::enum_string(usage),
              m_gl_name);
}

void Buffer::allocate(Buffer::Target               target,
                      size_t                       capacity,
                      size_t                       stride,
                      gl::buffer_usage_hint::value usage)
{
    m_target = target;
    m_stride = stride;
    m_capacity = capacity;
    m_usage = usage;

    log_trace("Buffer::Buffer(target = %s, capacity = %u, stride = %u, usage = %s) name = %u\n",
              desc(target),
              static_cast<unsigned int>(capacity),
              static_cast<unsigned int>(stride),
              gl::enum_string(usage),
              m_gl_name);
}

void Buffer::allocate_storage(Renderer &renderer)
{
    Buffer *old;

    gl::gen_buffers(1, &m_gl_name);

    if (m_target == Buffer::Target::element_array_buffer)
    {
        auto va = renderer.vertex_array();
        assert(va);
        old = va->set_index_buffer(this);
    }
    else
    {
        old = renderer.set_buffer(m_target, this);
    }

    log_trace("Buffer::allocate_storage() target = %s, name = %u\n",
              desc(m_target),
              m_gl_name);

    buffer_data(gl_buffer_target(m_target),
                static_cast<GLintptr>(m_capacity * m_stride),
                nullptr,
                m_usage);

    if (!configuration::can_use.map_buffer_range)
    {
        size_t new_size = GUARD_BYTES + (m_capacity * m_stride) + GUARD_BYTES;

        try
        {
            m_data_copy.resize(new_size);
        }
        catch (...)
        {
            // Most likely we run out of memory :(
            throw runtime_error("memory error in Buffer::Buffer()");
        }

        ::memset(&m_data_copy[0], 0xcd, new_size);
    }

    if (renderstack::graphics::configuration::use_vertex_array_object)
    {
        if (m_target == Buffer::Target::element_array_buffer)
        {
            auto va = renderer.vertex_array();
            (void)va->set_index_buffer(old);
        }
        else
        {
            (void)renderer.set_buffer(m_target, old);
        }
    }
}

void Buffer::set_debug_label(const std::string &value)
{
    m_debug_label = value;
}

const std::string &Buffer::debug_label() const
{
    return m_debug_label;
}

void Buffer::validate()
{
    if (!configuration::can_use.map_buffer_range)
    {
        for (int i = 0; i < GUARD_BYTES; ++i)
        {
            if (m_data_copy[i] != 0xcd)
            {
                throw runtime_error("Buffer memory corruption");
            }

            if (m_data_copy[(m_capacity * m_stride) + GUARD_BYTES + i] != 0xcd)
            {
                throw runtime_error("Buffer memory corruption");
            }
        }
    }
}

Buffer::~Buffer()
{
    delete_buffers(1, &m_gl_name);

    validate();
}

size_t Buffer::allocate(size_t count)
{
    size_t first = m_next_free;
    m_next_free += count;
    if (m_next_free > m_capacity)
    {
        throw runtime_error("out of Buffer capacity");
    }

    return first;
}

void *Buffer::map(Renderer &renderer, size_t first, size_t count, gl::buffer_access_mask::value access)
{
    (void)renderer; // avoid unused warnings in release builds
    assert(count > 0);

    slog_trace("Buffer::map(target = %s first = %u count = %u access = 0x%x) name = %u\n",
               desc(m_target),
               static_cast<unsigned int>(first),
               static_cast<unsigned int>(count),
               access,
               m_gl_name);

    if (!m_gl_name)
    {
        throw runtime_error("storage not allocated");
    }

    // Note, this is in multiples of stride
    if (first + count > m_capacity)
    {
        throw runtime_error("first + count > m_capacity");
    }

    assert(m_gl_name);
    assert(renderer.map_buffer(m_target, this));

    m_mapped_offset = static_cast<GLsizeiptr>(first * m_stride);
    m_mapped_size   = static_cast<GLsizeiptr>(count * m_stride);
    m_mapped_access = access;

#if defined(RENDERSTACK_GL_API_OPENGL) || defined(RENDERSTACK_GL_API_OPENGL_ES_3)
    if (configuration::can_use.map_buffer_range)
    {
        m_mapped_ptr = gl::map_buffer_range(gl_buffer_target(m_target),
                                            m_mapped_offset,
                                            m_mapped_size,
                                            access);
        assert(m_mapped_ptr);
    }
    else
#endif
    {
        // Access data copy
        uint8_t *start = &m_data_copy[GUARD_BYTES];
        uint8_t *ptr   = &start[m_mapped_offset];
        m_mapped_ptr   = reinterpret_cast<void *>(ptr);

#if defined(_DEBUG)
        validate();

#    if 0
      bool invalidate_range   = (access & gl::buffer_access_mask::map_invalidate_range_bit ) == gl::buffer_access_mask::map_invalidate_range_bit;
      bool invalidate_buffer  = (access & gl::buffer_access_mask::map_invalidate_buffer_bit) == gl::buffer_access_mask::map_invalidate_buffer_bit;

      if (invalidate_range)
         ::memset(ptr, 0xff, count * m_stride);
      if (invalidate_buffer)
         ::memset(start, 0xff, m_capacity * m_stride);
#    endif
#endif
    }

    log_trace(":m_mapped_offset = %u m_mapped_size = %u m_mapped_ptr = %p)\n",
              static_cast<unsigned int>(m_mapped_offset),
              static_cast<unsigned int>(m_mapped_size),
              m_mapped_ptr);

    return m_mapped_ptr;
}

void Buffer::unmap(Renderer &renderer)
{
    (void)renderer; // avoid unused warnings in release builds
    slog_trace("Buffer::unmap(target = %s offset = %u size = %u ptr = %p) name = %u\n",
               desc(m_target),
               static_cast<unsigned int>(m_mapped_offset),
               static_cast<unsigned int>(m_mapped_size),
               m_mapped_ptr,
               m_gl_name);
    set_text_color(C_GREY);

    assert(m_gl_name);
    assert(renderer.unmap_buffer(m_target, this));

#if defined(RENDERSTACK_GL_API_OPENGL) || defined(RENDERSTACK_GL_API_OPENGL_ES_3)
    if (configuration::can_use.map_buffer_range)
    {
        GLboolean res = gl::unmap_buffer(gl_buffer_target(m_target));
        (void)res;
        assert(res == GL_TRUE);
    }
    else
#endif
    {
        bool flush_explicit = (m_mapped_access & gl::buffer_access_mask::map_flush_explicit_bit) == gl::buffer_access_mask::map_flush_explicit_bit;

#if defined(_DEBUG)
        validate();
#endif

        // If flush explicit is not selected, unmap will flush full mapped range
        if (!flush_explicit)
        {
            uint8_t *start = &m_data_copy[GUARD_BYTES];
            uint8_t *ptr   = &start[m_mapped_offset];

            if (m_mapped_ptr != reinterpret_cast<void *>(ptr))
            {
                throw runtime_error("m_mapped_ptr != reinterpret_cast<void*>(ptr)");
            }

            gl::buffer_sub_data(gl_buffer_target(m_target),
                                m_mapped_offset,
                                m_mapped_size,
                                m_mapped_ptr);
        }
    }
    m_mapped_offset = size_t(~0);
    m_mapped_size   = 0;
    m_mapped_access = gl::buffer_access_mask::value(0);
}

void Buffer::flush(Renderer &renderer, size_t first, size_t count)
{
    // unmap will do flush
    size_t offset = first * m_stride;
    size_t size   = count * m_stride;

    bool flush_explicit = (m_mapped_access & gl::buffer_access_mask::map_flush_explicit_bit) == gl::buffer_access_mask::map_flush_explicit_bit;
    assert(flush_explicit);

    if (first + count > m_capacity)
    {
        throw runtime_error("first + count > m_capacity");
    }

    assert(m_gl_name);
    assert(renderer.buffer_is_mapped(m_target, this));
    (void)renderer;

#if defined(RENDERSTACK_GL_API_OPENGL) || defined(RENDERSTACK_GL_API_OPENGL_ES_3)
    if (configuration::can_use.map_buffer_range)
    {
        slog_trace("Buffer::flush(target = %s first = %u count = %u offset = %u size = %u) m_mapped_ptr = %p name = %u\n",
                   desc(m_target),
                   static_cast<unsigned int>(first),
                   static_cast<unsigned int>(count),
                   static_cast<unsigned int>(offset),
                   static_cast<unsigned int>(size),
                   m_mapped_ptr,
                   m_gl_name);

        gl::flush_mapped_buffer_range(gl_buffer_target(m_target),
                                      static_cast<GLintptr>(offset),
                                      static_cast<GLsizeiptr>(size));
    }
    else
#endif
    {
        uint8_t *start = &m_data_copy[GUARD_BYTES];
        uint8_t *ptr   = &start[m_mapped_offset + offset];

        slog_trace("Buffer::flush(target = %s first = %u count = %u offset = %u size = %u ptr = %p) name = %u\n",
                   desc(m_target),
                   static_cast<unsigned int>(first),
                   static_cast<unsigned int>(count),
                   static_cast<unsigned int>(offset),
                   static_cast<unsigned int>(size),
                   ptr,
                   m_gl_name);

        validate();

        if (size > m_mapped_size)
        {
            throw runtime_error("size > m_mapped_size");
        }

        gl::buffer_sub_data(gl_buffer_target(m_target),
                            m_mapped_offset + offset,
                            size,
                            reinterpret_cast<const GLvoid *>(ptr));
    }
}
#if defined(RENDERSTACK_GL_API_OPENGL)
void Buffer::dump() const
{
    bool      allocated = false;
    bool      unmap     = false;
    size_t    count     = ((m_capacity * m_stride) / 4);
    GLint     mapped    = 0;
    uint32_t *data      = nullptr;

    gl::get_buffer_parameter_iv(gl_buffer_target(m_target), GL_BUFFER_MAPPED, &mapped);

    if (mapped == GL_FALSE)
    {
        data = reinterpret_cast<uint32_t *>(
            gl::map_buffer_range(gl_buffer_target(m_target),
                                 0,
                                 m_capacity * m_stride,
                                 gl::buffer_access_mask::map_read_bit));
        unmap = data != nullptr;
    }

    if (data == nullptr)
    {
        // This happens if we already had buffer mapped
        data = new uint32_t[count + 1];

        gl::get_buffer_sub_data(gl_buffer_target(m_target), 0, count * 4, data);

        allocated = true;
    }

    for (size_t i = 0; i < count; ++i)
    {
        if (i % 16 == 0)
        {
            printf("%08x: ", static_cast<unsigned int>(i));
        }

        printf("%08x ", data[i]);

        if (i % 16 == 15)
        {
            printf("\n");
        }
    }
    printf("\n");

    if (unmap)
    {
        gl::unmap_buffer(gl_buffer_target(m_target));
    }

    if (allocated)
    {
        delete[] data;
    }
}
#endif

void Buffer::flush_and_unmap(Renderer &renderer, size_t count)
{
    bool flush_explicit = (m_mapped_access & gl::buffer_access_mask::map_flush_explicit_bit) == gl::buffer_access_mask::map_flush_explicit_bit;

    assert(m_gl_name);
    assert(renderer.buffer_is_mapped(m_target, this));

#if defined(_DEBUG)
    validate();
#endif

    slog_trace("flush_and_unmap(count = %u) name = %u\n",
               static_cast<unsigned int>(count), m_gl_name);

    //  If explicit is not selected, unmap will do full flush
    //  so we do manual flush only if explicit is selected
    if (flush_explicit)
    {
        flush(renderer, 0, count);
    }

    unmap(renderer);
}

size_t Buffer::free_capacity() const
{
    return m_capacity - m_next_free;
}

size_t Buffer::stride() const
{
    return m_stride;
}

size_t Buffer::capacity() const
{
    return m_capacity;
}

void Buffer::bind_range(unsigned int binding_point, size_t first, size_t size)
{
    gl::bind_buffer_range(gl_buffer_target(m_target),
                          binding_point,
                          m_gl_name,
                          first * m_stride,
                          size * m_stride);
}

} // namespace graphics
} // namespace renderstack
