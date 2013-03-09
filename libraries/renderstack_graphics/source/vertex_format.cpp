#include "renderstack_toolkit/platform.hpp"
#include "renderstack_graphics/vertex_format.hpp"
#include <algorithm>
#include <stdexcept>

namespace renderstack { namespace graphics {

using namespace std;

vertex_format::vertex_format():m_stride(0){}

void vertex_format::clear()
{
   m_attributes.clear();
   m_stride = 0;
}

shared_ptr<vertex_attribute> vertex_format::make_attribute(
   vertex_attribute_usage::value          usage,
   gl::vertex_attrib_pointer_type::value  data_type,
   gl::vertex_attrib_pointer_type::value  shader_type,
   size_t                                 index,
   size_t                                 dimension,
   bool                                   normalized
)
{
   auto attribute = make_shared<vertex_attribute>(
      usage,
      data_type,
      shader_type,
      index,
      dimension,
      m_stride,
      normalized
   );

   // Align attributes to their type
   switch (attribute->stride())
   {
   case 2: while (m_stride & 1) ++m_stride;
   case 4: while (m_stride & 3) ++m_stride;
   }

   attribute->set_offset(m_stride);
   m_attributes.push_back(attribute);
   m_stride += attribute->stride();
   return m_attributes.back();
}

bool vertex_format::match(vertex_format const& other) const
{
   if (m_attributes.size() != other.m_attributes.size())
      return false;

   for (size_t i = 0; i < m_attributes.size(); ++i)
      if (m_attributes[i] != other.m_attributes[i])
         return false;

   return true;
}

bool vertex_format::has_attribute(vertex_attribute_usage::value usage, unsigned int index) const
{
   for (auto i = m_attributes.begin(); i != m_attributes.end(); ++i)
   {
      if (
         ((*i)->usage() == usage) &&
         ((*i)->index() == index)
      )
      {
         return true;
      }
   }

   return false;
}

shared_ptr<vertex_attribute> vertex_format::find_attribute_maybe(
   vertex_attribute_usage::value   usage, 
   unsigned int                    index
) const
{
   for (auto i = m_attributes.begin(); i != m_attributes.end(); ++i)
   {
      if (
         ((*i)->usage() == usage) &&
         ((*i)->index() == index)
      )
      {
         return *i;
      }
   }

   return nullptr;
}

shared_ptr<vertex_attribute> vertex_format::find_attribute(
   vertex_attribute_usage::value   usage, 
   unsigned int                    index
) const
{
   for (auto i = m_attributes.begin(); i != m_attributes.end(); ++i) {
      if (
         ((*i)->usage() == usage) &&
         ((*i)->index() == index)
      )
      {
         return *i;
      }
   }

   throw runtime_error("vertex_attribute not found");
}

} }
