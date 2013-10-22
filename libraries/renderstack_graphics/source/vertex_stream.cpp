#include "renderstack_toolkit/platform.hpp"
#include "renderstack_toolkit/strong_gl_enums.hpp"
#include "renderstack_toolkit/gl.hpp"
#include "renderstack_graphics/log.hpp"
#include "renderstack_graphics/configuration.hpp"
#include "renderstack_graphics/vertex_attribute.hpp"
#include "renderstack_graphics/buffer.hpp"
#include "renderstack_graphics/vertex_stream_binding.hpp"
#include "renderstack_graphics/vertex_attribute_mapping.hpp"
#include "renderstack_graphics/vertex_stream.hpp"
#include <cassert>
#include <stdexcept>

#define LOG_CATEGORY &log_vertex_stream

namespace renderstack { namespace graphics {

using namespace std;


vertex_stream::vertex_stream()
{
   m_vertex_array = make_shared<class vertex_array>();
}

vertex_stream::~vertex_stream()
{
   m_vertex_array.reset();
}

vertex_stream_binding &vertex_stream::add(
   weak_ptr<class buffer>              vertex_buffer,
   weak_ptr<vertex_attribute_mapping>  mapping, 
   weak_ptr<vertex_attribute>          attribute,
   size_t                              stride
)
{
   m_vertex_stream_bindings.push_back(
      vertex_stream_binding(vertex_buffer, mapping, attribute, stride)
   );
   return m_vertex_stream_bindings.back();
}

void vertex_stream::clear()
{
   m_vertex_stream_bindings.clear();
}

} }


