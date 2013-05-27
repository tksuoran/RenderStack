#ifndef renderstack_ui__font_hpp
   #define renderstack_ui__font_hpp

#include "renderstack_toolkit/platform.hpp"
#include "renderstack_ui/rectangle.hpp"
#include <string>
#include <vector>
#include <memory>

namespace renderstack { namespace graphics {

class renderer;
class texture;

} }

namespace renderstack { namespace ui {

class bitmap;

class font
{
public:
   //font();
#if defined(RENDERSTACK_USE_FREETYPE)
   font(
      renderstack::graphics::renderer  &renderer,
      std::string const                &path,
      unsigned int                     size,
      float                            outline_thickness = 0.0f
   );
#endif
   ~font();

   void              save                  () const;
   float             line_height           () const { return m_line_height; }
   std::size_t       print                 (std::string const &text, rectangle &bounds, float *&ptr, float x, float y) const;
   void              measure               (std::string const &text, rectangle &bounds) const;

   std::shared_ptr<class renderstack::graphics::texture>
                     texture() const { return m_texture; }

   bool              hinting               () const { return m_hinting; }
   bool              rgb                   () const { return m_rgb; }
   bool              regular_grid          () const { return m_regular_grid; }
   unsigned int      dpi                   () const { return m_dpi; }
   float             gamma                 () const { return m_gamma; }
   float             saturation            () const { return m_saturation; }
   std::string const &chars                () const { return m_chars; }
   unsigned int      pixel_size            () const { return m_pixel_size; }
   float             bolding               () const { return m_bolding; }
   float             outline_thickness     () const { return m_outline_thickness; }

   void              set_hinting           (bool value){ m_hinting = value; }
   void              set_rgb               (bool value){ m_rgb = value; }
   void              set_regular_grid      (bool value){ m_regular_grid = value; }
   void              set_dpi               (unsigned int value){ m_dpi = value; }
   void              set_gamma             (float value){ m_gamma = value; }
   void              set_saturation        (float value){ m_saturation = value; }
   void              set_chars             (std::string const &value){ m_chars = value; }
   void              set_pixel_size        (unsigned int value){ m_pixel_size = value; }
   void              set_bolding           (float value){ m_bolding = value; }
   void              set_outline_thickness (float value){ m_outline_thickness = value; }

   void              render(renderstack::graphics::renderer &renderer);
   void              post_process(renderstack::graphics::renderer &renderer);

private:
   void    validate(int error);
   struct kerning
   {
      kerning(unsigned short _second):second(_second){}
      kerning(unsigned short _second, short _amount):second(_second), amount(_amount){}
      unsigned short  second;
      short           amount;
   };
   struct compare_kerning
   {
      bool operator()(kerning const &a, const kerning& b) const
      { 
         return a.second < b.second; 
      }
   };
   struct ft_char
   {
      ft_char()
      {
         width = 0;
         height = 0;
         xoffset = 0;
         yoffset = 0;
         xadvance = 0;
         u[0] = 0.0f; u[1] = 0.0f; u[2] = 0.0f; u[3] = 0.0f;
         v[0] = 0.0f; v[1] = 0.0f; v[2] = 0.0f; v[3] = 0.0f;
      }
      unsigned short       width;
      unsigned short       height;
      short                xoffset;
      short                yoffset;
      short                xadvance;

      float                u[4];
      float                v[4];
      std::vector<kerning> kernings;
   };

   ft_char           m_chars_256[256];
   std::string       m_chars;
   std::string       m_path;
   bool              m_hinting;
   bool              m_regular_grid;
   bool              m_rgb;
   unsigned int      m_dpi;
   float             m_gamma;
   float             m_saturation;
   unsigned int      m_pixel_size;
   float             m_bolding;
   float             m_outline_thickness;
   int               m_spacing_delta;
   int               m_hint_mode;

   float             m_line_height;

   unsigned int      m_texture_width;
   unsigned int      m_texture_height;

   std::shared_ptr<class renderstack::graphics::texture> m_texture;
   std::shared_ptr<class bitmap>                         m_bitmap;
};

} }

#endif
