#include "renderstack_ui/ninepatch_style.hpp"
#include "renderstack_ui/style.hpp"
#include "renderstack_toolkit/lodepng.h"
#include "renderstack_graphics/vertex_buffer.hpp"
#include "renderstack_graphics/index_buffer.hpp"
#include "renderstack_toolkit/gl.hpp"
#include "renderstack_toolkit/strong_gl_enums.hpp"
#include "renderstack_toolkit/logstream.hpp"
#include <stdexcept>

#define LOG_CATEGORY &log_ninepatch_style

namespace renderstack { namespace ui {

using namespace gl;
using namespace renderstack::graphics;

//mesh *ninepatch::s_shared_indices = nullptr;
ninepatch_style::ninepatch_style(std::string const &path)
:  m_border_uv(0.25f, 0.25f)
{
   slog_trace("ninepatch_style::ninepatch_style(path = %s)", path.c_str());

   gl::gen_textures(1, &m_texture);
   gl::active_texture(gl::texture_unit::texture0);
   gl::bind_texture(gl::texture_target::texture_2d, m_texture);

   {
      unsigned char* data = nullptr;
      unsigned int w;
      unsigned int h;
      unsigned int res = ::lodepng_decode32_file(&data, &w, &h, path.c_str());
      if (res != 0)
         throw std::runtime_error("texture image not found: " + path);

      gl::pixel_store_i(GL_UNPACK_ALIGNMENT, 1);
      gl::tex_image_2d(GL_TEXTURE_2D, 0, GL_RGBA, static_cast<GLint>(w), static_cast<GLint>(h), 0, GL_RGBA, GL_UNSIGNED_BYTE, reinterpret_cast<void*>(data));
      ::free(data);
      gl::tex_parameter_i(gl::texture_target::texture_2d, gl::texture_parameter_name::texture_min_filter, gl::texture_min_filter::nearest);
      gl::tex_parameter_i(gl::texture_target::texture_2d, gl::texture_parameter_name::texture_mag_filter, gl::texture_mag_filter::nearest);
      m_border_pixels.x = m_border_uv.x * (float)w;
      m_border_pixels.y = m_border_uv.y * (float)h;
   }

   gl::tex_parameter_i(gl::texture_target::texture_2d, gl::texture_parameter_name::texture_min_filter, gl::texture_min_filter::nearest);
   gl::tex_parameter_i(gl::texture_target::texture_2d, gl::texture_parameter_name::texture_mag_filter, gl::texture_mag_filter::nearest);
}

unsigned int ninepatch_style::texture() const
{
   return m_texture;
}

glm::vec2 const &ninepatch_style::border_pixels() const
{
   return m_border_pixels;
}

glm::vec2 const &ninepatch_style::border_uv() const 
{ 
   return m_border_uv; 
}

} }
