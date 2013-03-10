#ifndef renderstack_ui__gui_renderer_hpp
#define renderstack_ui__gui_renderer_hpp

#include "renderstack_toolkit/platform.hpp"
#include <glm/glm.hpp>
#include <memory>

namespace renderstack { namespace graphics {
   class program;
   class samplers;
   class uniform_block;
   class uniform_buffer;
   class uniform_buffer_range;
   class index_buffer;
   class vertex_buffer;
   class vertex_stream_mappings;
} }

namespace renderstack { namespace ui {

class font;
class ninepatch_style;
class style;

class gui_renderer
{
public:
   struct uniform_offsets
   {
      std::size_t  model_to_clip;  /* mat4 */
      std::size_t  color;          /* vec4 */
      std::size_t  color_add;      /* vec4 */
      std::size_t  color_scale;    /* vec4 */
      std::size_t  hsv_matrix;     /* mat4 */
      std::size_t  t;              /* float */
   };

   gui_renderer();

   void prepare();
   void on_resize(int width, int height);
   glm::mat4 const &ortho() const { return m_ortho; }
   void begin_edit();
   void end_edit();
   /*void push();
   void pop();*/
   void set_program     (std::shared_ptr<renderstack::graphics::program> value);
   void set_texture     (unsigned int unit, unsigned int texture);
   void set_transform   (glm::mat4 const &value);
   void set_color_add   (glm::vec4 const &value);
   void set_color_scale (glm::vec4 const &value);
   void set_hsv_matrix  (glm::mat4 const &value);
   void set_t           (float value);

   std::shared_ptr<style> button_style     () const { return m_button_style; }
   std::shared_ptr<style> menulist_style   () const { return m_menulist_style; }
   std::shared_ptr<style> choice_style     () const { return m_choice_style; }
   std::shared_ptr<style> colorpicker_style() const { return m_colorpicker_style; }
   std::shared_ptr<style> slider_style     () const { return m_slider_style; }

private:
   void map(std::shared_ptr<renderstack::graphics::program> program);

   glm::mat4                                                      m_ortho;

   int                                                            m_glsl_version;
   std::shared_ptr<renderstack::graphics::uniform_block>          m_uniform_block;
   std::shared_ptr<renderstack::graphics::uniform_buffer>         m_uniform_buffer;
   std::shared_ptr<renderstack::graphics::uniform_buffer_range>   m_uniform_buffer_range;
   struct uniform_offsets                                         m_uniform_offsets;
   struct uniform_offsets                                         m_uniform_keys;
   std::shared_ptr<renderstack::graphics::program>                m_program;

   std::shared_ptr<renderstack::graphics::samplers>               m_samplers;
   std::shared_ptr<renderstack::graphics::vertex_stream_mappings> m_mappings;

   std::shared_ptr<font>                                          m_font;
   std::shared_ptr<ninepatch_style>                               m_button_ninepatch_style;
   std::shared_ptr<ninepatch_style>                               m_menulist_ninepatch_style;
   std::shared_ptr<ninepatch_style>                               m_slider_ninepatch_style;
   std::shared_ptr<style>                                         m_button_style;
   std::shared_ptr<style>                                         m_menulist_style;
   std::shared_ptr<style>                                         m_choice_style;
   std::shared_ptr<style>                                         m_colorpicker_style;
   std::shared_ptr<style>                                         m_slider_style;
   std::shared_ptr<renderstack::graphics::program>                m_ninepatch_program;
   std::shared_ptr<renderstack::graphics::program>                m_font_program;
   std::shared_ptr<renderstack::graphics::program>                m_hsv_program;
   std::shared_ptr<renderstack::graphics::program>                m_slider_program;
   unsigned char                                                  *m_start;
};


} }

#endif
