#ifndef vertex_stream_hpp_renderstack_graphics
#define vertex_stream_hpp_renderstack_graphics

#include "renderstack_toolkit/platform.hpp"
#include "renderstack_graphics/vertex_array.hpp"
#include "renderstack_graphics/vertex_attribute.hpp"
#include "renderstack_graphics/vertex_stream_binding.hpp"
#include "renderstack_toolkit/gl.hpp"
#include "renderstack_toolkit/strong_gl_enums.hpp"
#include <vector>
#include <cstddef>
#include <memory>

namespace renderstack { namespace graphics {

class vertex_array;
class buffer;
class vertex_attribute_mapping;

class vertex_stream : public std::enable_shared_from_this<vertex_stream>
{
public:
   vertex_stream();
   ~vertex_stream();

   vertex_stream_binding &add(
      std::weak_ptr<class buffer>               vertex_buffer,
      std::weak_ptr<vertex_attribute_mapping>   vertex_attribute_mapping,
      std::weak_ptr<vertex_attribute>           attribute,
      std::size_t                               stride);

   void        clear();
   std::size_t count() const { return m_vertex_stream_bindings.size(); }

   std::shared_ptr<class vertex_array>       &vertex_array()         { return m_vertex_array; }
   std::shared_ptr<class vertex_array> const &vertex_array() const   { return m_vertex_array; }

   std::vector<vertex_stream_binding>        &bindings()       { return m_vertex_stream_bindings; }
   std::vector<vertex_stream_binding> const  &bindings() const { return m_vertex_stream_bindings; }

private:
   std::vector<vertex_stream_binding>  m_vertex_stream_bindings;
   std::shared_ptr<class vertex_array> m_vertex_array;
};

} }

#endif
