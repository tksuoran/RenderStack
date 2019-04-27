#ifndef mesh_hpp_renderstack_mesh
#define mesh_hpp_renderstack_mesh

#include "renderstack_graphics/buffer.hpp"
#include "renderstack_toolkit/gl.hpp"
#include "renderstack_toolkit/platform.hpp"
#include "renderstack_toolkit/strong_gl_enums.hpp"
#include <memory>
#include <stdexcept>

namespace renderstack
{
namespace mesh
{

struct Index_range;

class mesh
{
public:
    mesh() = default;

    ~mesh() = default;

    std::shared_ptr<renderstack::graphics::Buffer> vertex_buffer() const;

    std::shared_ptr<renderstack::graphics::Buffer> index_buffer() const;

    size_t first_vertex() const;

    size_t first_index() const;

    size_t vertex_count() const;

    size_t index_count() const;

    void allocate_vertex_buffer(renderstack::graphics::Renderer &renderer, size_t vertex_stride,
                                size_t vertex_count, gl::buffer_usage_hint::value usage = gl::buffer_usage_hint::static_draw);

    void allocate_vertex_buffer(std::shared_ptr<renderstack::graphics::Buffer> vertex_buffer, size_t vertex_count);

    void allocate_index_buffer(renderstack::graphics::Renderer &renderer, size_t index_stride, size_t index_count,
                               gl::buffer_usage_hint::value usage = gl::buffer_usage_hint::static_draw);

    void allocate_index_buffer(std::shared_ptr<renderstack::graphics::Buffer> index_buffer, size_t index_count);

    void allocate_index_range(gl::begin_mode::value begin_mode, size_t index_count, Index_range &range);

    void set_first_vertex(size_t value);

    void set_first_index(size_t value);

    void set_vertex_count(size_t value);

    void set_index_count(size_t value);

private:
    std::shared_ptr<renderstack::graphics::Buffer> m_vertex_buffer;
    std::shared_ptr<renderstack::graphics::Buffer> m_index_buffer;

    //  This is the container for index ranges
    size_t m_first_vertex{0U};
    size_t m_first_index{0U};
    size_t m_vertex_count{0U};
    size_t m_index_count{0U};
    size_t m_next_index_range_start{0U};
};

} // namespace mesh
} // namespace renderstack

#endif
