#ifndef menulist_hpp_renderstack_ui
#define menulist_hpp_renderstack_ui

#include "renderstack_toolkit/platform.hpp"
#include "renderstack_ui/dock.hpp"
#include "renderstack_ui/ninepatch.hpp"
#include <glm/glm.hpp>
#include <memory>

namespace renderstack
{
namespace ui
{

class menulist : public dock
{
public:
    menulist(
        std::shared_ptr<class gui_renderer> renderer,
        std::shared_ptr<class style>        style,
        orientation::value                  orientation,
        area_layout_style::value            child_layout_style = area_layout_style::normal)
        : dock(renderer, style, orientation, child_layout_style), m_ninepatch(renderer, style->ninepatch_style())
    {
    }
    virtual ~menulist() {}

    void update();
    void begin_place(rectangle const &reference, glm::vec2 const &grow_direction);
    void draw_self(ui_context &context);

private:
    glm::mat4       m_background_frame;
    class ninepatch m_ninepatch;
};

} // namespace ui
} // namespace renderstack

#endif
