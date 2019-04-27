#ifndef ninepatch_hpp_renderstack_ui
#define ninepatch_hpp_renderstack_ui

#include "renderstack_graphics/vertex_format.hpp"
#include "renderstack_graphics/vertex_stream.hpp"
#include "renderstack_mesh/mesh.hpp"
#include <glm/glm.hpp>
#include <memory>
#include <stdexcept>
#include <string>

namespace renderstack
{
namespace ui
{

class Gui_renderer;
class Ninepatch_style;

class Ninepatch
{
public:
    Ninepatch(Gui_renderer &renderer, Ninepatch_style &style);

    void place(Gui_renderer &renderer, float x0, float y0, float width, float height);

    void render(Gui_renderer &renderer);

    const Ninepatch_style &style() const
    {
        return m_style;
    }

    renderstack::mesh::mesh const &mesh() const
    {
        return m_mesh;
    }

    glm::vec2 size() const
    {
        return m_size;
    }

private:
    Ninepatch_style         &m_style;
    renderstack::mesh::mesh m_mesh;
    glm::vec2               m_size;
};

} // namespace ui
} // namespace renderstack

#endif
