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

class Vertex_attribute
{
public:
    struct Usage
    {
        using Value = unsigned int;
        static constexpr Value none           = 0U;
        static constexpr Value position       = 1U;
        static constexpr Value tangent        = 2U;
        static constexpr Value normal         = 4U;
        static constexpr Value bitangent      = 8U;
        static constexpr Value color          = 16U;
        static constexpr Value weights        = 32U;
        static constexpr Value matrix_indices = 64U;
        static constexpr Value tex_coord      = 128U;
        static constexpr Value id             = 256U;

        Usage(Value value) : value(value) {}
        Usage(const Usage &other) : value(other.value) {}
        Usage(Usage &&other) : value(other.value) {}

        bool operator==(const Usage &other) const
        {
            return value == other.value;
        }

        bool operator!=(const Usage &other) const
        {
            return value != other.value;
        }

        Usage &operator=(const Usage &other)
        {
            value = other.value;
            return *this;
        }

        Usage &operator=(Usage &&other)
        {
            value = other.value;
            return *this;
        }

        Usage operator|(const Usage &rhs) const
        {
            return Usage(value | rhs.value);
        }

        Usage &operator|=(const Usage &rhs)
        {
            value |= rhs.value;
            return *this;
        }

        Usage operator&(const Usage &rhs) const
        {
            return Usage(value & rhs.value);
        }

        Usage &operator&=(const Usage &rhs)
        {
            value &= rhs.value;
            return *this;
        }

        Value value{0U};
    };

    static const char *desc(Usage value);

    Vertex_attribute(
        Usage                                 usage,
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

    bool operator==(const Vertex_attribute &other) const
    {
        return (usage == other.usage) &&
            (data_type == other.data_type) &&
            (shader_type == other.shader_type) &&
            (dimension == other.dimension) &&
            (offset == other.offset) &&
            (normalized == other.normalized);
    }

    bool operator!=(const Vertex_attribute &other) const
    {
        return (usage != other.usage) ||
            (data_type != other.data_type) ||
            (shader_type != other.shader_type) ||
            (dimension != other.dimension) ||
            (offset != other.offset) ||
            (normalized != other.normalized);
    }

    Usage                                 usage{0U};
    gl::vertex_attrib_pointer_type::value data_type;
    gl::vertex_attrib_pointer_type::value shader_type;
    size_t                                index{0U};
    size_t                                dimension{0U};
    size_t                                offset{0U};
    bool                                  normalized{false};
    unsigned int                          divisor{0U};
};

} // namespace graphics
} // namespace renderstack

#endif
