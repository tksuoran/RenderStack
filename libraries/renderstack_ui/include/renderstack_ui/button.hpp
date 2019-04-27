#ifndef button_hpp_renderstack_ui
#define button_hpp_renderstack_ui

#include "renderstack_graphics/program.hpp"
#include "renderstack_graphics/renderer.hpp"
#include "renderstack_toolkit/math_util.hpp"
#include "renderstack_toolkit/platform.hpp"
#include "renderstack_ui/action.hpp"
#include "renderstack_ui/area.hpp"
#include "renderstack_ui/gui_renderer.hpp"
#include "renderstack_ui/ninepatch.hpp"
#include "renderstack_ui/text_buffer.hpp"
#include <glm/glm.hpp>

namespace renderstack
{
namespace ui
{

class Button : public Area, public Action_source
{
public:
    Button(Gui_renderer &renderer, const std::string &label, Style &style)
        : Area(renderer, style)
        , m_dirty(true)
        , m_trigger(false)
    {
        if (style.font != nullptr)
        {
            m_text_buffer = std::make_unique<Text_buffer>(renderer, *style.font);
        }
        if (style.ninepatch_style != nullptr)
        {
            m_ninepatch = std::make_unique<Ninepatch>(renderer, *style.ninepatch_style);
        }
        name = label;
        m_label = label;
    }

    virtual ~Button() = default;

    const std::string &label() const;

    void set_label(const std::string &value);

    void update_size();

    void update_place();

    void begin_size(glm::vec2 free_size_reference);

    void begin_place(Rectangle reference, glm::vec2 grow_direction);

    void draw_self(ui_context &context);

protected:
    glm::mat4 &text_frame();

    glm::mat4 &background_frame();

    Ninepatch *ninepatch();

    bool trigger() const;

    void set_trigger(bool value);

    Text_buffer *text_buffer();

private:
    glm::mat4                    m_text_frame;
    glm::mat4                    m_background_frame;
    std::unique_ptr<Text_buffer> m_text_buffer;
    std::unique_ptr<Ninepatch>   m_ninepatch;
    Rectangle                    m_bounds;
    std::string                  m_label;
    bool                         m_dirty;
    bool                         m_trigger;
};

} // namespace ui
} // namespace renderstack

#endif
