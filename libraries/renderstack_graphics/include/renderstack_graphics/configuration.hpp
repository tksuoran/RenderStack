#ifndef renderstack_graphics__configuration_hpp
#define renderstack_graphics__configuration_hpp

#include "renderstack_toolkit/platform.hpp"
#include <string>
#include <vector>

namespace renderstack { namespace graphics {

class configuration
{
public:
   static bool  use_gl1;
   static bool  intel;
   static int   gl_version;
   static int   glsl_version;
   static int   shader_model_version;

   static bool  core_profile                         ;
   static bool  compatibility_profile                ;
   static bool  forward_compatible                   ;

   static bool  use_binary_shaders                   ;
   static bool  use_integer_polygon_ids              ;
   static bool  use_vertex_array_object              ;
   static bool  must_use_vertex_array_object         ;
   static bool  throw_program_exceptions             ;

   struct can_use_t {
      bool  vertex_buffer                 ; // 150 map buffer etc.
      bool  gl_version_300                ; // 300 -----------------------------------
      bool  gpu_shader4                   ; // 300 integer attributes
      bool  conditional_render            ; // 300
      bool  map_buffer_range              ; // 300
      bool  bind_buffer_range             ; // 300
      bool  texture_float                 ; // 300
      bool  color_buffer_float            ; // 300
      bool  depth_buffer_float            ; // 300
      bool  packed_float                  ; // 300
      bool  texture_shared_exponent       ; // 300
      bool  framebuffer_object            ; // 300
      bool  half_float                    ; // 300
      bool  half_float_pixel              ; // 300
      bool  framebuffer_multisample       ; // 300
      bool  framebuffer_blit              ; // 300
      bool  texture_integer               ; // 300
      bool  texture_array                 ; // 300
      bool  texture_r                     ; // 300
      bool  texture_rg                    ; // 300
      bool  packed_depth_stencil          ; // 300
      bool  draw_buffers2                 ; // 300 Per-color-attachment blend enables and color writemasks
      bool  texture_compression_rgtc      ; // 300
      bool  vertex_array_object           ; // 300
      bool  integer_framebuffer_format    ; // 300
      bool  bind_frag_data_location       ; // 300 - alias for shader_model_version >= 4
      bool  integer_attributes            ; // 300
      bool  instanced_arrays              ; // 300
      bool  gl_version_310                ; // 310 -----------------------------------
      bool  draw_instanced                ; // 310
      bool  texture_buffer_object         ; // 310
      bool  uniform_buffer_object         ; // 310
      bool  gl_version_320                ; // 320 -----------------------------------
      bool  seamless_cube_map             ; // 320
      bool  draw_elements_base_vertex     ; // 320
      bool  geometry_shaders              ; // 320
      bool  gl_version_330                ; // 330 -----------------------------------
      bool  sampler_object                ; // 330
      bool  timer_query                   ; // 330
      bool  gl_version_400                ; // 400 -----------------------------------
      bool  gpu_shader5                   ; // 400
      bool  transform_feedback            ; // 400
      bool  tesselation_shaders           ; // 400
      bool  gl_version_410                ; // 410 -----------------------------------
      bool  binary_shaders                ; // 410

      bool  frame_terminator              ; // 
      bool  string_marker                 ;

      bool  pixel_buffer_object           ; //  \todo
   };

   static can_use_t can_use;

   static unsigned int   default_vao                 ;
   static int   max_texture_size                     ;
   static int   max_3d_texture_size                  ;
   static int   max_cube_map_texture_size            ;
   static int   max_texture_buffer_size              ;
   static int   max_texture_units                    ; /* fixed function */
   static int   max_texture_image_units              ; /* shaders */
   static int   max_combined_texture_image_units     ;
   static int   max_uniform_block_size               ;
   static int   max_uniform_buffer_bindings          ;
   static int   max_vertex_uniform_blocks            ;
   static int   max_fragment_uniform_blocks          ;
   static int   max_geometry_uniform_blocks          ;
   static int   max_tess_control_uniform_blocks      ;
   static int   max_tess_evaluation_uniform_blocks   ;
   static unsigned int   uniform_buffer_offset_alignment      ;

   static void initialize();

private:
   static void check(std::vector<std::string> const &extensions, bool &var, std::string const &name, int gl_min_ver, std::string const &gl_ext);
};

} }

#endif
