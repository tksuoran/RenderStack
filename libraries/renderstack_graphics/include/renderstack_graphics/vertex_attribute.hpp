#ifndef renderstack_graphics__vertex_attribute_hpp
#define renderstack_graphics__vertex_attribute_hpp

#include "renderstack_toolkit/platform.hpp"
#include "renderstack_toolkit/strong_gl_enums.hpp"
#include <exception>

namespace renderstack { namespace graphics {

std::size_t size_of_type(gl::vertex_attrib_pointer_type::value type);

class vertex_attribute_usage
{
public:
   enum value
   {
      none              =   0,
      position          =   1,
      tangent           =   2,
      normal            =   4,
      bitangent         =   8,
      color             =  16,
      weights           =  32,
      matrix_indices    =  64,
      tex_coord         = 128,
      id                = 256,
      //position_texcoord =   1 | 128,
   };
   static const char *desc(enum value);
};

class vertex_attribute
{
public:
   vertex_attribute(
      vertex_attribute_usage::value          usage,
      gl::vertex_attrib_pointer_type::value  data_type,
      gl::vertex_attrib_pointer_type::value  shader_type,
      std::size_t                            index,
      std::size_t                            dimension,
      std::size_t                            offset = 0,
      bool                                   normalized = false
   );

   vertex_attribute_usage::value          usage          () const;
   void                                   set_usage      (vertex_attribute_usage::value value);
   gl::vertex_attrib_pointer_type::value  data_type      () const;
   gl::vertex_attrib_pointer_type::value  shader_type    () const;
   void                                   set_data_type  (gl::vertex_attrib_pointer_type::value value);
   void                                   set_shader_type(gl::vertex_attrib_pointer_type::value value);
   std::size_t                            index          () const;
   std::size_t                            dimension      () const;
   std::size_t                            offset         () const;
   void                                   set_offset     (std::size_t value);
   bool                                   normalized     () const;
   std::size_t                            stride         () const;

   bool operator==(vertex_attribute const &other) const;
   bool operator!=(vertex_attribute const &other) const;

private:
   vertex_attribute_usage::value          m_usage;
   gl::vertex_attrib_pointer_type::value  m_data_type;
   gl::vertex_attrib_pointer_type::value  m_shader_type;
   std::size_t                            m_index;
   std::size_t                            m_dimension;
   std::size_t                            m_offset;
   bool                                   m_normalized;
};

} }

#endif
