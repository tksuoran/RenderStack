#ifndef renderstack_graphics__vertex_stream_hpp
#define renderstack_graphics__vertex_stream_hpp

#include "renderstack_toolkit/platform.hpp"
#include "renderstack_graphics/vertex_attribute.hpp"
#include "renderstack_graphics/vertex_stream_binding.hpp"
#include "renderstack_toolkit/gl.hpp"
#include <vector>
#include <cstddef>
#include <memory>

namespace renderstack { namespace graphics {

class index_buffer;
class vertex_buffer;
class vertex_stream_mapping;

class vertex_stream : public std::enable_shared_from_this<vertex_stream>
{
public:
   vertex_stream();
   ~vertex_stream();

public:
   std::vector<vertex_stream_binding> const &vertex_stream_bindings() const;
   bool                    dirty() const;
   void                    set_dirty(bool value);
   vertex_stream_binding   &add(std::weak_ptr<vertex_stream_mapping> mapping, std::weak_ptr<vertex_attribute> attribute, std::size_t stride);
   void                    clear();
   std::size_t             count() const { return m_vertex_stream_bindings.size(); }

#if 0
   void draw_elements_base_vertex(
      gl::begin_mode::value         mode, 
      GLsizei                       count, 
      gl::draw_elements_type::value type, 
      std::size_t                   indices, 
      GLint                         basevertex
   );
#endif

public:
   bool use();
   void setup_attribute_pointers(GLint basevertex = 0);
   void disable_attributes();

private:
#if defined(SUPPORT_LEGACY_OPENGL)
   void setup_attribute_pointers_old(GLint basevertex);
   void disable_attributes_old();
#endif
   void setup_attribute_pointers_new(GLint basevertex);
   void disable_attributes_new();

private:
   std::vector<vertex_stream_binding>  m_vertex_stream_bindings;
   bool                                m_dirty;
   unsigned int                        m_vertex_array_object;
};

} }

#endif
