#ifndef glyph_hpp_renderstack_ui
#define glyph_hpp_renderstack_ui

#include "RectangleBinPack/Rect.h"
#include "renderstack_toolkit/platform.hpp"
#include "renderstack_ui/rectangle.hpp"
#include <ft2build.h>
#include <string>
#include <vector>
#include FT_FREETYPE_H
#include FT_STROKER_H

namespace renderstack
{
namespace ui
{

class Glyph
{
public:
    Glyph(FT_Library    library,
          FT_Face       font_face,
          unsigned char c,
          float         bolding,
          float         outline_thickness,
          bool          rgb,
          int           hint_mode);

    Rect rect() const
    {
        return m_rect;
    }

    void set_rect(Rect const &value)
    {
        m_rect = value;
    }

    std::vector<unsigned char> const &buffer() const
    {
        return m_buffer;
    }

    void dump() const;

    unsigned int glyph_index() const
    {
        return m_glyph_index;
    }

    float bearingX() const
    {
        return m_bearingX;
    }

    float width() const
    {
        return m_width;
    }

    float bearingY() const
    {
        return m_bearingY;
    }

    float height() const
    {
        return m_height;
    }

    float horiAdvance() const
    {
        return m_horiAdvance;
    }

    int bm_width() const
    {
        return m_bm_width;
    }

    int bm_height() const
    {
        return m_bm_height;
    }

    int bm_pitch() const
    {
        return m_bm_pitch;
    }

    int x0() const
    {
        return m_x0;
    }

    int y0() const
    {
        return m_y0;
    }

    int xadvance() const
    {
        return m_xadvance;
    }

    int crop_x_min() const
    {
        return m_crop_x_min;
    }

    int crop_y_min() const
    {
        return m_crop_y_min;
    }

    int crop_x_max() const
    {
        return m_crop_x_max;
    }

    int crop_y_max() const
    {
        return m_crop_y_max;
    }

    int cropped_width() const
    {
        int cw = m_bm_width - m_crop_x_min - m_crop_x_max;
        return cw;
    }

    int cropped_height() const
    {
        int ch = m_bm_height - m_crop_y_min - m_crop_y_max;
        return ch;
    }

private:
    void validate(FT_Error error);

private:
    Rect m_rect;
    unsigned int m_glyph_index;
    float m_outline_thickness;
    float m_bearingX;
    float m_width;
    float m_bearingY;
    float m_height;
    float m_horiAdvance;
    int m_bm_width;
    int m_bm_height;
    int m_bm_pitch;
    int m_x0;
    int m_y0;
    int m_crop_x_min;
    int m_crop_y_min;
    int m_crop_x_max;
    int m_crop_y_max;
    int m_xadvance;
    std::vector<unsigned char> m_buffer;
};

} // namespace ui
} // namespace renderstack

#endif
