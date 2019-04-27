#include "renderstack_ui/button.hpp"
#include "renderstack_ui/context.hpp"
#include "renderstack_ui/gui_renderer.hpp"
#include "renderstack_ui/log.hpp"
#include "renderstack_ui/ninepatch_style.hpp"

#define LOG_CATEGORY &log_button

namespace renderstack
{
namespace ui
{

using namespace renderstack::toolkit;
using namespace glm;
using namespace std;

const std::string &Button::label() const
{
    return m_label;
}

void Button::set_label(const std::string &value)
{
    if (value != m_label)
    {
        m_dirty = true;
        m_label = value;
    }
}

mat4 &Button::text_frame()
{
    return m_text_frame;
}

mat4 &Button::background_frame()
{
    return m_background_frame;
}

Ninepatch *Button::ninepatch()
{
    return m_ninepatch.get();
}

bool Button::trigger() const
{
    return m_trigger;
}

void Button::set_trigger(bool value)
{
    m_trigger = value;
}

Text_buffer *Button::text_buffer()
{
    return m_text_buffer.get();
}

void Button::update_size()
{
    slog_trace("Button::update_size()");

    if (m_dirty)
    {
        if (m_text_buffer)
        {
            m_text_buffer->begin_print();
            m_text_buffer->print(m_label, 0, 0);
            m_text_buffer->end_print();
            m_bounds = m_text_buffer->bounding_box();

            fill_base_pixels = m_bounds.max() + 2.0f * style.padding;
        }
        else
        {
            fill_base_pixels = vec2(30.0f, 10.0f);
        }

        if (m_ninepatch)
        {
            m_ninepatch->place(renderer, 0.0f, 0.0f, fill_base_pixels.x, fill_base_pixels.y);
        }
        m_dirty = false;
    }
}

void Button::update_place()
{
    slog_trace("Button::update_place()");

    if (m_ninepatch)
    {
        if (size.x != m_bounds.max().x + 2.0f * style.padding.x)
        {
            m_ninepatch->place(renderer, 0.0f, 0.0f, size.x, size.y);
        }
    }
}

void Button::begin_size(vec2 free_size_reference)
{
    update_size();
    Area::begin_size(free_size_reference);
}

void Button::begin_place(Rectangle reference, vec2 grow_direction)
{
    Area::begin_place(reference, grow_direction);
    update_place();
    mat4 a;
    mat4 b;
    create_translation(rect.min() + style.padding, a);
    create_translation(rect.min(), b);

    mat4 o = renderer.ortho();
    m_text_frame       = o * a;
    m_background_frame = o * b;
}

void Button::draw_self(ui_context &context)
{
    slog_trace("Button::draw_self()");

    auto &gr = renderer;

    //r->push();

    assert(style.ninepatch_style != nullptr);
    gr.set_program(style.ninepatch_style->program);

    auto &r = gr.renderer();
    {
        auto t = style.ninepatch_style->texture.get();
        r.set_texture(style.ninepatch_style->texture_unit, t);
        //t->apply(rr, style()->ninepatch_style()->texture_unit());
    }

    gr.set_transform(m_background_frame);
    gr.set_color_scale(vec4(1.0f, 1.0f, 1.0f, 1.0f));
    if (rect.hit(context.mouse))
    {
        if (context.mouse_buttons[0])
        {
            gr.set_color_add(vec4(0.2f, 0.35f, 0.45f, 0.0f));
            m_trigger = true;
        }
        else
        {
            if (trigger())
            {
                if (sink())
                {
                    sink()->action(this);
                }
                set_trigger(false);
            }
            gr.set_color_add(vec4(0.1f, 0.2f, 0.35f, 0.0f));
        }
    }
    else
    {
        set_trigger(false);
        gr.set_color_add(vec4(0.0f, 0.0f, 0.0f, 0.0f));
    }

    if (m_ninepatch)
    {
        m_ninepatch->render(gr);
    }

    // Then draw text
    if (m_text_buffer)
    {
        gr.blend_alpha();
        gr.set_program(style.program);
        auto t = style.font->texture();
        gr.set_texture(style.texture_unit, t);
        t->apply(r, style.texture_unit);

        // TODO Font shader does not currently support add/scale
        //gr->set_color_add  (glm::vec4(0.00f, 0.00f, 0.00f, 0.0f));
        gr.set_color_scale(vec4(0.72f, 0.72f, 0.72f, 2.0f));
        gr.set_transform(m_text_frame);
        m_text_buffer->render();
        gr.blend_disable();
    }
    //r->pop();
}

} // namespace ui
} // namespace renderstack
