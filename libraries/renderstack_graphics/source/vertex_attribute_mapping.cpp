#include "renderstack_toolkit/platform.hpp"
#include "renderstack_graphics/vertex_attribute_mapping.hpp"

namespace renderstack { namespace graphics {

using namespace std;

vertex_attribute_mapping::vertex_attribute_mapping(
   string const                  &name,
   vertex_attribute_usage::value usage,
   unsigned int                  index
)
:  m_name(name)
,  m_src_usage(usage)
,  m_dst_usage(usage)
,  m_src_index(index)
,  m_dst_index(index)
{
}

vertex_attribute_mapping::vertex_attribute_mapping(
   string const                  &name,
   vertex_attribute_usage::value src_usage,
   unsigned int                  src_index,
   vertex_attribute_usage::value dst_usage,
   unsigned int                  dst_index
)
:  m_name(name)
,  m_src_usage(src_usage)
,  m_dst_usage(dst_usage)
,  m_src_index(src_index)
,  m_dst_index(dst_index)
{
}

string const &vertex_attribute_mapping::name() const
{
   return m_name;
}

vertex_attribute_usage::value vertex_attribute_mapping::src_usage() const
{
   return m_src_usage;
}

vertex_attribute_usage::value vertex_attribute_mapping::dst_usage() const
{
   return m_dst_usage;
}

unsigned int vertex_attribute_mapping::src_index() const
{
   return m_src_index;
}

unsigned int vertex_attribute_mapping::dst_index() const
{
   return m_dst_index;
}

} }
