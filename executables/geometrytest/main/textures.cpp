#include "renderstack_toolkit/platform.hpp"
#include "main/application.hpp"
#include "main/textures.hpp"
#include "renderstack_toolkit/gl.hpp"
#include "renderstack_toolkit/strong_gl_enums.hpp"
#include "renderstack_toolkit/lodepng.h"
#include "renderstack_toolkit/logstream.hpp"
#include <stdexcept>

#define LOG_CATEGORY &log_textures

using namespace gl;

textures::textures()
:  background_texture_object(~0U)
{
}

void textures::on_load()
{
   slog_trace("textures::on_load()");

   load(&background_texture_object,    1, "res/images/background.png");
}

void textures::load(
   unsigned int *texture_object, 
   int texture_unit,
   std::string const &path
)
{
   gl::gen_textures(1, texture_object);
   gl::active_texture(static_cast<GLenum>(gl::texture_unit::texture0 + texture_unit));
   gl::bind_texture(texture_target::texture_2d, *texture_object);

   {
      unsigned char* data = nullptr;
      unsigned int w;
      unsigned int h;
      unsigned int res = ::lodepng_decode24_file(&data, &w, &h, path.c_str());

      switch (res)
      {
      case 0: break;
      case 27: throw std::runtime_error("texture image load failed - data length smaller than PNG header");
      case 28: throw std::runtime_error("texture image load failed - PNG header signature fail");
      case 29: throw std::runtime_error("texture image load failed - PNG header fail IHDR");
      case 32: throw std::runtime_error("texture image load failed - PNG header fail compression");
      case 33: throw std::runtime_error("texture image load failed - PNG header fail filter");
      case 34: throw std::runtime_error("texture image load failed - PNG header fail interlace");
      case 48: throw std::runtime_error("texture image load failed - empty data (file?)");
      case 57: throw std::runtime_error("texture image load failed - invalid CRC");
      case 78: throw std::runtime_error("texture image not found");
      case 83: throw std::runtime_error("texture image load failed - out of memory");
      default: throw std::runtime_error("texture image load failed");
      }

      if (data == nullptr)
         throw std::runtime_error("texture image load failed - no data");

      gl::pixel_store_i(GL_UNPACK_ALIGNMENT, 1);
      gl::tex_image_2d(
         GL_TEXTURE_2D,
         0,
         GL_RGB,
         static_cast<GLsizei>(w),
         static_cast<GLsizei>(h),
         0,
         GL_RGB,
         GL_UNSIGNED_BYTE, 
         reinterpret_cast<void*>(data)
      );
      ::free(data);
      tex_parameter_i(texture_target::texture_2d, texture_parameter_name::texture_min_filter, texture_min_filter::nearest);
      tex_parameter_i(texture_target::texture_2d, texture_parameter_name::texture_mag_filter, texture_mag_filter::nearest);
   }
}
