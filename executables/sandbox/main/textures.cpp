#include "renderstack_toolkit/platform.hpp"
#include "renderstack_toolkit/gl.hpp"
#include "renderstack_toolkit/strong_gl_enums.hpp"
#include "renderstack_graphics/texture.hpp"
#include "renderstack_graphics/load_png.hpp"

#include "main/application.hpp"
#include "main/textures.hpp"
#include "main/log.hpp"

#include <stdexcept>

#define LOG_CATEGORY &log_textures

using namespace gl;
using namespace std;
using namespace renderstack::graphics;

textures::textures()
:  service("textures")
,  background_texture(nullptr)
{
}

textures::~textures()
{
}

void textures::connect(shared_ptr<renderstack::graphics::renderer> renderer)
{
   m_renderer = renderer;

   initialization_depends_on(renderer);
}

void textures::initialize_service()
{
   assert(m_renderer);

   slog_trace("textures::on_load()");
   
   background_texture = load(1, "res/images/background.png");
}

#if 0
shared_ptr<renderstack::graphics::texture> textures::load(
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

   auto &r = *m_renderer;

   auto texture = make_shared<renderstack::graphics::texture>(
      renderstack::graphics::texture_target::texture_2d,
      GL_RGB8,
      false,
      w,
      h
   );

   unsigned int old_unit;
   auto old_texture = r.set_texture(texture_unit, texture, &old_unit);

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

   texture->apply(r, texture_unit);

   r.restore_texture(
      renderstack::graphics::texture_target::texture_2d,
      old_texture,
      old_unit
   );

   return texture;
}
#endif

#if 0
static int
read_png(FILE *fp)
{
   png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING,0,0,0);
   png_infop info_ptr = NULL;
   png_bytep row = NULL, display = NULL;

   if (png_ptr == NULL)
      return 0;

   if (setjmp(png_jmpbuf(png_ptr)))
   {
      png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
      if (row != NULL) free(row);
      if (display != NULL) free(display);
      return 0;
   }

   png_init_io(png_ptr, fp);

   info_ptr = png_create_info_struct(png_ptr);
   if (info_ptr == NULL)
      png_error(png_ptr, "OOM allocating info structure");

   png_set_keep_unknown_chunks(png_ptr, PNG_HANDLE_CHUNK_ALWAYS, NULL, 0);

   png_read_info(png_ptr, info_ptr);

   {
      png_size_t rowbytes = png_get_rowbytes(png_ptr, info_ptr);

      row = malloc(rowbytes);
      display = malloc(rowbytes);

      if (row == NULL || display == NULL)
         png_error(png_ptr, "OOM allocating row buffers");

      {
         png_uint_32 height = png_get_image_height(png_ptr, info_ptr);
         int passes = png_set_interlace_handling(png_ptr);
         int pass;

         png_start_read_image(png_ptr);

         for (pass = 0; pass < passes; ++pass)
         {
            png_uint_32 y = height;

            /* NOTE: this trashes the row each time; interlace handling won't
             * work, but this avoids memory thrashing for speed testing.
             */
            while (y-- > 0)
               png_read_row(png_ptr, row, display);
         }
      }
   }

   /* Make sure to read to the end of the file: */
   png_read_end(png_ptr, info_ptr);
   png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
   free(row);
   free(display);
   return 1;
}
#endif


shared_ptr<renderstack::graphics::texture> textures::load(
   unsigned int texture_unit,
   string const &path
)
{
   return load_png(*m_renderer, texture_unit, path);
}