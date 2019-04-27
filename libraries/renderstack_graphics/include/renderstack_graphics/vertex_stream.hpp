#ifndef vertex_stream_hpp_renderstack_graphics
#define vertex_stream_hpp_renderstack_graphics

#include "renderstack_graphics/vertex_array.hpp"
#include "renderstack_graphics/vertex_attribute.hpp"
#include "renderstack_graphics/vertex_stream_binding.hpp"
#include "renderstack_toolkit/gl.hpp"
#include "renderstack_toolkit/strong_gl_enums.hpp"
#include <cstddef>
#include <memory>
#include <vector>

namespace renderstack
{
namespace graphics
{

class Buffer;
class Renderer;
class Vertex_attribute_mapping;
class Vertex_array;

class Vertex_stream : public std::enable_shared_from_this<Vertex_stream>
{
public:
    Vertex_stream();

    ~Vertex_stream() = default;

    Vertex_stream_binding &add(Buffer                         *vertex_buffer,
                               const Vertex_attribute_mapping &vertex_attribute_mapping,
                               const Vertex_attribute         *attribute,
                               size_t                         stride);
 
    void clear();

    size_t count() const
    {
        return m_vertex_stream_bindings.size();
    }

    Vertex_array *vertex_array()
    {
        return m_vertex_array.get();
    }

    std::vector<Vertex_stream_binding> &bindings()
    {
        return m_vertex_stream_bindings;
    }

    std::vector<Vertex_stream_binding> const &bindings() const
    {
        return m_vertex_stream_bindings;
    }

private:
    std::vector<Vertex_stream_binding> m_vertex_stream_bindings;
    std::unique_ptr<Vertex_array> m_vertex_array;
};

} // namespace graphics
} // namespace renderstack

#endif
