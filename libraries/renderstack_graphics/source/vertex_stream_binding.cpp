#include "renderstack_toolkit/platform.hpp"
#include "renderstack_graphics/vertex_stream_binding.hpp"

namespace renderstack { namespace graphics {

using namespace std;

vertex_stream_binding::vertex_stream_binding(
   weak_ptr<class buffer>                    vertex_buffer,
   weak_ptr<class vertex_attribute_mapping>  mapping,
   weak_ptr<class vertex_attribute>          attribute,
   size_t                                    stride
)
:  m_vertex_buffer(vertex_buffer)
,  m_vertex_attribute_mapping(mapping)
,  m_vertex_attribute(attribute)
,  m_stride(stride)
{
}

weak_ptr<class buffer> const &vertex_stream_binding::vertex_buffer() const
{
   return m_vertex_buffer;
}
weak_ptr<class buffer> &vertex_stream_binding::vertex_buffer()
{
   return m_vertex_buffer;
}
weak_ptr<class vertex_attribute_mapping> const &vertex_stream_binding::vertex_attribute_mapping() const
{
   return m_vertex_attribute_mapping;
}
weak_ptr<class vertex_attribute_mapping> &vertex_stream_binding::vertex_attribute_mapping()
{
   return m_vertex_attribute_mapping;
}
weak_ptr<class vertex_attribute> const &vertex_stream_binding::vertex_attribute() const
{
   return m_vertex_attribute;
}
weak_ptr<class vertex_attribute> &vertex_stream_binding::vertex_attribute()
{
   return m_vertex_attribute;
}

size_t vertex_stream_binding::stride() const
{
   return m_stride;
}

} }
