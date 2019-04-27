#ifndef quad_renderer_hpp_renderers
#define quad_renderer_hpp_renderers

#include "renderstack_toolkit/platform.hpp"
#include "renderstack_toolkit/service.hpp"
#include <memory>

namespace renderstack
{
namespace graphics
{
class renderer;
class vertex_attribute_mappings;
class vertex_format;
class vertex_stream;
class buffer;
} // namespace graphics
} // namespace renderstack

class quad_renderer : public renderstack::toolkit::service
{
public:
    quad_renderer();
    /*virtual*/ ~quad_renderer();

    void connect(std::shared_ptr<renderstack::graphics::renderer> renderer);

    void initialize_service() override;

public:
    void render_minus_one_to_one();
    void render_zero_to_one();

private:
    std::shared_ptr<renderstack::graphics::renderer> m_renderer;

    std::shared_ptr<renderstack::graphics::vertex_attribute_mappings> m_vertex_attribute_mappings;
    std::shared_ptr<renderstack::graphics::vertex_format>             m_vertex_format;
    std::shared_ptr<renderstack::graphics::vertex_stream>             m_vertex_stream;
    std::shared_ptr<renderstack::graphics::buffer>                    m_vertex_buffer;
    std::shared_ptr<renderstack::graphics::buffer>                    m_index_buffer;
};

#endif
