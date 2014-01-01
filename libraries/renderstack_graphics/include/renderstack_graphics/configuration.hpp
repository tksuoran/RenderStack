#ifndef configuration_hpp_renderstack_graphics
#define configuration_hpp_renderstack_graphics

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

   struct can_use_t
   {
      bool  texture_3d                    ; 
      bool  vertex_buffer                 ; 
      bool  gl_version_300                ; 
      bool  gpu_shader4                   ; 
      bool  conditional_render            ; 
      bool  map_buffer_range              ; 
      bool  bind_buffer_range             ; 
      bool  texture_float                 ; 
      bool  color_buffer_float            ; 
      bool  depth_buffer_float            ; 
      bool  packed_float                  ; 
      bool  texture_shared_exponent       ; 
      bool  framebuffer_object            ; 
      bool  half_float                    ; 
      bool  half_float_pixel              ; 
      bool  framebuffer_multisample       ; 
      bool  framebuffer_blit              ; 
      bool  texture_integer               ; 
      bool  texture_array                 ; 
      bool  texture_r                     ; 
      bool  texture_rg                    ; 
      bool  packed_depth_stencil          ; 
      bool  draw_buffers2                 ; 
      bool  texture_compression_rgtc      ; 
      bool  vertex_array_object           ; 
      bool  integer_framebuffer_format    ; 
      bool  bind_frag_data_location       ; 
      bool  integer_attributes            ; 
      bool  instanced_arrays              ; 
      bool  gl_version_310                ; 
      bool  draw_instanced                ; 
      bool  texture_buffer_object         ; 
      bool  uniform_buffer_object         ; 
      bool  gl_version_320                ; 
      bool  seamless_cube_map             ; 
      bool  draw_elements_base_vertex     ; 
      bool  geometry_shaders              ; 
      bool  gl_version_330                ; 
      bool  sampler_object                ; 
      bool  timer_query                   ; 
      bool  gl_version_400                ; 
      bool  gpu_shader5                   ; 
      bool  transform_feedback            ; 
      bool  tesselation_shaders           ; 
      bool  gl_version_410                ; 
      bool  binary_shaders                ; 

      bool  frame_terminator              ; // 
      bool  string_marker                 ;

      bool  pixel_buffer_object           ; //  \todo

      bool  map_buffer_oes                ;
      bool  discard_framebuffer_oes       ;
      bool  invalidate_framebuffer        ;
      bool  tex_storage                   ;

      bool  debug_output_arb              ;

      can_use_t();
   };

   static can_use_t can_use;

   static unsigned int   default_vao                 ;
   static int   max_vertex_attribs                   ;
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
   static void check(std::vector<std::string> const &extensions, bool &var, std::string const &name, int gl_min_ver, int gles_min_ver, std::string const &gl_ext);
   static void check(std::vector<std::string> const &extensions, bool &var, std::string const &name, int gl_min_ver, int gles_min_ver, std::string const &gl_ext, std::string const &gl_ext2);
   static void check(std::vector<std::string> const &extensions, bool &var, std::string const &name, int gl_min_ver, int gles_min_ver, std::string const &gl_ext, std::string const &gl_ext2, std::string const &gl_ext3);
};

} }

#endif
