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

class Menulist : public Dock
{
public:
    Menulist(Gui_renderer &renderer,
             Style        &style,
             Orientation  orientation,
             Flow_mode    child_layout_style = Flow_mode::normal)
        : Dock(renderer, style, orientation, child_layout_style)
    {
        if (style.ninepatch_style != nullptr)
        {
            m_ninepatch = std::make_unique<Ninepatch>(renderer, *style.ninepatch_style);
        }
    }

    virtual ~Menulist() = default;

    void update();

    void begin_place(Rectangle reference, glm::vec2 grow_direction) override;

    void draw_self(ui_context &context) override;

private:
    glm::mat4                  m_background_frame;
    std::unique_ptr<Ninepatch> m_ninepatch;
};

} // namespace ui
} // namespace renderstack

#endif
