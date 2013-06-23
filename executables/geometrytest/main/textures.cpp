#include "renderstack_toolkit/platform.hpp"
#include "main/application.hpp"
#include "main/textures.hpp"
#include "renderstack_graphics/texture.hpp"
#include "renderstack_toolkit/gl.hpp"
#include "renderstack_toolkit/strong_gl_enums.hpp"
#include "renderstack_toolkit/lodepng.h"
#include "renderstack_toolkit/logstream.hpp"
#include <stdexcept>

#define LOG_CATEGORY &log_textures

using namespace gl;
using namespace std;
using namespace renderstack::graphics;

textures::textures()
:  background_texture(nullptr)
{
}

void textures::on_load(renderstack::graphics::renderer &renderer)
{
   slog_trace("textures::on_load()");

   background_texture = load(renderer, 1, "res/images/background.png");
}

shared_ptr<renderstack::graphics::texture> textures::load(
   renderstack::graphics::renderer &renderer,
   unsigned int texture_unit,
   string const &path
)
{
   unsigned char* data = nullptr;
   unsigned int w;
   unsigned int h;
   unsigned int res = ::lodepng_decode24_file(&data, &w, &h, path.c_str());

   switch (res)
   {
   case 0: break;
   case 27: throw runtime_error("texture image load failed - data length smaller than PNG header");
   case 28: throw runtime_error("texture image load failed - PNG header signature fail");
   case 29: throw runtime_error("texture image load failed - PNG header fail IHDR");
   case 32: throw runtime_error("texture image load failed - PNG header fail compression");
   case 33: throw runtime_error("texture image load failed - PNG header fail filter");
   case 34: throw runtime_error("texture image load failed - PNG header fail interlace");
   case 48: throw runtime_error("texture image load failed - empty data (file?)");
   case 57: throw runtime_error("texture image load failed - invalid CRC");
   case 78: throw runtime_error("texture image not found");
   case 83: throw runtime_error("texture image load failed - out of memory");
   default: throw runtime_error("texture image load failed");
   }

   if (data == nullptr)
      throw runtime_error("texture image load failed - no data");

   auto texture = make_shared<renderstack::graphics::texture>(
      renderstack::graphics::texture_target::texture_2d,
      GL_RGB8,
      false,
      w,
      h
   );

   unsigned int old_unit;
   auto old_texture = renderer.set_texture(texture_unit, texture, &old_unit);

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

   texture->set_min_filter(texture_min_filter::nearest);
   texture->set_mag_filter(texture_mag_filter::nearest);

   texture->apply(renderer, texture_unit);

   renderer.restore_texture(
      renderstack::graphics::texture_target::texture_2d,
      old_texture,
      old_unit
   );

   return texture;
}
