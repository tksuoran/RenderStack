#include "renderstack_toolkit/platform.hpp"
#include "renderstack_toolkit/lodepng.h"
#include "renderstack_toolkit/gl.hpp"
#include "renderstack_toolkit/strong_gl_enums.hpp"
#include "renderstack_ui/ninepatch_style.hpp"
#include "renderstack_ui/style.hpp"
#include "renderstack_ui/log.hpp"
#include "renderstack_graphics/buffer.hpp"
#include "renderstack_graphics/renderer.hpp"
#include "renderstack_graphics/texture.hpp"
#include <stdexcept>

#define LOG_CATEGORY &log_ninepatch_style

namespace renderstack { namespace ui {

using namespace gl;
using namespace std;
using namespace renderstack::graphics;

//mesh *ninepatch::s_shared_indices = nullptr;
ninepatch_style::ninepatch_style(
   renderstack::graphics::renderer &renderer,
   string const &path,
   shared_ptr<renderstack::graphics::program> program,
   int texture_unit
)
:  m_program      (program)
,  m_texture_unit (texture_unit)
,  m_border_uv    (0.25f, 0.25f)
{
   slog_trace("ninepatch_style::ninepatch_style(path = %s)", path.c_str());

   if (path.length() > 0)
   {
      unsigned char* data = nullptr;
      unsigned int w;
      unsigned int h;
      unsigned int res = ::lodepng_decode32_file(&data, &w, &h, path.c_str());
      if (res != 0)
         throw runtime_error("texture image not found: " + path);

      m_texture = make_shared<renderstack::graphics::texture>(
         renderstack::graphics::texture_target::texture_2d,
         GL_RGBA8,
         false,
         w,
         h
      );
      m_texture->allocate_storage(renderer);
      m_texture->set_debug_label(path);
      unsigned int old_unit;
      auto old_texture = renderer.set_texture(0, m_texture, &old_unit);

      gl::pixel_store_i(GL_UNPACK_ALIGNMENT, 1);
      gl::tex_sub_image_2d(
         GL_TEXTURE_2D,
         0,
         0,
         0,
         static_cast<GLint>(w),
         static_cast<GLint>(h),
         GL_RGBA,
         GL_UNSIGNED_BYTE,
         reinterpret_cast<void*>(data)
      );
      ::free(data);

      m_texture->set_min_filter(gl::texture_min_filter::nearest);
      m_texture->set_mag_filter(gl::texture_mag_filter::nearest);

      m_border_pixels.x = m_border_uv.x * (float)w;
      m_border_pixels.y = m_border_uv.y * (float)h;

      renderer.restore_texture(renderstack::graphics::texture_target::texture_2d, old_texture, old_unit);
   }
}

shared_ptr<renderstack::graphics::texture> ninepatch_style::texture() const
{
   return m_texture;
}

shared_ptr<renderstack::graphics::program> ninepatch_style::program() const
{
   return m_program;
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
