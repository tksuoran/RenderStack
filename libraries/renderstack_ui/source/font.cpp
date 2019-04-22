#if defined(RENDERSTACK_USE_FREETYPE)
#    include <ft2build.h>
#    include FT_FREETYPE_H
#    include FT_STROKER_H
#    include FT_GLYPH_H
#    include FT_BITMAP_H
#endif

#include "renderstack_ui/font.hpp"
#if defined(RENDERSTACK_USE_FREETYPE)
#    include "renderstack_ui/glyph.hpp"
#endif
#include "RectangleBinPack/SkylineBinPack.h"
#include "renderstack_graphics/configuration.hpp"
#include "renderstack_graphics/renderer.hpp"
#include "renderstack_graphics/texture.hpp"
#include "renderstack_toolkit/gl.hpp"
#include "renderstack_toolkit/strong_gl_enums.hpp"
#include "renderstack_ui/bitmap.hpp"
#include "renderstack_ui/log.hpp"
#include <map>
#include <memory>
#include <stdexcept>

#define LOG_CATEGORY &log_font

// #define LOG

namespace renderstack
{
namespace ui
{

using namespace gl;
using namespace std;
using namespace renderstack::graphics;
/*
font::font()
{
   glGenTextures(1, &m_texture_object);
   glBindTexture(GL_TEXTURE_2D, m_texture_object);
   m_chars             = "";
   m_path              = "";
   m_hinting           = false;
   m_regular_grid      = false;
   m_rgb               = false;
   m_dpi               = 0;
   m_gamma             = 0.0f;
   m_saturation        = 0.0f;;
   m_pixel_size        = 0;
   m_bolding           = 0.0f;
   m_outline_thickness = 0.0f;
   m_spacing_delta     = 0;
   m_texture_width     = 0;
   m_texture_height    = 0;
   m_line_height       = 0.0f;
   m_bitmap = make_shared<bitmap>(m_texture_width, m_texture_height, 2);
   //m_bitmap->load_data(nullptr);
   post_process();
}*/

font::~font()
{
}

font::font(
    renderstack::graphics::renderer &renderer,
    string const &                   path,
    unsigned int                     size,
    float                            outline_thickness)
    : m_path(path), m_hinting(true), m_regular_grid(false), m_rgb(false), m_dpi(96), m_gamma(1.0f), m_saturation(1.0f), m_bolding((size > 10) ? 0.5f : 0.0f), m_outline_thickness(outline_thickness), m_spacing_delta(0)
{
    slog_trace("font::font(path = %s, size = %u, outline_thickness = %f)", path.c_str(), size, outline_thickness);

    if (m_hinting)
        //m_hint_mode = FT_LOAD_FORCE_AUTOHINT | FT_LOAD_TARGET_LIGHT;
        //m_hint_mode = FT_LOAD_FORCE_AUTOHINT;
        m_hint_mode = 0;
#if defined(RENDERSTACK_USE_FREETYPE)
    else
        m_hint_mode = FT_LOAD_NO_HINTING;
#endif

#if 0
   m_chars = "volumetric";
#else
    m_chars =
        " !\"#$%&'()*+,-./" // latin
        "0123456789:;<=>?"
        "@ABCDEFGHIJKLMNO"
        "PQRSTUVWXYZ[\\]^_"
        "&`abcdefghijklmno"
        "pqrstuvwxyz{|}~";
#endif

    m_pixel_size = size;

#if defined(RENDERSTACK_USE_FREETYPE)
    render(renderer);
#endif
}
#if defined(RENDERSTACK_USE_FREETYPE)
void font::validate(FT_Error error)
{
    if (error == FT_Err_Unknown_File_Format)
        log_error("font::validate(): FT_Err_Unknown_File_Format");

    if (error)
        throw runtime_error("freetype error");
}
void font::render(renderstack::graphics::renderer &renderer)
{
    slog_trace("font::render()");

    FT_Library library;
    FT_Face    face;

    validate(FT_Init_FreeType(&library));
    validate(FT_New_Face(library, m_path.c_str(), 0, &face));
    validate(FT_Select_Charmap(face, FT_ENCODING_UNICODE));

    try
    {

        FT_F26Dot6 xsize = m_pixel_size << 6;
        FT_F26Dot6 ysize = m_pixel_size << 6;
        if (m_rgb)
            xsize *= 3;

        validate(FT_Set_Char_Size(face, xsize, ysize, m_dpi, m_dpi));

#    if defined(LOG)
        log() << "num faces     " << face->num_faces << '\n';
        log() << "face index    " << face->face_index << '\n';
        log() << "face flags    ";
        if (face->face_flags & FT_FACE_FLAG_SCALABLE)
            log() << "scalable ";
        if (face->face_flags & FT_FACE_FLAG_FIXED_WIDTH)
            log() << "fixed_width ";
        if (face->face_flags & FT_FACE_FLAG_SFNT)
            log() << "sfnt ";
        if (face->face_flags & FT_FACE_FLAG_HORIZONTAL)
            log() << "horizontal ";
        if (face->face_flags & FT_FACE_FLAG_VERTICAL)
            log() << "vertical ";
        if (face->face_flags & FT_FACE_FLAG_KERNING)
            log() << "kerning ";
        if (face->face_flags & FT_FACE_FLAG_FAST_GLYPHS)
            log() << "fast_glyphs ";
        if (face->face_flags & FT_FACE_FLAG_MULTIPLE_MASTERS)
            log() << "multiple_masters ";
        if (face->face_flags & FT_FACE_FLAG_GLYPH_NAMES)
            log() << "glyph_names ";
        if (face->face_flags & FT_FACE_FLAG_EXTERNAL_STREAM)
            log() << "external_stream ";
        if (face->face_flags & FT_FACE_FLAG_HINTER)
            log() << "hinter ";
        if (face->face_flags & FT_FACE_FLAG_CID_KEYED)
            log() << "cid_keyed ";
        if (face->face_flags & FT_FACE_FLAG_TRICKY)
            log() << "tricky ";
        log() << '\n';
        log().flush();
        log() << "style flags   ";
        if (->style_flags & FT_STYLE_FLAG_ITALIC)
            log() << "italic ";
        if (->style_flags & FT_STYLE_FLAG_BOLD)
            log() << "bold ";
        log() << '\n';
        log().flush();
        log() << "num glyphs    " << face->num_glyphs << '\n';
        log() << "family name   " << face->family_name << '\n';
        log() << "style name    " << face->style_name << '\n';
        log() << "fixed sizes   " << face->num_fixed_sizes << ": ";
        for (int i = 0; i < face->num_fixed_sizes; ++i)
            log() << face->available_sizes[i].width << " * " << face->available_sizes[i].height << ", ";

        log() << '\n';
        log().flush();
        log() << "num charmaps  " << face->num_charmaps << '\n';
        log() << "units per em  " << face->units_per_EM << '\n';
        log() << "ascender      " << face->ascender << '\n';
        log() << "descender     " << face->descender << '\n';
        log() << "height        " << face->height << '\n';
        log() << "max a. width  " << face->max_advance_width << '\n';
        log() << "max a. height " << face->max_advance_height << '\n';
        log() << "underline pos " << face->underline_position << '\n';
        log() << "underline thk " << face->underline_thickness << '\n';
        log() << "charmap       ";
        switch (face->charmap->encoding)
        {
            case FT_ENCODING_NONE: log() << "none\n"; break;
            case FT_ENCODING_MS_SYMBOL: log() << "MS Symbol\n"; break;
            case FT_ENCODING_UNICODE: log() << "Unicode\n"; break;
            case FT_ENCODING_SJIS: log() << "SJIS\n"; break;
            case FT_ENCODING_GB2312: log() << "GB2312 Simplified Chinese\n"; break;
            case FT_ENCODING_BIG5: log() << "BIG5 Traditional Chinese\n"; break;
            case FT_ENCODING_WANSUNG: log() << "WANSUNG\n"; break;               // Korean
            case FT_ENCODING_JOHAB: log() << "JOHAB KS C 5601-1992\n"; break;    // Korean, MS Windows code page 1361
            case FT_ENCODING_ADOBE_STANDARD: log() << "Adobe Standard\n"; break; // Type 1, CFF, and OpenType/CFF (256)
            case FT_ENCODING_ADOBE_EXPERT: log() << "Adobe Expert\n"; break;     // Type 1, CFF, and OpenType/CFF (256)
            case FT_ENCODING_ADOBE_CUSTOM: log() << "Adobe Custom\n"; break;     // Type 1, CFF, and OpenType/CFF (256)
            case FT_ENCODING_ADOBE_LATIN_1: log() << "Adobe Latin 1\n"; break;   // Type 1 PostScript (256)
            case FT_ENCODING_OLD_LATIN_2: log() << "Old Latin 2\n"; break;
            case FT_ENCODING_APPLE_ROMAN: log() << "Apple Roman\n"; break;
            default: log() << "unknown\n"; break;
        }
        log().flush();

        log() << "patents       ";
        ;
        log().flush();
        FT_Bool res = FT_Face_CheckTrueTypePatents(face);
        log() << ((res == 1) ? "yes\n" : "no\n");
        log().flush();
#    endif

        m_line_height = std::ceil(static_cast<float>(face->size->metrics.height) / 64.0f);

        map<char, shared_ptr<glyph>> glyphs;
        map<char, shared_ptr<glyph>> outline_glyphs;

        int max_bm_size = 0;

        //  First pass: Render glyphs to bitmaps, Find max size
        for (auto i = m_chars.cbegin(); i != m_chars.cend(); ++i)
        {
            char          c  = *i;
            unsigned char uc = static_cast<unsigned char>(c);
            try
            {
                auto g = glyphs[c] = make_shared<glyph>(library, face, uc, m_bolding, 0.0f, m_rgb, m_hint_mode);
                if (g->bm_width() > max_bm_size)
                    max_bm_size = g->bm_width();
                if (g->bm_height() > max_bm_size)
                    max_bm_size = g->bm_height();

                if (m_outline_thickness > 0.0f)
                {
                    auto og = outline_glyphs[c] = make_shared<glyph>(library, face, uc, m_bolding, m_outline_thickness, m_rgb, m_hint_mode);
                    if (og->bm_width() > max_bm_size)
                        max_bm_size = og->bm_width();
                    if (og->bm_height() > max_bm_size)
                        max_bm_size = og->bm_height();
                }
            }
            catch (...)
            {
                continue;
            }
        }

        //  Second pass: Search pack resolution
        SkylineBinPack packer;
        m_texture_width  = 4;
        m_texture_height = 4;
        for (;;)
        {
            //  Reserve 1 pixel border
            packer.Init(m_texture_width - 2, m_texture_height - 2, false);

            bool pack_failed = false;
            for (auto i = m_chars.cbegin(); i != m_chars.cend(); ++i)
            {
                char c  = *i;
                auto g  = glyphs[c];
                auto og = outline_glyphs[c];
                if (g == nullptr)
                    continue;

                int w = g->bm_width();
                int h = g->bm_height();
                if (og != nullptr)
                {
                    w = std::max(w, og->bm_width());
                    h = std::max(h, og->bm_height());
                }
                if (m_regular_grid)
                {
                    w = max_bm_size;
                    h = max_bm_size;
                }
                if (w == 0 || h == 0)
                {
                    Rect r;
                    r.x = r.y = r.width = r.height = 0;
                    g->set_rect(r);
                    continue;
                }

                Rect r = packer.Insert(w + 1, h + 1, SkylineBinPack::LevelBottomLeft);
                if (r.width == 0 || r.height == 0)
                {
                    pack_failed = true;
                    break;
                }
                g->set_rect(r);
            }

            if (pack_failed == true)
            {
                if (m_texture_width <= m_texture_height)
                    m_texture_width *= 2;
                else
                    m_texture_height *= 2;

                if (m_texture_width >= 16384)
                    throw runtime_error("unable to pack glyphs to texture");
            }
            else
            {
                break;
            }
        }
#    if defined(LOG)
        log() << "packing glyps to " << m_texture_width << " x " << m_texture_height << " succeeded\n";
        log().flush();
#    endif

        // Third pass: render glyphs
        m_bitmap = make_shared<bitmap>(m_texture_width, m_texture_height, m_rgb ? 3 : 2);
        m_bitmap->fill(0);
        for (auto i = m_chars.cbegin(); i != m_chars.cend(); ++i)
        {
            char          c  = *i;
            unsigned char uc = static_cast<unsigned char>(c);
            try
            {
                auto g  = glyphs[c];
                auto og = outline_glyphs[c];
                if (g == nullptr)
                    continue;

                bool render = (g->width() != 0) && (g->height() != 0);

                Rect const &r = g->rect();
#    if defined(LOG)
                log() << "char " << c << " blit size " << face->glyph->bitmap.width << " * " << face->glyph->bitmap.rows << '\n';
                log().flush();
#    endif

                ft_char d;
                d.width    = static_cast<unsigned short>(g->bm_width());
                d.height   = static_cast<unsigned short>(g->bm_height());
                d.xadvance = static_cast<short>(g->xadvance());
                d.xoffset  = static_cast<short>(g->x0());
                d.yoffset  = static_cast<short>(g->y0());

                bool  rotated = (r.width != r.height) && ((d.width + 1) == r.height) && ((d.height + 1) == r.width);
                short x_extra = 0;
                short y_extra = 0;
                //float x_delta   = 0.0f;
                //float y_delta   = 0.0f;
                //float x_f_extra = 0.0f;
                //float y_f_extra = 0.0f;
                if (og)
                {
                    // g is glyph and og is the outline glyph
                    d.width   = static_cast<unsigned short>(std::max(g->bm_width(), og->bm_width()));
                    d.height  = static_cast<unsigned short>(std::max(g->bm_height(), og->bm_height()));
                    rotated   = (r.width != r.height) && ((d.width + 1) == r.height) && ((d.height + 1) == r.width);
                    d.xoffset = static_cast<short>((std::min)(g->x0(), og->x0()));
                    d.yoffset = static_cast<short>((std::min)(g->y0(), og->y0()));
                    if (!rotated)
                    {
                        x_extra = static_cast<short>((og->crop_x_min() - g->crop_x_min()) + (og->cropped_width() - g->cropped_width()) / 2);
                        y_extra = static_cast<short>((og->crop_y_max() - g->crop_y_max()) + (og->cropped_height() - g->cropped_height()) / 2);
                    }
                    else
                    {
                        x_extra = static_cast<short>((og->crop_y_min() - g->crop_y_min()) + (og->cropped_height() - g->cropped_height()) / 2);
                        y_extra = static_cast<short>((og->crop_x_min() - g->crop_x_min()) + (og->cropped_width() - g->cropped_width()) / 2);
                    }
                }

#    if defined(LOG)
                if (og)
                {
                    //int dch = og->cropped_height() - g ->cropped_height();
                    //int dy0 = g->y0() - og->y0();
                    //int dymin = og->crop_y_min() - g->crop_y_min();
                    log() << c << " ch: " << og->cropped_height() << " : " << g->cropped_height() << " y0: " << og->y0() << " : " << g->y0() << " y min: " << og->crop_y_min() << " : " << g->crop_y_min() << " y max: " << og->crop_y_max() << " : " << g->crop_y_max() << " extra " << y_extra << '\n';
                    log() << c << " ch: " << og->cropped_width() << " : " << g->cropped_width() << " x0: " << og->x0() << " : " << g->x0() << " x min: " << og->crop_x_min() << " : " << g->crop_x_min() << " x max: " << og->crop_x_max() << " : " << g->crop_x_max() << " extra " << x_extra << '\n';
                    log().flush();
                }
#    endif

                float x_scale = 1.0f / ((float)(m_texture_width));
                float y_scale = 1.0f / ((float)(m_texture_height));
                float fx      = (float)r.x + 1.0f;
                float fy      = (float)r.y + 1.0f;
                float fw      = (float)d.width;
                float fh      = (float)d.height;

                if (!rotated)
                {
                    d.u[0] = (fx)*x_scale;
                    d.v[0] = (fy)*y_scale;
                    d.u[1] = (fx + fw) * x_scale;
                    d.v[1] = (fy)*y_scale;
                    d.u[2] = (fx + fw) * x_scale;
                    d.v[2] = (fy + fh) * y_scale;
                    d.u[3] = (fx)*x_scale;
                    d.v[3] = (fy + fh) * y_scale;
                }
                else
                {
                    d.u[0] = (fx + fh) * x_scale;
                    d.v[0] = (fy)*y_scale;
                    d.u[1] = (fx + fh) * x_scale;
                    d.v[1] = (fy + fw) * y_scale;
                    d.u[2] = (fx)*x_scale;
                    d.v[2] = (fy + fw) * y_scale;
                    d.u[3] = (fx)*x_scale;
                    d.v[3] = (fy)*y_scale;
                }
                if (render)
                {
                    m_bitmap->blit(
                        g->bm_width(),
                        g->bm_height(),
                        r.x + 1 + x_extra,
                        r.y + 1 + y_extra,
                        g->buffer(),
                        g->bm_pitch(),
                        g->bm_width(),
                        m_rgb ? 3 : 1,
                        0,
                        rotated);
                    if (og)
                    {
                        m_bitmap->blit(
                            og->bm_width(),
                            og->bm_height(),
                            r.x + 1,
                            r.y + 1,
                            og->buffer(),
                            og->bm_pitch(),
                            og->bm_width(),
                            1,
                            1,
                            rotated);
                    }
                }
                m_chars_256[uc] = d;

                // Kerning
                for (auto j = m_chars.cbegin(); j != m_chars.cend(); ++j)
                {
                    char    c2           = *j;
                    FT_UInt glyph_index2 = FT_Get_Char_Index(face, static_cast<FT_ULong>(c2));
                    if (glyph_index2 == 0)
                        continue;

                    FT_Vector k;
                    validate(FT_Get_Kerning(face, g->glyph_index(), glyph_index2, FT_KERNING_DEFAULT, &k));
                    if (k.x != 0)
                    {
                        float kx = k.x / 64.0f;
                        if (m_rgb)
                            kx /= 3.0f;

                        kx             = std::ceil(kx);
                        short   amount = static_cast<short>(kx);
                        kerning k2(static_cast<unsigned short>(c2), amount);
                        d.kernings.push_back(k2);
                    }
                }
            }
            catch (...)
            {
#    if defined(LOG)
                log() << "char " << c << " could not be processed\n";
#    endif
            }
        }
        post_process(renderer);
    }
    catch (...)
    {
    }
    validate(FT_Done_Face(face));
    validate(FT_Done_FreeType(library));
}
#endif

void font::post_process(renderstack::graphics::renderer &renderer)
{
    slog_trace("font::post_process()");

    bitmap bm(m_bitmap->width(), m_bitmap->height(), m_bitmap->components());
    m_bitmap->post_process(bm, m_gamma);

    gl::pixel_internal_format::value i;
    gl::pixel_format::value          f;

    if (renderstack::graphics::configuration::can_use.texture_rg)
    {
        i = m_rgb ? gl::pixel_internal_format::rgb8 : gl::pixel_internal_format::rg8;
        f = m_rgb ? gl::pixel_format::rgb : gl::pixel_format::rg;
    }
    else
    {
        i = m_rgb ? gl::pixel_internal_format::rgb : gl::pixel_internal_format::luminance_alpha;
        f = m_rgb ? gl::pixel_format::rgb : gl::pixel_format::luminance_alpha;
    }

    m_texture = make_shared<renderstack::graphics::texture>(
        renderstack::graphics::texture_target::texture_2d,
        i,
        false,
        m_texture_width,
        m_texture_height);
    m_texture->allocate_storage(renderer);
    m_texture->set_debug_label("font");
    unsigned int old_unit;
    auto         old_texture = renderer.set_texture(0, m_texture, &old_unit);

    gl::tex_sub_image_2d(
        gl::texture_target::texture_2d,
        0,
        0,
        0,
        m_bitmap->width(),
        m_bitmap->height(),
        f,
        gl::pixel_type::unsigned_byte,
        bm.ptr());

    m_texture->set_min_filter(texture_min_filter::nearest);
    m_texture->set_mag_filter(texture_mag_filter::nearest);

    m_texture->apply(renderer, 0);

    renderer.restore_texture(texture_target::texture_2d, old_texture, old_unit);
}

void font::save() const
{
    slog_trace("font::save()");

    printf("   glGenTextures(1, &m_texture_object);\n");
    printf("   glBindTexture(GL_TEXTURE_2D, m_texture_object);\n");
    printf("   m_chars             = \"%s\";\n", m_chars.c_str());
    printf("   m_path              = \"%s\";\n", m_path.c_str());
    printf("   m_hinting           = %s;\n", m_hinting ? "true" : "false");
    printf("   m_regular_grid      = %s;\n", m_regular_grid ? "true" : "false");
    printf("   m_rgb               = %s;\n", m_rgb ? "true" : "false");
    printf("   m_dpi               = %d;\n", m_dpi);
    printf("   m_gamma             = %ff;\n", m_gamma);
    printf("   m_saturation        = %ff;\n", m_saturation);
    printf("   m_pixel_size        = %d;\n", m_pixel_size);
    printf("   m_bolding           = %ff;\n", m_bolding);
    printf("   m_outline_thickness = %ff;\n", m_outline_thickness);
    printf("   m_spacing_delta     = %d;\n", m_spacing_delta);
    printf("   m_texture_width     = %d;\n", m_texture_width);
    printf("   m_texture_height    = %d;\n", m_texture_height);
    printf("   m_line_height       = %ff;\n", m_line_height);
    for (size_t i = 0; i < 256; ++i)
    {
        ft_char const &c = m_chars_256[i];
        if ((c.width > 0 && c.height > 0) || (c.xadvance > 0) || (c.kernings.size() > 0))
        {
            printf("   m_chars_256[%lu].width    = %u;\n", i, c.width);
            printf("   m_chars_256[%lu].height   = %u;\n", i, c.height);
            printf("   m_chars_256[%lu].xoffset  = %d;\n", i, c.xoffset);
            printf("   m_chars_256[%lu].yoffset  = %d;\n", i, c.yoffset);
            printf("   m_chars_256[%lu].xadvance = %d;\n", i, c.xadvance);
            printf("   m_chars_256[%lu].u[0] = %ff; "
                   "m_chars_256[%lu].u[1] = %ff; "
                   "m_chars_256[%lu].u[2] = %ff; "
                   "m_chars_256[%lu].u[3] = %ff;\n",
                   i, c.u[0], i, c.u[1], i, c.u[2], i, c.u[3]);
            printf("   m_chars_256[%lu].v[0] = %ff; "
                   "m_chars_256[%lu].v[1] = %ff; "
                   "m_chars_256[%lu].v[2] = %ff; "
                   "m_chars_256[%lu].v[3] = %ff;\n",
                   i, c.v[0], i, c.v[1], i, c.v[2], i, c.v[3]);
            for (size_t j = 0; j < c.kernings.size(); ++j)
            {
                kerning const &k = c.kernings[i];
                printf("   m_chars[%lu].push_back(kerning(%d, %d));\n", j, k.second, k.amount);
            }
        }
    }
    printf("   m_bitmap = make_shared<bitmap>(%d, %d, %d);\n",
           m_bitmap->width(), m_bitmap->height(), m_bitmap->components());
    printf("   static const unsigned char data[] = {\n");
    m_bitmap->dump_data();
    printf("0};\n");
    printf("   m_bitmap->load_data(&data[0]);\n");
    printf("   post_process();\n");
}
size_t font::print(string const &text, rectangle &bounds, float *&ptr, float x, float y, std::size_t max_chars) const
{
    slog_trace("font::print(ptr = %p, text = %s, x = % 7.2, y = % 7.2)",
               ptr,
               text.c_str(),
               x,
               y);

    if (text.size() == 0)
        return 0;

    size_t chars_printed = 0;
    for (size_t i = 0; i < text.size(); ++i)
    {
        char          c  = text[i];
        unsigned char uc = static_cast<unsigned char>(c);

        ft_char const &font_char = m_chars_256[uc];

        float a = static_cast<float>(font_char.xadvance);

        if (font_char.width != 0)
        {
            float w  = static_cast<float>(font_char.width);
            float h  = static_cast<float>(font_char.height);
            float ox = static_cast<float>(font_char.xoffset);
            float oy = static_cast<float>(font_char.yoffset);

#if 0
         glTexCoord2f(font_char.u[0], font_char.v[0]); glVertex2f(x + ox, y + oy);
         glTexCoord2f(font_char.u[1], font_char.v[1]); glVertex2f(x + ox + w, y + oy);
         glTexCoord2f(font_char.u[2], font_char.v[2]); glVertex2f(x + ox + w, y + oy + h);
         glTexCoord2f(font_char.u[0], font_char.v[0]); glVertex2f(x + ox, y + oy);
         glTexCoord2f(font_char.u[2], font_char.v[2]); glVertex2f(x + ox + w, y + oy + h);
         glTexCoord2f(font_char.u[3], font_char.v[3]); glVertex2f(x + ox, y + oy + h);
#else
            *ptr++ = x + ox;
            *ptr++ = y + oy;
            *ptr++ = font_char.u[0];
            *ptr++ = font_char.v[0];
            *ptr++ = x + ox + w;
            *ptr++ = y + oy;
            *ptr++ = font_char.u[1];
            *ptr++ = font_char.v[1];
            *ptr++ = x + ox + w;
            *ptr++ = y + oy + h;
            *ptr++ = font_char.u[2];
            *ptr++ = font_char.v[2];
            *ptr++ = x + ox;
            *ptr++ = y + oy + h;
            *ptr++ = font_char.u[3];
            *ptr++ = font_char.v[3];
#endif

            bounds.extend_by(x + ox, y + oy);
            bounds.extend_by(x + ox + w, y + oy);
            bounds.extend_by(x + ox + w, y + oy + h);
            bounds.extend_by(x + ox, y + oy + h);

            ++chars_printed;
            if (chars_printed == max_chars)
                break;
        }

        x += a;
        x += m_spacing_delta;

        if (i + 1 < text.size())
        {
            char            next = text[i + 1];
            kerning         search_key(static_cast<unsigned short>(next));
            compare_kerning cmp;

            vector<kerning>::const_iterator j = std::lower_bound(
                font_char.kernings.cbegin(),
                font_char.kernings.cend(),
                search_key,
                cmp);

            if (j != font_char.kernings.cend())
            {
                kerning const &k = *j;
                x += (float)(k.amount);
            }
        }
    }
    return chars_printed;
}
void font::measure(string const &text, rectangle &bounds) const
{
    float x = 0.0f;
    float y = 0.0f;

    if (text.size() == 0)
        return;

    //y += m_common.base;
    for (size_t i = 0; i < text.size(); ++i)
    {
        char          c  = text[i];
        unsigned char uc = static_cast<unsigned char>(c);

        ft_char const &font_char = m_chars_256[uc];
        if (font_char.width == 0)
            continue;

        float a  = static_cast<float>(font_char.xadvance);
        float w  = static_cast<float>(font_char.width);
        float h  = static_cast<float>(font_char.height);
        float ox = static_cast<float>(font_char.xoffset);
        float oy = static_cast<float>(font_char.yoffset);

        bounds.extend_by(x + ox, y + oy);
        bounds.extend_by(x + w + ox, y + oy);
        bounds.extend_by(x + w + ox, y + h + oy);
        bounds.extend_by(x + ox, y + h + oy);

        x += a;
        x += m_spacing_delta;

        if (i + 1 < text.size() - 1)
        {
            char            next = text[i + 1];
            kerning         search_key(static_cast<unsigned short>(next));
            compare_kerning cmp;

            auto j = std::lower_bound(
                font_char.kernings.cbegin(),
                font_char.kernings.cend(),
                search_key,
                cmp);

            if (j != font_char.kernings.cend())
            {
                kerning const &k = *j;
                x += static_cast<float>(k.amount);
            }
        }
    }
}

} // namespace ui
} // namespace renderstack
