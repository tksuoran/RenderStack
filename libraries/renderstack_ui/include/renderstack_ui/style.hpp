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

class style
{
public:
   glm::vec2 const                                 &padding          () const { return m_padding; }
   glm::vec2 const                                 &inner_padding    () const { return m_inner_padding; }
   std::shared_ptr<class font>                     font              () const { return m_font; }
   std::shared_ptr<class ninepatch_style>          ninepatch_style   () const { return m_ninepatch_style; }
   std::shared_ptr<renderstack::graphics::program> program           () const { return m_program; }
   unsigned int                                    texture_unit      () const { return m_texture_unit; }
   void                                            set_texture_unit	(unsigned int value){ m_texture_unit = value; }

   style(
      glm::vec2 const &padding_in, 
      glm::vec2 const &inner_padding_in
   )
   :  m_padding         (padding_in)
   ,  m_inner_padding   (inner_padding_in)
   ,  m_font            (nullptr)
   ,  m_ninepatch_style (nullptr)
   ,  m_program         (nullptr)
   ,  m_texture_unit    (0)
   {
   }

   style(
      glm::vec2 const                                 &padding_in, 
      glm::vec2 const                                 &inner_padding_in,
      std::shared_ptr<class font>                     font_in,
      std::shared_ptr<class ninepatch_style>          ninepatch_style_in,
      std::shared_ptr<renderstack::graphics::program> program
   )
   :  m_padding         (padding_in)
   ,  m_inner_padding   (inner_padding_in)
   ,  m_font            (font_in)
   ,  m_ninepatch_style (ninepatch_style_in)
   ,  m_program         (program)
   ,  m_texture_unit    (0)
   {
   }
private:
   glm::vec2                                       m_padding;
   glm::vec2                                       m_inner_padding;
   std::shared_ptr<class font>                     m_font;
   std::shared_ptr<class ninepatch_style>          m_ninepatch_style;
   std::shared_ptr<renderstack::graphics::program> m_program;
   unsigned int                                    m_texture_unit;
};

} }

#endif
