#ifndef renderstack_ui__style_hpp
#define renderstack_ui__style_hpp

#include "renderstack_toolkit/platform.hpp"
#include "renderstack_graphics/program.hpp"
#include "renderstack_ui/font.hpp"
#include "renderstack_ui/ninepatch_style.hpp"
#include <string>
#include <memory>
#include <glm/glm.hpp>

namespace renderstack { namespace ui {

class context_style;

class style
{
private:
   glm::vec2                                       m_padding;
   glm::vec2                                       m_inner_padding;
   std::shared_ptr<class font>                     m_font;
   std::shared_ptr<class ninepatch_style>          m_ninepatch_style;
   std::shared_ptr<renderstack::graphics::program> m_background_program;
   std::shared_ptr<renderstack::graphics::program> m_foreground_program;
   unsigned int                                    m_background_texture_unit;
   unsigned int                                    m_foreground_texture_unit;

public:
   glm::vec2 const                                 &padding                () const { return m_padding; }
   glm::vec2 const                                 &inner_padding          () const { return m_inner_padding; }
   std::shared_ptr<class font>                     font                    () const { return m_font; }
   std::shared_ptr<class ninepatch_style>          ninepatch_style         () const { return m_ninepatch_style; }
   std::shared_ptr<renderstack::graphics::program> background_program      () const { return m_background_program; }
   std::shared_ptr<renderstack::graphics::program> foreground_program      () const { return m_foreground_program; }
   unsigned int                                    background_texture_unit () const { return m_background_texture_unit; }
   unsigned int                                    foreground_texture_unit () const { return m_foreground_texture_unit; }

   void                                            set_background_texture_unit	(unsigned int value){ m_background_texture_unit = value; }
   void                                            set_foreground_texture_unit	(unsigned int value){ m_foreground_texture_unit = value; }

   style(
      glm::vec2 const &padding_in, 
      glm::vec2 const &inner_padding_in
   )
   :  m_padding                   (padding_in)
   ,  m_inner_padding             (inner_padding_in)
   ,  m_font                      (nullptr)
   ,  m_ninepatch_style           (nullptr)
   ,  m_background_program        (nullptr)
   ,  m_foreground_program        (nullptr)
   ,  m_background_texture_unit   (0)
   ,  m_foreground_texture_unit   (0)
   {
   }

   style(
      glm::vec2 const                                 &padding_in, 
      glm::vec2 const                                 &inner_padding_in,
      std::shared_ptr<class font>                     font_in,
      std::shared_ptr<class ninepatch_style>          ninepatch_style_in,
      std::shared_ptr<renderstack::graphics::program> background_program,
      std::shared_ptr<renderstack::graphics::program> foreground_program
   )
   :  m_padding                   (padding_in)
   ,  m_inner_padding             (inner_padding_in)
   ,  m_font                      (font_in)
   ,  m_ninepatch_style           (ninepatch_style_in)
   ,  m_background_program        (background_program)
   ,  m_foreground_program        (foreground_program)
   ,  m_background_texture_unit   (0)
   ,  m_foreground_texture_unit   (0)
   {
   }
};

class context_style
{
private:
   std::shared_ptr<style>  s_default;
   std::shared_ptr<style>  s_null_padding;

public:
   std::shared_ptr<style> default_()
   {
      if (!s_default)
      {
         s_default = std::make_shared<style>(
            glm::vec2(6.0f, 6.0f), 
            glm::vec2(6.0f, 6.0f)
         );
      }
      return s_default; 
   }
   std::shared_ptr<style> null_padding()
   {
      if (!s_null_padding)
      {
         s_default = std::make_shared<style>(
            glm::vec2(0.0f, 0.0f), 
            glm::vec2(0.0f, 0.0f)
         );
      }
      return s_null_padding;
   }
};

} }

#endif
