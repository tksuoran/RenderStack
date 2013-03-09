#ifndef renderstack_graphics__vertex_stream_mappings_hpp
#define renderstack_graphics__vertex_stream_mappings_hpp

#include "renderstack_toolkit/platform.hpp"
#include "renderstack_graphics/vertex_attribute.hpp"
#include "renderstack_graphics/vertex_stream_mapping.hpp"

#include <memory>
#include <vector>
#include <string>
#include <map>

namespace renderstack { namespace graphics {

class program;
class vertex_format;
class vertex_stream;

class context_vertex_stream_mappings;

class vertex_stream_mappings : public std::enable_shared_from_this<vertex_stream_mappings>
{
friend class context;
private:
   vertex_stream_mappings(std::string const &name, unsigned int instance_index);

public:
   ~vertex_stream_mappings();

public:
   unsigned int    instance_index() const;
   void            clear();
   void            add(std::string const &name, vertex_attribute_usage::value usage,     unsigned int index);
   void            add(std::string const &name, vertex_attribute_usage::value usage,     unsigned int src_index, unsigned int dst_index);
   void            add(std::string const &name, vertex_attribute_usage::value src_usage, unsigned int src_index, vertex_attribute_usage::value dst_usage, unsigned int dst_index);
   void            bind_attributes(vertex_stream &vertex_stream, vertex_format const &vertex_format) const;
   void            bind_attrib_locations(program &program);
   std::vector<std::shared_ptr<vertex_stream_mapping> > const &mappings() const { return m_mappings; }

private:
   std::string                                           m_name;
   unsigned int                                          m_instance_index;
   std::vector<std::shared_ptr<vertex_stream_mapping> >  m_mappings;
};

} }

#endif
