#ifndef vertex_attribute_mappings_hpp_renderstack_graphics
#define vertex_attribute_mappings_hpp_renderstack_graphics

#include "renderstack_graphics/vertex_attribute.hpp"
#include "renderstack_graphics/vertex_attribute_mapping.hpp"
#include "renderstack_toolkit/platform.hpp"

#include <map>
#include <memory>
#include <string>
#include <vector>

namespace renderstack
{
namespace graphics
{

class Buffer;
class Program;
class Vertex_format;
class Vertex_stream;

class Vertex_attribute_mappings : public std::enable_shared_from_this<Vertex_attribute_mappings>
{
public:
    Vertex_attribute_mappings() = default;

    ~Vertex_attribute_mappings() = default;

    void clear();

    void add(const std::string &name, vertex_attribute_usage::value usage, unsigned int index);

    void add(const std::string &name, vertex_attribute_usage::value usage, unsigned int src_index, unsigned int dst_index);

    void add(const std::string &name, vertex_attribute_usage::value src_usage, unsigned int src_index, vertex_attribute_usage::value dst_usage, unsigned int dst_index);

    void add_to_vertex_stream(Vertex_stream &vertex_stream,
                              Buffer        *vertex_buffer,
                              Vertex_format &vertex_format) const;

    void bind_attrib_locations(Program &program);

    std::vector<Vertex_attribute_mapping> const &mappings() const
    {
        return m_mappings;
    }

private:
    std::vector<Vertex_attribute_mapping> m_mappings;
};

} // namespace graphics
} // namespace renderstack

#endif
