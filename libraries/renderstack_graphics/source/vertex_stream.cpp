#include "renderstack_graphics/vertex_stream.hpp"
#include "renderstack_graphics/buffer.hpp"
#include "renderstack_graphics/configuration.hpp"
#include "renderstack_graphics/log.hpp"
#include "renderstack_graphics/vertex_attribute.hpp"
#include "renderstack_graphics/vertex_attribute_mapping.hpp"
#include "renderstack_graphics/vertex_stream_binding.hpp"
#include "renderstack_toolkit/gl.hpp"
#include "renderstack_toolkit/platform.hpp"
#include "renderstack_toolkit/strong_gl_enums.hpp"
#include <memory>
#include <cassert>
#include <stdexcept>

#define LOG_CATEGORY &log_vertex_stream

namespace renderstack
{
namespace graphics
{

Vertex_stream::Vertex_stream()
{
    m_vertex_array = std::make_unique<Vertex_array>();
}

Vertex_stream_binding &Vertex_stream::add(
    Buffer                         *vertex_buffer,
    const Vertex_attribute_mapping &mapping,
    const Vertex_attribute         *attribute,
    size_t                         stride)
{
    m_vertex_stream_bindings.emplace_back(vertex_buffer, mapping, attribute, stride);
    return m_vertex_stream_bindings.back();
}

void Vertex_stream::clear()
{
    m_vertex_stream_bindings.clear();
}

} // namespace graphics
} // namespace renderstack
