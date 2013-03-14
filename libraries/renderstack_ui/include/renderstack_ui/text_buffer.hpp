#ifndef renderstack_ui__text_buffer_hpp
#define renderstack_ui__text_buffer_hpp

#include "renderstack_toolkit/platform.hpp"
#include "renderstack_graphics/vertex_stream.hpp"
#include "renderstack_graphics/vertex_format.hpp"
#include "renderstack_graphics/vertex_stream_mappings.hpp"
#include "renderstack_mesh/mesh.hpp"
#include "renderstack_ui/rectangle.hpp"
#include "renderstack_ui/font.hpp"
#include <string>
#include <vector>
#include <memory>

namespace renderstack { namespace ui {


class text_buffer
{
public:
   text_buffer(std::shared_ptr<class font> font, std::shared_ptr<renderstack::graphics::vertex_stream_mappings> mappings);
   ~text_buffer();

   rectangle const               &bounding_box  () const { return m_bounding_box;}
   float                         line_height    () const { return m_font->line_height(); }
   void                          begin_print    ();
   std::size_t                   end_print      ();
   void                          print          (std::string const &text, float x, float y);
   void                          measure        (std::string const &text);
   void                          print_center   (std::string const &text, float x, float y);
   void                          render         ();
   std::shared_ptr<class font>   font           ();

private:
   std::shared_ptr<class font>   m_font;
   unsigned int                  m_max_chars;
   rectangle                     m_bounding_box;
   renderstack::mesh::mesh       m_mesh;
   float                         *m_vertex_ptr;
   std::size_t                   m_chars_printed;
};

} }

#endif
