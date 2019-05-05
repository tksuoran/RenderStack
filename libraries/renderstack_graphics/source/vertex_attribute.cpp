#include "renderstack_graphics/vertex_attribute.hpp"
#include "renderstack_toolkit/platform.hpp"
#include <stdexcept>

namespace renderstack
{
namespace graphics
{

using namespace std;

const char *Vertex_attribute::desc(Usage usage)
{
    switch (usage.value)
    {
        case Usage::none: return "none";
        case Usage::position: return "position";
        case Usage::tangent: return "tangent";
        case Usage::normal: return "normal";
        case Usage::bitangent: return "bitangent";
        case Usage::color: return "color";
        case Usage::weights: return "weights";
        case Usage::matrix_indices: return "matrix_indices";
        case Usage::tex_coord: return "tex_coord";
        case Usage::id: return "id";
        case Usage::position | Usage::tex_coord: return "position | tex_coord";
        default: return "???";
    }
}

size_t size_of_type(gl::vertex_attrib_pointer_type::value type)
{
    switch (type)
    {
        case gl::vertex_attrib_pointer_type::byte: return 1;
        case gl::vertex_attrib_pointer_type::double_: return 8;
        case gl::vertex_attrib_pointer_type::float_: return 4;
        case gl::vertex_attrib_pointer_type::half_float: return 2;
        case gl::vertex_attrib_pointer_type::int_: return 4;
        case gl::vertex_attrib_pointer_type::short_: return 2;
        case gl::vertex_attrib_pointer_type::unsigned_byte: return 1;
        case gl::vertex_attrib_pointer_type::unsigned_int: return 4;
        case gl::vertex_attrib_pointer_type::unsigned_short: return 2;
        default:
            throw runtime_error("unknown attribute type");
    }
}

} // namespace graphics
} // namespace renderstack
