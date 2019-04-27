#include "renderstack_graphics/vertex_attribute.hpp"
#include "renderstack_toolkit/platform.hpp"
#include <stdexcept>

namespace renderstack
{
namespace graphics
{

using namespace std;

const char *vertex_attribute_usage::desc(enum value a)
{
    switch (static_cast<int>(a))
    {
        case none: return "none";
        case position: return "position";
        case tangent: return "tangent";
        case normal: return "normal";
        case bitangent: return "bitangent";
        case color: return "color";
        case weights: return "weights";
        case matrix_indices: return "matrix_indices";
        case tex_coord: return "tex_coord";
        case id: return "id";

        case position | tex_coord: return "position | tex_coord";

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
