#include "renderstack_toolkit/platform.hpp"
#include "renderstack_toolkit/logstream.hpp"
#include "renderstack_graphics/configuration.hpp"
#include "renderstack_graphics/context.hpp"
#include "renderstack_graphics/program.hpp"
#include "renderstack_graphics/vertex_format.hpp"
#include "renderstack_graphics/vertex_stream.hpp"
#include "renderstack_graphics/vertex_stream_mappings.hpp"
#include "renderstack_graphics/vertex_stream_mapping.hpp"
#include <stdexcept>

#define LOG_CATEGORY &log_vertex_stream_mappings

namespace renderstack { namespace graphics {

using namespace std;


vertex_stream_mappings::vertex_stream_mappings(std::string const &name, unsigned int instance_index)
:  m_name(name)
,  m_instance_index(instance_index)
{
}
vertex_stream_mappings::~vertex_stream_mappings()
{
}

unsigned int vertex_stream_mappings::instance_index() const
{
   return m_instance_index;
}

void vertex_stream_mappings::clear()
{
   m_mappings.clear();
}

void vertex_stream_mappings::add(
   std::string const             &name,
   vertex_attribute_usage::value usage,
   unsigned int                  index
)
{
   m_mappings.push_back(
      make_shared<vertex_stream_mapping>(
         name,
         usage,
         index
      )
   );
}

void vertex_stream_mappings::add(
   std::string const             &name,
   vertex_attribute_usage::value usage,
   unsigned int                  src_index,
   unsigned int                  dst_index
)
{
   m_mappings.push_back(
      make_shared<vertex_stream_mapping>(
         name,
         usage,
         src_index,
         usage,
         dst_index
      )
   );
}

void vertex_stream_mappings::add(
   std::string const             &name,
   vertex_attribute_usage::value src_usage,
   unsigned int                  src_index,
   vertex_attribute_usage::value dst_usage,
   unsigned int                  dst_index
)
{
   m_mappings.push_back(
      make_shared<vertex_stream_mapping>(
         name,
         src_usage,
         src_index,
         dst_usage,
         dst_index
      )
   );
}

void vertex_stream_mappings::bind_attrib_locations(program &program)
{
   if (configuration::can_use.bind_frag_data_location)
   {
      for (auto mapping = m_mappings.begin(); mapping != m_mappings.end(); ++mapping)
         program.bind_attrib_location((*mapping)->dst_index(), (*mapping)->name());

      program.bind_frag_data_location(0, "out_color");
   }
}

void vertex_stream_mappings::bind_attributes(
   vertex_stream &vertex_stream, 
   vertex_format const &vertex_format
) const
{
   slog_trace("vertex_stream_mappings::bind_attributes()", m_name.c_str());

   if (vertex_stream.count() != 0)
      throw runtime_error("this vertex stream is already bound");

   for (auto mapping = m_mappings.cbegin(); mapping != m_mappings.cend(); ++mapping)
   {
      if (vertex_format.has_attribute((*mapping)->src_usage(), (*mapping)->src_index()))
      {
         auto attribute = vertex_format.find_attribute((*mapping)->src_usage(), (*mapping)->src_index());

         log_trace("adding vertex attribute: usage = %s, data_type = %s, dimension = %u, index = %u",
            vertex_attribute_usage::desc(attribute->usage()),
            gl::enum_string(attribute->data_type()),
            static_cast<unsigned int>(attribute->dimension()),
            static_cast<unsigned int>(attribute->index()));

         vertex_stream.add(
            *mapping,
            attribute,
            vertex_format.stride()
         );
      }
   }
}

} }
