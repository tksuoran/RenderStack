#include "renderstack_toolkit/platform.hpp"
#include "renderstack_graphics/vertex_attribute.hpp"
#include <stdexcept>

namespace renderstack { namespace graphics {

using namespace std;

const char *vertex_attribute_usage::desc(enum value a)
{
   switch (static_cast<int>(a))
   {
   case none          : return "none";
   case position      : return "position";
   case tangent       : return "tangent";
   case normal        : return "normal";
   case bitangent     : return "bitangent";
   case color         : return "color";
   case weights       : return "weights";
   case matrix_indices: return "matrix_indices";
   case tex_coord     : return "tex_coord";
   case id            : return "id";

   case position | tex_coord  : return "position | tex_coord";

   default:             return "???";
   }
}

vertex_attribute::vertex_attribute(
   vertex_attribute_usage::value          usage,
   gl::vertex_attrib_pointer_type::value  data_type,
   gl::vertex_attrib_pointer_type::value  shader_type,
   size_t                                 index,
   size_t                                 dimension,
   size_t                                 offset,
   bool                                   normalized,
   unsigned int                           divisor
)
:  m_usage        (usage)
,  m_data_type    (data_type)
,  m_shader_type  (shader_type)
,  m_index        (index)
,  m_dimension    (dimension)
,  m_offset       (offset)
,  m_normalized   (normalized)
,  m_divisor      (divisor)
{
}

vertex_attribute_usage::value vertex_attribute::usage() const
{
   return m_usage;
}

void vertex_attribute::set_usage(vertex_attribute_usage::value value)
{
   m_usage = value;
}

gl::vertex_attrib_pointer_type::value vertex_attribute::data_type() const
{
   return m_data_type;
}

gl::vertex_attrib_pointer_type::value vertex_attribute::shader_type() const
{
   return m_shader_type;
}

void vertex_attribute::set_data_type(gl::vertex_attrib_pointer_type::value value)
{
   m_data_type = value;
}

void vertex_attribute::set_shader_type(gl::vertex_attrib_pointer_type::value value)
{
   m_shader_type = value;
}

size_t vertex_attribute::index() const
{
   return m_index;
}

size_t vertex_attribute::dimension() const
{
   return m_dimension;
}

size_t vertex_attribute::offset() const
{
   return m_offset;
}

void vertex_attribute::set_offset(size_t value)
{
   m_offset = value;
}

bool vertex_attribute::normalized() const
{
   return m_normalized;
}

size_t vertex_attribute::stride() const
{
   return m_dimension * size_of_type(m_data_type);
}

unsigned int vertex_attribute::divisor() const
{
   return m_divisor;
}

void vertex_attribute::set_divisor(unsigned int value)
{
   m_divisor = value;
}

bool vertex_attribute::operator==(vertex_attribute const &other) const
{
   return
      (m_usage       == other.m_usage      ) &&
      (m_data_type   == other.m_data_type  ) &&
      (m_shader_type == other.m_shader_type) &&
      (m_dimension   == other.m_dimension  ) &&
      (m_offset      == other.m_offset     ) &&
      (m_normalized  == other.m_normalized );
}

bool vertex_attribute::operator!=(vertex_attribute const &other) const
{
   return
      (m_usage       != other.m_usage      ) ||
      (m_data_type   != other.m_data_type  ) ||
      (m_shader_type != other.m_shader_type) ||
      (m_dimension   != other.m_dimension  ) ||
      (m_offset      != other.m_offset     ) ||
      (m_normalized  != other.m_normalized );
}

size_t size_of_type(gl::vertex_attrib_pointer_type::value type)
{
   switch (type)
   {
   case gl::vertex_attrib_pointer_type::byte:              return 1;
   case gl::vertex_attrib_pointer_type::double_:           return 8;
   case gl::vertex_attrib_pointer_type::float_:            return 4;
   case gl::vertex_attrib_pointer_type::half_float:        return 2;
   case gl::vertex_attrib_pointer_type::int_:              return 4;
   case gl::vertex_attrib_pointer_type::short_:            return 2;
   case gl::vertex_attrib_pointer_type::unsigned_byte:     return 1;
   case gl::vertex_attrib_pointer_type::unsigned_int:      return 4;
   case gl::vertex_attrib_pointer_type::unsigned_short:    return 2;
   default:
      throw runtime_error("unknown attribute type");
   }
}

} }
