#ifndef buffer_hpp_renderstack_graphics
#define buffer_hpp_renderstack_graphics

#include "renderstack_graphics/configuration.hpp"
#include "renderstack_toolkit/gl.hpp"
#include "renderstack_toolkit/strong_gl_enums.hpp"
#include <cstddef>
#include <memory>
#include <vector>

namespace renderstack
{
namespace graphics
{

class Renderer;

size_t count_in_context();

class Buffer //: public ::std::enable_shared_from_this<Buffer>
{
public:
    enum class Target
    {
        array_buffer                            = 0, // 6.3   (bound VBO)
        pixel_pack_buffer                       = 1, // 6.16
        pixel_unpack_buffer                     = 2, // 6.16
        uniform_buffer                          = 3, // 6.25
        texture_buffer                          = 4, // TODO
        copy_read_buffer                        = 5, // 6.36
        copy_write_buffer                       = 6, // 6.36
        draw_indirect_buffer                    = 7, // TODO
        element_array_buffer                    = 8,
    };

    static bool is_valid(Buffer::Target target);

    static constexpr size_t non_indexed_context_buffer_target_count = 8;  // TODO Badly named
    static constexpr size_t all_buffer_target_count = 9;

    static const char *const desc(Target target);

    static gl::buffer_target::value gl_buffer_target(Target rs_target);

    Buffer() = default;

    Buffer(Target                       target,
           size_t                       capacity,
           size_t                       stride,
           gl::buffer_usage_hint::value usage = gl::buffer_usage_hint::static_draw);

    ~Buffer();

    void allocate(Target                       target,
                  size_t                       capacity,
                  size_t                       stride,
                  gl::buffer_usage_hint::value usage = gl::buffer_usage_hint::static_draw);

    void allocate_storage(Renderer &renderer);

    void set_debug_label(const std::string &value);

    const std::string &debug_label() const;

    size_t stride() const;

    size_t capacity() const;

    size_t allocate(size_t count);

    void *map(Renderer &renderer, size_t first, size_t count, gl::buffer_access_mask::value access);

    void unmap(Renderer &renderer);

    void flush(Renderer &renderer, size_t first, size_t count);

    void flush_and_unmap(Renderer &renderer, size_t count);

    size_t free_capacity() const;

    Target target() const
    {
        return m_target;
    }

#if defined(RENDERSTACK_GL_API_OPENGL)
    void dump() const;
#endif

    void bind_range(unsigned int binding_point, size_t offset, size_t size);

    friend class Renderer;
    friend class Vertex_array;
    friend class Texture;

private:
    unsigned int gl_name() const
    {
        return m_gl_name;
    }

private:
    unsigned int          m_gl_name{0};
    std::string           m_debug_label;
    Target                m_target;
    size_t                m_stride;
    size_t                m_capacity;
    size_t                m_next_free{0};
    gl::buffer_usage_hint::value m_usage;

    // Storage if not using MapBuffer
    std::vector<unsigned char> m_data_copy;

    // Last MapBuffer
    void *                        m_mapped_ptr;
    size_t                        m_mapped_offset;
    size_t                        m_mapped_size;
    gl::buffer_access_mask::value m_mapped_access;

    void validate();
};

} // namespace graphics
} // namespace renderstack

#endif
