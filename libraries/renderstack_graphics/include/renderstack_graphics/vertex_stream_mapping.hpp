#ifndef renderstack_graphics__vertex_stream_mapping_hpp
#define renderstack_graphics__vertex_stream_mapping_hpp

#include "renderstack_toolkit/platform.hpp"
#include "renderstack_graphics/vertex_attribute.hpp"

#include <memory>
#include <vector>
#include <string>
#include <map>

namespace renderstack { namespace graphics {

class vertex_stream_mapping
{
public:
   vertex_stream_mapping(std::string const &name, vertex_attribute_usage::value usage, unsigned int index);
   vertex_stream_mapping(std::string const &name, vertex_attribute_usage::value src_usage, unsigned int src_index, vertex_attribute_usage::value dst_usage, unsigned int dst_index);

   std::string const             &name() const;
   vertex_attribute_usage::value src_usage() const;
   vertex_attribute_usage::value dst_usage() const;
   unsigned int                  src_index() const;
   unsigned int                  dst_index() const;

private:
   std::string                   m_name;
   vertex_attribute_usage::value m_src_usage;
   vertex_attribute_usage::value m_dst_usage;
   unsigned int                  m_src_index;
   unsigned int                  m_dst_index;
};

} }

#endif
