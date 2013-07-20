#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_STROKER_H
#include FT_GLYPH_H
#include FT_BITMAP_H

#include "renderstack_ui/glyph.hpp"
#include "renderstack_ui/log.hpp"
#include "renderstack_ui/bitmap.hpp"
#include <map>
#include <stdexcept>

namespace renderstack { namespace ui {

void glyph::validate(FT_Error error)
{
   // if (error == FT_Err_Unknown_File_Format)
   //   log() << "FT_Err_Unknown_File_Format\n";

   if (error)
      throw std::runtime_error("freetype error");
}

/*
   FT_Library  m_library;
   FT_Bitmap   m_bitmap;
   FT_UInt     m_glyph_index;
*/

glyph::glyph(
   FT_Library    library,
   FT_Face       font_face,
   unsigned char c,
   float         bolding,
   float         outline_thickness,
   bool          rgb,
   int           hint_mode
)
{
   m_outline_thickness = outline_thickness;

   m_glyph_index = FT_Get_Char_Index(font_face, c);
   if (m_glyph_index == 0) {
      m_bearingX      = 0;
      m_width         = 0;
      m_bearingY      = 0;
      m_height        = 0;
      m_horiAdvance   = 0;
      m_bm_width      = 0;
      m_bm_height     = 0;
      m_x0            = 0;
      m_y0            = 0;
      m_xadvance      = 0;
      return;
   }

   validate(
      FT_Load_Glyph(
         font_face, 
         m_glyph_index, 
         ((m_outline_thickness == 0.0f) ? FT_LOAD_RENDER : 0) | hint_mode
      )
   );

   bool has_width  = (font_face->glyph->metrics.width != 0);
   bool has_height = (font_face->glyph->metrics.height != 0);
   bool render     = has_width && has_height;

   FT_Bitmap      bitmap;
   FT_Bitmap_New(&bitmap);

   m_bearingX     = (float)font_face->glyph->metrics.horiBearingX / 64.0f;
   m_width        = (float)font_face->glyph->metrics.width        / 64.0f;
   m_bearingY     = (float)font_face->glyph->metrics.horiBearingY / 64.0f;
   m_height       = (float)font_face->glyph->metrics.height       / 64.0f;
   m_horiAdvance  = (float)font_face->glyph->metrics.horiAdvance  / 64.0f;

   if (render)
   {
      if (outline_thickness > 0.0f)
      {
         FT_Stroker stroker;

         validate(FT_Stroker_New(library, &stroker));

         FT_Stroker_Set(
            stroker,
            static_cast<int>(outline_thickness * 64.0f),
            FT_STROKER_LINECAP_ROUND,
            FT_STROKER_LINEJOIN_ROUND,
            0
         );

         {
            validate(FT_Load_Glyph(font_face, m_glyph_index, hint_mode));

            struct FT_GlyphRec_ *glyph;
            validate(FT_Get_Glyph(font_face->glyph, &glyph));

            {
               struct FT_GlyphRec_ *stroked_glyph = glyph;
               validate(FT_Glyph_StrokeBorder(&stroked_glyph, stroker, 0, 0));

               {
                  struct FT_GlyphRec_ *bitmap_glyph = stroked_glyph;
                  validate(FT_Glyph_To_Bitmap(&bitmap_glyph, FT_RENDER_MODE_NORMAL, 0, 1));

                  struct FT_BitmapGlyphRec_ *ft_bitmap_glyph = (struct FT_BitmapGlyphRec_*)bitmap_glyph;
                  FT_Bitmap_Copy(library, &ft_bitmap_glyph->bitmap, &bitmap);
                  FT_Done_Glyph(bitmap_glyph);
               }

               //FT_Done_Glyph(stroked_glyph);
            }

            FT_Done_Glyph(glyph);
         }
         FT_Stroker_Done(stroker);

      }
      else
      {
         FT_Bitmap_Copy(library, &font_face->glyph->bitmap, &bitmap);
      }

      if (bolding > 0.0f)
      {
         int i_bolding = static_cast<int>(bolding * 64.0f);
         validate(FT_Bitmap_Embolden(library, &bitmap, i_bolding, 0));
      }

      /*if(rgb)
      {
      lcd_filter(&font_face->glyph->bitmap, FT_RENDER_MODE_LCD);
      } */

      m_bm_width  = bitmap.width;
      m_bm_height = bitmap.rows;
      m_bm_pitch  = bitmap.pitch;
   }
   else
   {
      m_bm_width  = 0;
      m_bm_height = 0;
      m_bm_pitch  = 0;
   }

   // printf("char '%c' (%f) bm size %d x %d\n", c, outline_thickness, m_bm_width, m_bm_height);

   if (rgb)
   {
      m_bearingX    /= 3.0f;
      m_horiAdvance /= 3.0f;
      m_width       /= 3.0f;
      float bm_width_f = (float)m_bm_width;
      bm_width_f = std::ceil(bm_width_f / 3.0f);
      m_bm_width = static_cast<int>(bm_width_f);
   }

   m_x0 = static_cast<int>(std::floor(m_bearingX + 0.5f));
   m_y0 = static_cast<int>(std::floor(m_bearingY - m_height + 0.5f));
   m_xadvance = static_cast<int>(std::floor(m_horiAdvance));

   //  Search for bounding box where value > threshold
   //  First pass records pixel coordinates
   m_crop_x_min = m_bm_width;
   m_crop_y_min = m_bm_height;
   m_crop_x_max = 0;
   m_crop_y_max = 0;
   m_buffer.resize(m_bm_height * m_bm_pitch);
   //char *shades = " -+#";
   //printf("\nGlyph = %c\n", c);
   for (int iy = 0; iy < m_bm_height; ++iy)
   {
      for (int ix = 0; ix < m_bm_width; ++ix)
      {
         int            src_x    = ix;
         int            src_y    = m_bm_height - 1 - iy;
         size_t         address  = src_x + (src_y * m_bm_pitch);
         unsigned char  value    = bitmap.buffer[address];
         m_buffer[address] = value;

         //printf("%c", shades[value / 64]);

         if (value > 40)
         {
            m_crop_x_max = std::max(src_x, m_crop_x_max);
            m_crop_x_min = std::min(src_x, m_crop_x_min);
            m_crop_y_max = std::max(src_y, m_crop_y_max);
            m_crop_y_min = std::min(src_y, m_crop_y_min);
         }
      }
      //printf("\n");
   }

   FT_Bitmap_Done(library, &bitmap);

   //  Turn max pixel coordinates to number of pixels to crop
   m_crop_x_max = m_bm_width  - 1 - m_crop_x_max;
   m_crop_y_max = m_bm_height - 1 - m_crop_y_max;
}

void glyph::dump() const
{
   const char *shades = " -+#";
   printf("\nglyph dump: w = %d h = %d\n", m_bm_width, m_bm_height);
   for (int iy = 0; iy < m_bm_height; ++iy)
   {
      for (int ix = 0; ix < m_bm_width; ++ix)
      {
         std::size_t offset = static_cast<size_t>(ix + (iy * m_bm_pitch));
         unsigned char value = m_buffer[offset];
         printf("%c", shades[value / 64]);
      }
      printf("\n");
   }
}

} }
