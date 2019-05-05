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

class Vertex_attribute_mapping
{
public:
    Vertex_attribute_mapping(const std::string &name,
                             Vertex_attribute::Usage usage,
                             unsigned int index)
        : name(name)
        , src_usage(usage)
        , dst_usage(usage)
        , src_index(index)
        , dst_index(index)
    {}

    Vertex_attribute_mapping(const std::string &name,
                             Vertex_attribute::Usage src_usage,
                             unsigned int src_index,
                             Vertex_attribute::Usage dst_usage,
                             unsigned int dst_index)
        : name(name)
        , src_usage(src_usage)
        , dst_usage(dst_usage)
        , src_index(src_index)
        , dst_index(dst_index)
    {}

    std::string                   name;
    Vertex_attribute::Usage src_usage;
    Vertex_attribute::Usage dst_usage;
    unsigned int                  src_index;
    unsigned int                  dst_index;
};

} // namespace graphics
} // namespace renderstack

#endif
