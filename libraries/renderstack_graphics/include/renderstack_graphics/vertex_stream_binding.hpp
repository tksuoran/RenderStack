#ifndef vertex_stream_binding_hpp_renderstack_graphics
#define vertex_stream_binding_hpp_renderstack_graphics

#include "renderstack_graphics/vertex_attribute_mapping.hpp"
#include "renderstack_toolkit/platform.hpp"
#include "renderstack_toolkit/strong_gl_enums.hpp"
#include <map>
#include <memory>
#include <string>
#include <vector>

namespace renderstack
{
namespace graphics
{

class Buffer;
class Vertex_attribute;
class Vertex_attribute_mapping;

class Vertex_stream_binding
{
public:
    Vertex_stream_binding(Buffer                         *vertex_buffer,
                          const Vertex_attribute_mapping &mapping,
                          const Vertex_attribute         *attribute,
                          size_t                         stride)
        : vertex_buffer(vertex_buffer)
        , vertex_attribute_mapping(mapping)
        , vertex_attribute(attribute)
        , stride(stride)
    {}

    Vertex_stream_binding(const Vertex_stream_binding &other)
        : vertex_buffer(other.vertex_buffer)
        , vertex_attribute_mapping(other.vertex_attribute_mapping)
        , vertex_attribute(other.vertex_attribute)
        , stride(other.stride)
    {}

    Vertex_stream_binding &operator=(const Vertex_stream_binding &other) = delete;

    Buffer                         *vertex_buffer{nullptr};
    const Vertex_attribute_mapping &vertex_attribute_mapping;
    const Vertex_attribute         *vertex_attribute{nullptr};
    size_t                          stride{0U};
};

} // namespace graphics
} // namespace renderstack

#endif
