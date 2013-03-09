#ifndef renderstack_graphics__vertex_stream_binding_hpp
#define renderstack_graphics__vertex_stream_binding_hpp

#include "renderstack_toolkit/platform.hpp"
#include "renderstack_toolkit/strong_gl_enums.hpp"
#include "renderstack_graphics/vertex_stream_mapping.hpp"
#include <memory>
#include <vector>
#include <string>
#include <map>

namespace renderstack { namespace graphics {

class vertex_attribute;
class vertex_stream_mapping;

class vertex_stream_binding
{
public:
   vertex_stream_binding(
      std::weak_ptr<class vertex_stream_mapping>   mapping,
      std::weak_ptr<class vertex_attribute>        attribute,
      std::size_t                                  stride
   );

   vertex_stream_binding(vertex_stream_binding const &other)
   :  m_vertex_stream_mapping (other.vertex_stream_mapping())
   ,  m_vertex_attribute      (other.vertex_attribute())
   ,  m_stride                (other.stride())
   {
   }

   vertex_stream_binding &operator=(vertex_stream_binding other) 
   {
      swap(other);
      return *this;
   }

   void swap(vertex_stream_binding &other) 
   {
      m_vertex_stream_mapping.swap(other.vertex_stream_mapping());
      m_vertex_attribute.swap(other.vertex_attribute());
      std::swap(m_stride, other.stride_());
   }

   std::weak_ptr<class vertex_stream_mapping>   const &vertex_stream_mapping() const;
   std::weak_ptr<class vertex_stream_mapping>   &vertex_stream_mapping();
   std::weak_ptr<class vertex_attribute>        const &vertex_attribute() const;
   std::weak_ptr<class vertex_attribute>        &vertex_attribute();
   std::size_t                                  stride() const;

private:
   std::size_t                                  &stride_() { return m_stride; }

private:
   std::weak_ptr<class vertex_stream_mapping>   m_vertex_stream_mapping;
   std::weak_ptr<class vertex_attribute>        m_vertex_attribute;
   std::size_t                                  m_stride;
};

} }

#endif
