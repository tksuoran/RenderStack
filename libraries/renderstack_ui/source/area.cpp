#define _CRT_SECURE_NO_WARNINGS

#include "renderstack_ui/area.hpp"
#include "renderstack_ui/gui_renderer.hpp"
#include "renderstack_ui/log.hpp"
#include <glm/gtx/string_cast.hpp>

#define LOG_CATEGORY &log_layout

#define LOG_LAYOUT 1

namespace renderstack
{
namespace ui
{

using namespace std;
using namespace glm;


Area *Area::add(Area *area)
{
    if (m_in_draw)
    {
        m_add_list.push_back(area);
    }
    else
    {
        children.push_back(area);
        area->parent = this;
    }
    return area;
}

Area *Area::remove(Area *area)
{
    if (area != nullptr)
    {
        if (m_in_draw)
        {
            m_remove_list.push_back(area);
        }
        else
        {
            children.erase(std::remove(children.begin(), children.end(), area),
                        children.end());
            area->parent = nullptr;
        }
    }
    return area;
}

Area *Area::get_hit(glm::vec2 hit_position)
{
    if (hidden)
    {
        return nullptr;
    }

    if (event_ordering == Order::self_first)
    {
        if (rect.hit(hit_position))
        {
            return this;
        }
    }

    for (auto child : children)
    {
        auto hit = child->get_hit(hit_position);
        if (hit != nullptr)
        {
            return hit;
        }
    }

    if (event_ordering == Order::post_self)
    {
        if (rect.hit(hit_position))
        {
            return this;
        }
    }

    return nullptr;
}

void Area::draw_self(ui_context &context)
{
    static_cast<void>(context);
}

void Area::draw(ui_context &context)
{
    if (hidden)
    {
        return;
    }

    m_in_draw = true;
    if (draw_ordering == Order::self_first)
    {
        draw_self(context);
    }

    for (auto child : children)
    {
        child->draw(context);
    }

    if (draw_ordering == Order::post_self)
    {
        draw_self(context);
    }

    m_in_draw = false;

    for (auto i : m_add_list)
    {
        add(i);
    }

    for (auto i : m_remove_list)
    {
        remove(i);
    }

    m_add_list.clear();
    m_remove_list.clear();
}

//  Layout

void Area::begin_size(glm::vec2 free_size_reference)
{
    const char *const name_ = name.c_str();
    slog_trace("%s Area::begin_size() reference = %f, %f\n", name_, free_size_reference.x, free_size_reference.y);

    size = fill_base_pixels + free_size_reference * fill_free_size_relative;
    size = glm::round(size);

    log_trace("%s size = %f, %f\n", name_, size.x, size.y);
}

void Area::call_size(Area *area)
{
    const char *const name_ = name.c_str();
    const char *const area_name = area ? area->name.c_str() : "(nullptr)";
    slog_trace("%s Area::call_size(%s)\n", name_, area_name);

    if (area != nullptr)
    {
        area->do_size(size);
    }
}

void Area::end_size()
{
    assert_size_valid();

    const char *const name_ = name.c_str();
    slog_trace("%s Area::end_size() size = %f, %f\n", name_, size.x, size.y);
}

// Do not make this virtual.
// Derived classes should override BeginSize() instead
glm::vec2 Area::do_size(glm::vec2 free_size_reference)
{
    const char *const name_ = name.c_str();
    slog_trace("%s Area::do_size() reference = %f, %f", name_, free_size_reference.x, free_size_reference.y);

    begin_size(free_size_reference);
    for (auto child : children)
    {
        call_size(child);
    }

    end_size();

    assert_size_valid();

    log_trace("  %s size = %f, %f\n", name_, size.x, size.y);

    return size;
}

void Area::begin_place(Rectangle reference, glm::vec2 container_grow_direction)
{
    const char *const name_ = name.c_str();
    static_cast<void>(container_grow_direction); // TODO
    slog_trace("%s Area::begin_place() reference = (%f, %f)..(%f, %f)\n",
               name_,
               reference.min().x, reference.min().y,
               reference.max().x, reference.max().y);
    log_trace("  %s size = %f, %f\n", name_, size.x, size.y);

    switch (layout_style)
    {
        case Flow_mode::normal:
        {
            break;
        }

        case Flow_mode::extend_horizontal:
        {
            size.x = reference.size().x;
            break;
        }

        case Flow_mode::extend_vertical:
        {
            size.y = reference.size().y;
            break;
        }
    }

    log_trace("  %s size = %f, %f after layout style\n", name_, size.x, size.y);

    rect.min() = reference.min() +
                 offset_pixels +
                 reference.size() * offset_free_size_relative +
                 size * offset_self_size_relative;

    rect.max() = rect.min() + size;

    rect.min() = glm::round(rect.min());
    rect.max() = glm::round(rect.max());

    if (clip_to_reference)
    {
        rect.clip_to(reference);
    }

    in_rect = rect.shrink(style.padding);
}

void Area::call_place(Area *area)
{
    const char *const name_ = name.c_str();
    const char *const area_name = area ? area->name.c_str() : "(nullptr)";
    assert(area != nullptr);

    slog_trace("%s Area::call_place(%s)\n", name_, area_name);

    area->do_place(rect, glm::vec2(1.0f, 1.0f));
}

void Area::end_place()
{
}

// Do not make this virtual.
// Derived classes should override BeginPlace() instead
glm::vec2 Area::do_place(Rectangle reference_location, glm::vec2 grow_direction)
{
    const char *const name_ = name.c_str();
    slog_trace("%s Area::do_place() reference size = %f, %f\n", name_, reference_location.size().x, reference_location.size().y);
    log_trace("  %s size = %f, %f\n", name_, size.x, size.y);

    begin_place(reference_location, grow_direction);
    for (auto child : children)
    {
        call_place(child);
    }
    //string name = area.Name != null ? area.Name : "";

    end_place();
    return size;
}


} // namespace ui
} // namespace renderstack
