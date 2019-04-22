#ifndef vertex_attribute_mapping_hpp_renderstack_graphics
#define vertex_attribute_mapping_hpp_renderstack_graphics

#include "renderstack_graphics/vertex_attribute.hpp"
#include "renderstack_toolkit/platform.hpp"

#include <map>
#include <memory>
#include <string>
#include <vector>

namespace renderstack
{
namespace graphics
{

class vertex_attribute_mapping
{
public:
    vertex_attribute_mapping(std::string const &name, vertex_attribute_usage::value usage, unsigned int index);
    vertex_attribute_mapping(std::string const &name, vertex_attribute_usage::value src_usage, unsigned int src_index, vertex_attribute_usage::value dst_usage, unsigned int dst_index);

    std::string const &           name() const;
    vertex_attribute_usage::value src_usage() const;
    vertex_attribute_usage::value dst_usage() const;
    unsigned int                  src_index() const;
    unsigned int                  dst_index() const;

private:
    std::string                   m_name;
    vertex_attribute_usage::value m_src_usage;
    vertex_attribute_usage::value m_dst_usage;
    unsigned int                  m_src_index;
    unsigned int                  m_dst_index;
};

} // namespace graphics
} // namespace renderstack

#endif
