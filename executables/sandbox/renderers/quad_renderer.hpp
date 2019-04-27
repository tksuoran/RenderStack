#ifndef quad_renderer_hpp_renderers
#define quad_renderer_hpp_renderers

#include "renderstack_graphics/vertex_attribute_mappings.hpp"
#include "renderstack_graphics/vertex_format.hpp"
#include "renderstack_graphics/vertex_stream.hpp"
#include "renderstack_toolkit/platform.hpp"
#include "renderstack_toolkit/service.hpp"
#include <memory>

namespace renderstack
{
namespace graphics
{
class Renderer;
class Buffer;
} // namespace graphics
} // namespace renderstack

class Quad_renderer : public renderstack::toolkit::service
{
public:
    Quad_renderer();

    virtual ~Quad_renderer() = default;

    void connect(std::shared_ptr<renderstack::graphics::Renderer> renderer);

    void initialize_service() override;

public:
    void render_minus_one_to_one();
    void render_zero_to_one();

private:
    std::shared_ptr<renderstack::graphics::Renderer> m_renderer;

    renderstack::graphics::Vertex_attribute_mappings m_vertex_attribute_mappings;
    renderstack::graphics::Vertex_format             m_vertex_format;
    renderstack::graphics::Vertex_stream             m_vertex_stream;
    std::shared_ptr<renderstack::graphics::Buffer>   m_vertex_buffer;
    std::shared_ptr<renderstack::graphics::Buffer>   m_index_buffer;
};

#endif
