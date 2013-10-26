#ifndef gui_renderer_hpp_renderstack_ui
#define gui_renderer_hpp_renderstack_ui

#include "renderstack_toolkit/platform.hpp"
#include "renderstack_toolkit/service.hpp"
#include "renderstack_graphics/configuration.hpp"
#include "renderstack_graphics/renderer.hpp"
#include "renderstack_graphics/blend_state.hpp"
#include <glm/glm.hpp>
#include <memory>

namespace renderstack { namespace graphics {
   class buffer;
   class program;
   class renderer;
   class samplers;
   class shader_monitor;
   class uniform_block;
   class uniform_buffer_range;
   class vertex_format;
   class vertex_stream;
} }

namespace renderstack { namespace ui {

class font;
class ninepatch_style;
class style;

class gui_renderer : public renderstack::toolkit::service
{
private:
   gui_renderer(gui_renderer const &other);

public:
   struct gui_uniforms
   {
      std::size_t clip_from_model;  /* mat4 */
      std::size_t color;            /* vec4 */
      std::size_t color_add;        /* vec4 */
      std::size_t color_scale;      /* vec4 */
      std::size_t hsv_matrix;       /* mat4 */
      std::size_t t;                /* float */
   };

   gui_renderer();
   /*virtual*/ ~gui_renderer();

   void connect(
      std::shared_ptr<renderstack::graphics::renderer>         renderer_,
      std::shared_ptr<renderstack::graphics::shader_monitor>   shader_monitor_
   );
   /*virtual*/ void initialize_service();

   void prepare();
   void on_resize(int width, int height);
   glm::mat4 const &ortho() const { return m_ortho; }
   void set_program     (std::shared_ptr<renderstack::graphics::program> value);
   void set_texture     (unsigned int unit, std::shared_ptr<renderstack::graphics::texture> texture);
   void set_transform   (glm::mat4 const &value);
   void set_color_add   (glm::vec4 const &value);
   void set_color_scale (glm::vec4 const &value);
   void set_hsv_matrix  (glm::mat4 const &value);
   void set_t           (float value);

   void set_vertex_buffer();
   void set_index_buffer();
   void draw_elements_base_vertex(
      unsigned int begin_mode, int count, unsigned int index_type, const void *indices,
      int base_vertex);

   std::shared_ptr<style> default_style      () const { return m_default_style; }
   std::shared_ptr<style> null_padding_style () const { return m_null_padding_style; }
   std::shared_ptr<style> button_style       () const { return m_button_style; }
   std::shared_ptr<style> menulist_style     () const { return m_menulist_style; }
   std::shared_ptr<style> choice_style       () const { return m_choice_style; }
   std::shared_ptr<style> colorpicker_style  () const { return m_colorpicker_style; }
   std::shared_ptr<style> slider_style       () const { return m_slider_style; }

   std::shared_ptr<class renderstack::graphics::renderer>   renderer()        const { return m_renderer; }
   std::shared_ptr<renderstack::graphics::vertex_format>    vertex_format()   const { return m_vertex_format; }
   std::shared_ptr<renderstack::graphics::vertex_stream>    vertex_stream()   const { return m_vertex_stream; }
   std::shared_ptr<renderstack::graphics::buffer>           vertex_buffer()   const { return m_vertex_buffer; }
   std::shared_ptr<renderstack::graphics::buffer>           index_buffer()    const { return m_index_buffer; }

   bool use_uniform_buffers() const
   {
      // Test all conditions; can_use.uniform_buffer_object can be forced to false
      bool use_uniform_buffers = 
         renderstack::graphics::configuration::can_use.uniform_buffer_object && 
         //(m_glsl_version >= 140) &&
         (renderstack::graphics::configuration::shader_model_version >= 4);

      return use_uniform_buffers;
   }

   void blend_alpha();
   void blend_add();
   void blend_disable();

private:
   std::shared_ptr<renderstack::graphics::program> load_program(std::string const &name, std::string const &shader);
   void map(std::shared_ptr<renderstack::graphics::program> program);

private:
   renderstack::graphics::blend_state     m_blend_disabled;
   renderstack::graphics::blend_state     m_blend_alpha;
   renderstack::graphics::blend_state     m_blend_add;
   renderstack::graphics::render_states   m_gui_render_states;


   std::shared_ptr<renderstack::graphics::renderer>                  m_renderer;
   std::shared_ptr<renderstack::graphics::shader_monitor>            m_shader_monitor;

   glm::mat4                                                         m_ortho;

   std::string                                                       m_shader_path;
   std::vector<std::pair<std::string, int> >                         m_shader_versions;

   std::shared_ptr<renderstack::graphics::uniform_block>             m_uniform_block;
   struct gui_uniforms                                               m_uniforms;
   std::shared_ptr<renderstack::graphics::program>                   m_program;

   std::shared_ptr<renderstack::graphics::samplers>                  m_samplers;
   std::shared_ptr<renderstack::graphics::vertex_attribute_mappings> m_vertex_attribute_mappings;
   std::shared_ptr<renderstack::graphics::fragment_outputs>          m_fragment_outputs;

   std::shared_ptr<renderstack::graphics::vertex_format>             m_vertex_format;
   std::shared_ptr<renderstack::graphics::vertex_stream>             m_vertex_stream;
   std::shared_ptr<renderstack::graphics::buffer>                    m_vertex_buffer;
   std::shared_ptr<renderstack::graphics::buffer>                    m_index_buffer;

   std::shared_ptr<font>                                             m_font;
   std::shared_ptr<renderstack::ui::style>                           m_default_style;
   std::shared_ptr<renderstack::ui::style>                           m_null_padding_style;
   std::shared_ptr<ninepatch_style>                                  m_button_ninepatch_style;
   std::shared_ptr<ninepatch_style>                                  m_menulist_ninepatch_style;
   std::shared_ptr<ninepatch_style>                                  m_slider_ninepatch_style;
   std::shared_ptr<style>                                            m_button_style;
   std::shared_ptr<style>                                            m_menulist_style;
   std::shared_ptr<style>                                            m_choice_style;
   std::shared_ptr<style>                                            m_colorpicker_style;
   std::shared_ptr<style>                                            m_slider_style;
   std::shared_ptr<renderstack::graphics::program>                   m_ninepatch_program;
   std::shared_ptr<renderstack::graphics::program>                   m_font_program;
   std::shared_ptr<renderstack::graphics::program>                   m_hsv_program;
   std::shared_ptr<renderstack::graphics::program>                   m_slider_program;
   unsigned char                                                     *m_start;
};


} }

#endif
