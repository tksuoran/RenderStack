#include "renderstack_toolkit/platform.hpp"
#include "renderstack_toolkit/strong_gl_enums.hpp"
#include "renderstack_toolkit/logstream.hpp"
#include "renderstack_toolkit/gl.hpp"
#include "renderstack_graphics/configuration.hpp"
#include "renderstack_graphics/index_buffer.hpp"
#include "renderstack_graphics/vertex_attribute.hpp"
#include "renderstack_graphics/vertex_buffer.hpp"
#include "renderstack_graphics/vertex_stream_binding.hpp"
#include "renderstack_graphics/vertex_stream_mapping.hpp"
#include "renderstack_graphics/vertex_stream.hpp"
#include <cassert>
#include <stdexcept>

#define LOG_CATEGORY &log_vertex_stream

namespace renderstack { namespace graphics {

using namespace std;

vertex_stream::vertex_stream()
:  m_vertex_array_object(~0u)
,  m_last_base_vertex(~0u)
{
#if defined(RENDERSTACK_USE_GLES3) || !defined(RENDERSTACK_USE_GLES2_OR_GLES3)
   if (
      configuration::must_use_vertex_array_object || 
      (
         configuration::can_use.vertex_array_object && 
         configuration::use_vertex_array_object
      )
   )
   {
      gl::gen_vertex_arrays(1, &m_vertex_array_object);
   }
#endif
}

vertex_stream::~vertex_stream()
{
#if defined(RENDERSTACK_USE_GLES3) || !defined(RENDERSTACK_USE_GLES2_OR_GLES3)
   if (
      (m_vertex_array_object != ~0u) &&
      (
         configuration::must_use_vertex_array_object || 
         (
            configuration::can_use.vertex_array_object && 
            configuration::use_vertex_array_object
         )
      )
   )
   {
      gl::delete_vertex_arrays(1, &m_vertex_array_object);
   }
#endif
}

vertex_stream_binding &vertex_stream::add(
   weak_ptr<vertex_stream_mapping>  mapping, 
   weak_ptr<vertex_attribute>       attribute,
   size_t                           stride
)
{
   m_vertex_stream_bindings.push_back(
      vertex_stream_binding(mapping, attribute, stride)
   );
   return m_vertex_stream_bindings.back();
}

void vertex_stream::clear()
{
   m_vertex_stream_bindings.clear();
}
   
bool vertex_stream::use()
{
#if defined(RENDERSTACK_USE_GLES3) || !defined(RENDERSTACK_USE_GLES2_OR_GLES3)
   if (
      configuration::must_use_vertex_array_object || 
      (
         configuration::can_use.vertex_array_object && 
         configuration::use_vertex_array_object
      )
   )
   {
      gl::bind_vertex_array(m_vertex_array_object);
      return true;
   }
#endif
   return false;
}

void vertex_stream::setup_attribute_pointers(GLint base_vertex)
{
   m_last_base_vertex = base_vertex;

   if (configuration::use_gl1)
   {
#     if defined(SUPPORT_LEGACY_OPENGL)
      {
         setup_attribute_pointers_old(basevertex);
      }
#     else
      {
         throw std::exception();
      }
#     endif
   }
   else
   {
      setup_attribute_pointers_new(base_vertex);
   }
}

#if defined(SUPPORT_LEGACY_OPENGL)
void vertex_stream::setup_attribute_pointers_old(GLint basevertex)
{
   for (auto i = m_vertex_stream_bindings.begin(); i != m_vertex_stream_bindings.end(); ++i)
   {
      vertex_stream_binding  *binding   = *i;
      class vertex_attribute *attribute = binding->vertex_attribute();
      vertex_stream_mapping  *mapping   = binding->vertex_stream_mapping();

      switch (mapping->dst_usage())
      {
      case vertex_attribute_usage::position:
         {
            gl::enable_client_state(gl::array_cap::vertex_array);
            gl::vertex_pointer(
               attribute->dimension(), 
               attribute->type(), 
               binding->stride(), 
               reinterpret_cast<char*>(basevertex * binding->stride() + attribute->offset())
            );
         }
         break;
      case vertex_attribute_usage::normal:
         {
            gl::enable_client_state(gl::array_cap::normal_array);
            gl::normal_pointer(
               attribute->type(),
               binding->stride(),
               reinterpret_cast<char*>(basevertex * binding->stride() + attribute->offset())
            );
         }
         break;
      case vertex_attribute_usage::color:
         {
            if ((attribute->dimension() != 3) && (attribute->dimension() != 4)) {
               throw std::exception();
               continue;
            }
            gl::enable_client_state(gl::array_cap::color_array);
            gl::color_pointer(
               attribute->dimension(), 
               attribute->type(), 
               binding->stride(), 
               reinterpret_cast<char*>(basevertex * binding->stride() + attribute->offset())
            );
         }
         break;
      case vertex_attribute_usage::tex_coord:
         {
            gl::client_active_texture(gl::texture_unit::texture0 + binding->vertex_stream_mapping()->dst_index());
            gl::enable_client_state(gl::array_cap::texture_coord_array);
            gl::texcoord_pointer(
               attribute->dimension(), 
               attribute->type(), 
               binding->stride(), 
               reinterpret_cast<char*>(basevertex * binding->stride() + attribute->offset())
            );
         }
         break;
      default:
         break;
      }
   }
}
#endif // defined(SUPPORT_LEGACY_OPENGL)

void vertex_stream::setup_attribute_pointers_new(GLint basevertex)
{
   slog_trace("vertex_stream::setup_attribute_pointers_new(basevertex = %d", basevertex);

   for (auto i = m_vertex_stream_bindings.begin(); i != m_vertex_stream_bindings.end(); ++i)
   {
      auto binding    = *i;
      auto attribute  = binding.vertex_attribute().lock();
      auto mapping    = binding.vertex_stream_mapping().lock();

      assert(attribute);
      assert(mapping);

      gl::enable_vertex_attrib_array(mapping->dst_index());
#if defined(RENDERSTACK_USE_GLES3) || !defined(RENDERSTACK_USE_GLES2_OR_GLES3)
      if (
         (attribute->shader_type() == gl::vertex_attrib_pointer_type::double_   ) ||
         (attribute->shader_type() == gl::vertex_attrib_pointer_type::float_    ) ||
         (attribute->shader_type() == gl::vertex_attrib_pointer_type::half_float)
      )
#endif
      {
         gl::vertex_attrib_pointer(
            mapping->dst_index(),
            static_cast<GLint>(attribute->dimension()), 
            attribute->data_type(), 
            attribute->normalized(),
            static_cast<GLsizei>(binding.stride()), 
            reinterpret_cast<char*>(basevertex * binding.stride() + attribute->offset())
         );
      }
#if defined(RENDERSTACK_USE_GLES3) || !defined(RENDERSTACK_USE_GLES2_OR_GLES3)
      else
      {
         gl::vertex_attrib_i_pointer(
            mapping->dst_index(),
            static_cast<GLint>(attribute->dimension()),
            attribute->data_type(),
            static_cast<GLsizei>(binding.stride()),
            reinterpret_cast<char*>(basevertex * binding.stride() + attribute->offset())
         );
      }
#endif
   }
}

void vertex_stream::disable_attributes()
{
   if (configuration::use_gl1)
   {
#if defined(SUPPORT_LEGACY_OPENGL)
      disable_attributes_old();
#else
      throw std::exception();
#endif
   }
   else
   {
      disable_attributes_new();
   }
}

#if defined(SUPPORT_LEGACY_OPENGL)
void vertex_stream::disable_attributes_old()
{
   for (auto i = vertex_stream_bindings().begin(); i != vertex_stream_bindings().end(); ++i)
   {
      vertex_stream_binding  *binding   = *i;
      class vertex_attribute *attribute = binding->vertex_attribute();
      vertex_stream_mapping  *mapping   = binding->vertex_stream_mapping();

      switch (mapping->dst_usage())
      {
      case vertex_attribute_usage::position:
         gl::disable_client_state(gl::array_cap::vertex_array);
         break;
      case vertex_attribute_usage::normal:
         gl::disable_client_state(gl::array_cap::normal_array);
         break;
      case vertex_attribute_usage::color:
         gl::disable_client_state(gl::array_cap::color_array);
         break;
      case vertex_attribute_usage::tex_coord:
         gl::client_active_texture(gl::texture_unit::texture0 + binding->vertex_stream_mapping()->dst_index());
         gl::disable_client_state(gl::array_cap::texture_coord_array);
         break;
      default:
         break;
      }
   }
}
#endif

void vertex_stream::disable_attributes_new()
{
   for (auto i = m_vertex_stream_bindings.begin(); i != m_vertex_stream_bindings.end(); ++i)
   {
      auto binding = *i;
      auto mapping = binding.vertex_stream_mapping().lock();

      assert(mapping);

      gl::disable_vertex_attrib_array(mapping->dst_index());
   }
}

uint32_t vertex_stream::last_base_vertex()
{
   return m_last_base_vertex;
}

} }


