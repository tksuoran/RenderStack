#ifndef vertex_attribute_hpp_renderstack_graphics
#define vertex_attribute_hpp_renderstack_graphics

#include "renderstack_toolkit/platform.hpp"
#include "renderstack_toolkit/strong_gl_enums.hpp"
#include <exception>

namespace renderstack
{
namespace graphics
{

size_t size_of_type(gl::vertex_attrib_pointer_type::value type);

class vertex_attribute_usage
{
public:
    enum value
    {
        none           = 0,
        position       = 1,
        tangent        = 2,
        normal         = 4,
        bitangent      = 8,
        color          = 16,
        weights        = 32,
        matrix_indices = 64,
        tex_coord      = 128,
        id             = 256,
        //position_texcoord =   1 | 128,
    };
    static const char *desc(enum value);
};

class Vertex_attribute
{
public:
    Vertex_attribute(
        vertex_attribute_usage::value         usage,
        gl::vertex_attrib_pointer_type::value data_type,
        gl::vertex_attrib_pointer_type::value shader_type,
        size_t                                index,
        size_t                                dimension,
        size_t                                offset     = 0,
        bool                                  normalized = false,
        unsigned int                          divisor    = 0)
        : usage(usage)
        , data_type(data_type)
        , shader_type(shader_type)
        , index(index)
        , dimension(dimension)
        , offset(offset)
        , normalized(normalized)
        , divisor(divisor)
    {}

    size_t stride() const
    {
        return dimension * size_of_type(data_type);
    }

    bool operator==(Vertex_attribute const &other) const
    {
        return (usage == other.usage) &&
            (data_type == other.data_type) &&
            (shader_type == other.shader_type) &&
            (dimension == other.dimension) &&
            (offset == other.offset) &&
            (normalized == other.normalized);
    }

    bool operator!=(Vertex_attribute const &other) const
    {
        return (usage != other.usage) ||
            (data_type != other.data_type) ||
            (shader_type != other.shader_type) ||
            (dimension != other.dimension) ||
            (offset != other.offset) ||
            (normalized != other.normalized);
    }

    vertex_attribute_usage::value         usage;
    gl::vertex_attrib_pointer_type::value data_type;
    gl::vertex_attrib_pointer_type::value shader_type;
    size_t                                index;
    size_t                                dimension;
    size_t                                offset;
    bool                                  normalized;
    unsigned int                          divisor;
};

} // namespace graphics
} // namespace renderstack

#endif
