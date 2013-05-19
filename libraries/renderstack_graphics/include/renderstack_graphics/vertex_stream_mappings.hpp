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

class vertex_stream_mappings : public std::enable_shared_from_this<vertex_stream_mappings>
{
public:
   vertex_stream_mappings();
   ~vertex_stream_mappings();

public:
   void clear();
   void add(std::string const &name, vertex_attribute_usage::value usage,     unsigned int index);
   void add(std::string const &name, vertex_attribute_usage::value usage,     unsigned int src_index, unsigned int dst_index);
   void add(std::string const &name, vertex_attribute_usage::value src_usage, unsigned int src_index, vertex_attribute_usage::value dst_usage, unsigned int dst_index);

   void add_to_vertex_stream(
      std::shared_ptr<class vertex_stream> vertex_stream,
      std::shared_ptr<class vertex_buffer> vertex_buffer,
      std::shared_ptr<class vertex_format> vertex_format) const;

   void bind_attrib_locations(program &program);
   std::vector<std::shared_ptr<vertex_stream_mapping> > const &mappings() const { return m_mappings; }

private:
   std::vector<std::shared_ptr<vertex_stream_mapping> > m_mappings;
};

} }

#endif
