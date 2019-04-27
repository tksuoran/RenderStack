#include "renderstack_graphics/vertex_format.hpp"
#include <algorithm>
#include <stdexcept>
#include <cassert>

namespace renderstack
{
namespace graphics
{

using namespace std;


void Vertex_format::clear()
{
    m_attributes.clear();
    m_stride = 0U;
}

Vertex_attribute &Vertex_format::make_attribute(
    vertex_attribute_usage::value         usage,
    gl::vertex_attrib_pointer_type::value data_type,
    gl::vertex_attrib_pointer_type::value shader_type,
    size_t                                index,
    size_t                                dimension,
    bool                                  normalized)
{
    size_t stride = dimension * size_of_type(data_type);

    // Align attributes to their type
    switch (stride)
    {
        case 1:
        {
            break;
        }
        case 2:
        {
            while (m_stride & 1)
            {
                ++m_stride;
            }
            break;
        }
        default:
        case 4:
        {
            while (m_stride & 3)
            {
                ++m_stride;
            }
            break;
        }
    }

    auto &attribute = m_attributes.emplace_back(usage,
                                                data_type,
                                                shader_type,
                                                index,
                                                dimension,
                                                m_stride,
                                                normalized);

    assert(stride == attribute.stride());
    m_stride += stride;
    return attribute;
}

bool Vertex_format::match(Vertex_format const &other) const
{
    if (m_attributes.size() != other.m_attributes.size())
    {
        return false;
    }

    for (size_t i = 0; i < m_attributes.size(); ++i)
    {
        if (m_attributes[i] != other.m_attributes[i])
        {
            return false;
        }
    }

    return true;
}

bool Vertex_format::has_attribute(vertex_attribute_usage::value usage, unsigned int index) const
{
    for (auto &i : m_attributes)
    {
        if ((i.usage == usage) && (i.index == index))
        {
            return true;
        }
    }

    return false;
}

const Vertex_attribute *Vertex_format::find_attribute_maybe(vertex_attribute_usage::value usage, unsigned int index) const
{
    for (auto &i : m_attributes)
    {
        if ((i.usage == usage) && (i.index == index))
        {
            return &(i);
        }
    }

    return nullptr;
}

const Vertex_attribute *Vertex_format::find_attribute(vertex_attribute_usage::value usage, unsigned int index) const
{
    for (auto &i : m_attributes)
    {
        if ((i.usage == usage) && (i.index == index))
        {
            return &(i);
        }
    }

    throw runtime_error("vertex_attribute not found");
}

} // namespace graphics
} // namespace renderstack
