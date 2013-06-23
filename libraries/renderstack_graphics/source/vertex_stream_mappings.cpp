#include "renderstack_toolkit/platform.hpp"
#include "renderstack_toolkit/logstream.hpp"
#include "renderstack_graphics/configuration.hpp"
#include "renderstack_graphics/program.hpp"
#include "renderstack_graphics/vertex_format.hpp"
#include "renderstack_graphics/vertex_stream.hpp"
#include "renderstack_graphics/vertex_stream_mappings.hpp"
#include "renderstack_graphics/vertex_stream_mapping.hpp"
#include <stdexcept>

#define LOG_CATEGORY &log_graphics_vertex_stream_mappings

namespace renderstack { namespace graphics {

using namespace std;


vertex_stream_mappings::vertex_stream_mappings()
{
}
vertex_stream_mappings::~vertex_stream_mappings()
{
}

void vertex_stream_mappings::clear()
{
   m_mappings.clear();
}

void vertex_stream_mappings::add(
   string const                  &name,
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
   string const                  &name,
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
   string const                  &name,
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
   for (auto mapping = m_mappings.begin(); mapping != m_mappings.end(); ++mapping)
      program.bind_attrib_location((*mapping)->dst_index(), (*mapping)->name());
}

void vertex_stream_mappings::add_to_vertex_stream(
   shared_ptr<class vertex_stream>  vertex_stream,
   shared_ptr<class buffer>         vertex_buffer,
   shared_ptr<class vertex_format>  vertex_format
) const
{
   //shared_ptr<vertex_stream> vertex_stream = make_shared<class vertex_stream>();
   slog_trace("vertex_stream_mappings::bind_attributes()");

   if (vertex_stream->count() != 0)
      throw runtime_error("this vertex stream is already bound");

   for (auto mapping = m_mappings.cbegin(); mapping != m_mappings.cend(); ++mapping)
   {
      if (vertex_format->has_attribute((*mapping)->src_usage(), (*mapping)->src_index()))
      {
         auto attribute = vertex_format->find_attribute((*mapping)->src_usage(), (*mapping)->src_index());

         log_trace("adding vertex attribute: name = %s, usage = %s, data_type = %s, dimension = %u, index = %u",
            (*mapping)->name().c_str(),
            vertex_attribute_usage::desc(attribute->usage()),
            gl::enum_string(attribute->data_type()),
            static_cast<unsigned int>(attribute->dimension()),
            static_cast<unsigned int>(attribute->index()));

         vertex_stream->add(
            vertex_buffer,
            *mapping,
            attribute,
            vertex_format->stride()
         );
      }
   }
}

} }
