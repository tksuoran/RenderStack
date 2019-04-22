#ifndef vertex_format_hpp_renderstack_graphics
#define vertex_format_hpp_renderstack_graphics

#include "renderstack_graphics/vertex_attribute.hpp"
#include "renderstack_toolkit/platform.hpp"
#include <memory>
#include <vector>

namespace renderstack
{
namespace graphics
{

class vertex_format
{
public:
    vertex_format();
    ~vertex_format() {}

    void                              clear();
    std::shared_ptr<vertex_attribute> make_attribute(
        vertex_attribute_usage::value         usage,
        gl::vertex_attrib_pointer_type::value data_type,
        gl::vertex_attrib_pointer_type::value shader_type,
        std::size_t                           index,
        std::size_t                           dimension,
        bool                                  normalized = false);
    bool                              has_attribute(vertex_attribute_usage::value usage, unsigned int index) const;
    std::shared_ptr<vertex_attribute> find_attribute_maybe(vertex_attribute_usage::value usage, unsigned int index) const;
    std::shared_ptr<vertex_attribute> find_attribute(vertex_attribute_usage::value usage, unsigned int index) const;
    std::size_t                       stride() const
    {
        return m_stride;
    }
    bool match(vertex_format const &other) const;

private:
    std::vector<std::shared_ptr<vertex_attribute>> m_attributes;
    std::size_t                                    m_stride;
};

} // namespace graphics
} // namespace renderstack

#endif
