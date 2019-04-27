#include "renderstack_graphics/vertex_attribute_mappings.hpp"
#include "renderstack_graphics/configuration.hpp"
#include "renderstack_graphics/log.hpp"
#include "renderstack_graphics/program.hpp"
#include "renderstack_graphics/vertex_attribute_mapping.hpp"
#include "renderstack_graphics/vertex_format.hpp"
#include "renderstack_graphics/vertex_stream.hpp"
#include <stdexcept>

#define LOG_CATEGORY &log_vertex_attribute_mappings

namespace renderstack
{
namespace graphics
{

using namespace std;
using namespace renderstack::toolkit;

void Vertex_attribute_mappings::clear()
{
    m_mappings.clear();
}

void Vertex_attribute_mappings::add(const std::string &           name,
                                    vertex_attribute_usage::value usage,
                                    unsigned int                  index)
{
    m_mappings.emplace_back(name, usage, index);
}

void Vertex_attribute_mappings::add(const std::string &           name,
                                    vertex_attribute_usage::value usage,
                                    unsigned int                  src_index,
                                    unsigned int                  dst_index)
{
    m_mappings.emplace_back(name, usage, src_index, usage, dst_index);
}

void Vertex_attribute_mappings::add(const std::string &           name,
                                    vertex_attribute_usage::value src_usage,
                                    unsigned int                  src_index,
                                    vertex_attribute_usage::value dst_usage,
                                    unsigned int                  dst_index)
{
    m_mappings.emplace_back(name, src_usage, src_index, dst_usage, dst_index);
}

void Vertex_attribute_mappings::bind_attrib_locations(Program &program)
{
    for (auto &mapping : m_mappings)
    {
        program.bind_attrib_location(mapping.dst_index, mapping.name);
    }
}

void Vertex_attribute_mappings::add_to_vertex_stream(Vertex_stream &vertex_stream,
                                                     Buffer        *vertex_buffer,
                                                     Vertex_format &vertex_format) const
{
    //shared_ptr<Vertex_stream> Vertex_stream = make_shared<Vertex_stream>();
    slog_trace("Vertex_attribute_mappings::bind_attributes()\n");

    if (vertex_stream.count() != 0)
    {
        throw runtime_error("this vertex stream is already bound");
    }

    for (auto &mapping : m_mappings)
    {
        if (vertex_format.has_attribute(mapping.src_usage, mapping.src_index))
        {
            auto attribute = vertex_format.find_attribute(mapping.src_usage, mapping.src_index);
            if (attribute != nullptr)
            {
                log_trace("adding vertex attribute: name = %s, usage = %s, data_type = %s, dimension = %u, index = %u\n",
                          mapping.name.c_str(),
                          vertex_attribute_usage::desc(attribute->usage),
                          gl::enum_string(attribute->data_type),
                          static_cast<unsigned int>(attribute->dimension),
                          static_cast<unsigned int>(attribute->index));

                vertex_stream.add(vertex_buffer,
                                mapping,
                                attribute,
                                vertex_format.stride());
            }
            else
            {
                log_trace("adding vertex attribute: name = %s - not found in vertex format\n",
                          mapping.name.c_str());
            }
        }
    }
}

} // namespace graphics
} // namespace renderstack
