#ifndef vertex_format_hpp_renderstack_graphics
#define vertex_format_hpp_renderstack_graphics

#include "renderstack_graphics/vertex_attribute.hpp"
#include <memory>
#include <vector>

namespace renderstack
{
namespace graphics
{

class Vertex_format
{
public:
    Vertex_format() = default;

    ~Vertex_format() = default;

    void clear();

    Vertex_attribute &make_attribute(
        vertex_attribute_usage::value         usage,
        gl::vertex_attrib_pointer_type::value data_type,
        gl::vertex_attrib_pointer_type::value shader_type,
        size_t                                index,
        size_t                                dimension,
        bool                                  normalized = false);

    bool has_attribute(vertex_attribute_usage::value usage, unsigned int index) const;

    const Vertex_attribute *find_attribute_maybe(vertex_attribute_usage::value usage, unsigned int index) const;

    const Vertex_attribute *find_attribute(vertex_attribute_usage::value usage, unsigned int index) const;

    size_t stride() const
    {
        return m_stride;
    }

    bool match(Vertex_format const &other) const;

private:
    std::vector<Vertex_attribute> m_attributes;
    size_t                        m_stride{0U};
};

} // namespace graphics
} // namespace renderstack

#endif
