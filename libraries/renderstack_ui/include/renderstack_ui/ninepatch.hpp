#ifndef ninepatch_hpp_renderstack_ui
#define ninepatch_hpp_renderstack_ui

#include "renderstack_graphics/vertex_format.hpp"
#include "renderstack_graphics/vertex_stream.hpp"
#include "renderstack_mesh/mesh.hpp"
#include "renderstack_toolkit/platform.hpp"
#include <glm/glm.hpp>
#include <memory>
#include <stdexcept>
#include <string>

namespace renderstack
{
namespace ui
{

class ninepatch_style;

class context_ninepatch;

class ninepatch
{
private:
    std::shared_ptr<ninepatch_style> m_style;
    renderstack::mesh::mesh          m_mesh;
    glm::vec2                        m_size;

public:
    std::shared_ptr<ninepatch_style> style() const
    {
        return m_style;
    }
    renderstack::mesh::mesh const &mesh() const
    {
        return m_mesh;
    }
    glm::vec2 const &size() const
    {
        return m_size;
    }

public:
    ninepatch(
        std::shared_ptr<class gui_renderer> renderer,
        std::shared_ptr<ninepatch_style>    style);

    void place(std::shared_ptr<class gui_renderer> renderer, float x0, float y0, float width, float height);
    void render(std::shared_ptr<class gui_renderer> renderer);
};

} // namespace ui
} // namespace renderstack

#endif
