#ifndef vertex_array_hpp_renderstack_graphics
#define vertex_array_hpp_renderstack_graphics

#include "renderstack_toolkit/platform.hpp"
#include "renderstack_toolkit/gl.hpp"
#include "renderstack_toolkit/strong_gl_enums.hpp"
#include "renderstack_toolkit/log.hpp"
#include "renderstack_graphics/vertex_attribute.hpp"
#include <vector>
#include <cstdint>
#include <cstddef>
#include <memory>

namespace renderstack { namespace graphics {

class index_buffer;

#define RS_ATTRIBUTE_COUNT 32 // TODO Get rid of this kind of defines?

struct vertex_array_state
{
   bool                          attrib_enabled[RS_ATTRIBUTE_COUNT];
   std::uint32_t                 attrib_divisor[RS_ATTRIBUTE_COUNT];
   std::shared_ptr<class buffer> element_array_buffer_binding;
};

class vertex_array
{
public:
   vertex_array(bool default_ = false);
   ~vertex_array();

   void use() const;
   void clear_enabled_attribs();
   void enable_attrib(
      uint32_t index,
      uint32_t dimension,
      gl::vertex_attrib_pointer_type::value shader_type,
      gl::vertex_attrib_pointer_type::value data_type,
      uint32_t stride,
      uint32_t offset,
      bool normalized,
      uint32_t divisor
   );
   void apply_attrib_enables();

   void trash()
   {
      for (unsigned int i = 0; i < RS_ATTRIBUTE_COUNT; ++i)
      {
         m_effective.attrib_enabled[i] = false;
         m_effective.attrib_divisor[i] = 0;
         gl::disable_vertex_attrib_array(i);
#if !defined(__APPLE__)
         gl::vertex_attrib_divisor(i, 0);
#endif
      }
      m_effective.element_array_buffer_binding.reset();
      gl::bind_buffer(gl::buffer_target::element_array_buffer, 0);
   }

   // returns previous index buffer
   std::shared_ptr<class buffer> set_index_buffer(std::shared_ptr<class buffer> buffer);
   std::shared_ptr<class buffer> index_buffer() const;

friend class renderer;
private:
   unsigned int   gl_name() { return m_gl_name; }
   void           set_bound(bool value);

private:
   GLuint               m_gl_name;
   vertex_array_state   m_requested;
   vertex_array_state   m_effective;
   bool                 m_bound;       // Is currently bound?
};


} }

#endif
