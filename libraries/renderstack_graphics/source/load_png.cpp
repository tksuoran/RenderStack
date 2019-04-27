#include "renderstack_graphics/log.hpp"
#include "renderstack_graphics/renderer.hpp"
#include "renderstack_graphics/texture.hpp"
#include "renderstack_toolkit/platform.hpp"
#include <cstdio>
#include <png.h>
#include <sstream>
#include <stdexcept>

namespace renderstack
{
namespace graphics
{

#define LOG_CATEGORY &log_load_png

using namespace std;

bool load_png(renderstack::graphics::Texture  &texture,
              renderstack::graphics::Renderer &renderer,
              unsigned int                     texture_unit,
              const std::string &              path)
{
    png_byte header[8];

    FILE *fp = ::fopen(path.c_str(), "rb");
    if (fp == 0)
    {
        stringstream ss;
        ss << "load_png(): file not found: " << path;
        log_error(ss.str().c_str());
        throw runtime_error(ss.str().c_str());
    }

    ::fread(header, 1, 8, fp);

    try
    {
        if (::png_sig_cmp(header, 0, 8))
        {
            throw runtime_error("png header fail");
        }

        png_structp png_ptr = ::png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
        if (!png_ptr)
        {
            throw runtime_error("png_create_read_struct() fail");
        }

        try
        {
            png_infop info_ptr = ::png_create_info_struct(png_ptr);
            if (!info_ptr)
            {
                throw runtime_error("png_create_info_struct() fail");
            }

            png_infop end_info = ::png_create_info_struct(png_ptr);
            if (!end_info)
            {
                throw runtime_error("png_create_info_struct() fail");
            }

            ::png_init_io(png_ptr, fp);
            ::png_set_sig_bytes(png_ptr, 8);
            ::png_read_info(png_ptr, info_ptr);

            int         bit_depth;
            int         color_type;
            png_uint_32 width;
            png_uint_32 height;

            ::png_get_IHDR(png_ptr, info_ptr, &width, &height, &bit_depth, &color_type, NULL, NULL, NULL);
            ::png_read_update_info(png_ptr, info_ptr);

            switch (bit_depth)
            {
                case 8:
                {
                    // OK
                    break;
                }

                case 16:
                {
                    // TODO; fallthrough
                }

                default:
                {
                    // TODO use expand
                    throw runtime_error("unsupported PNG image bit depth");
                }
            }

            GLenum internal_format;
            GLenum format;
            GLenum type;

            switch (color_type)
            {
                case PNG_COLOR_TYPE_GRAY: // (bit depths 1, 2, 4, 8, 16)
                {
                    internal_format = GL_R8;
                    format          = GL_RED;
                    type            = GL_UNSIGNED_BYTE;
                    break;
                }

                case PNG_COLOR_TYPE_GRAY_ALPHA: // (bit depths 8, 16)
                {
                    internal_format = GL_RG8;
                    format          = GL_RG;
                    type            = GL_UNSIGNED_BYTE;
                    break;
                }

                case PNG_COLOR_TYPE_PALETTE: // (bit depths 1, 2, 4, 8)
                {
                    // TODO expand
                    throw runtime_error("unsupported PNG image color type");
                }

                case PNG_COLOR_TYPE_RGB: /* (bit_depths 8, 16) */
                {
                    internal_format = GL_RGB8;
                    format          = GL_RGB;
                    type            = GL_UNSIGNED_BYTE;
                    break;
                }

                case PNG_COLOR_TYPE_RGB_ALPHA: /* (bit_depths 8, 16) */
                {
                    internal_format = GL_RGBA8;
                    format          = GL_RGBA;
                    type            = GL_UNSIGNED_BYTE;
                    break;
                }

                default:
                {
                    throw runtime_error("unsupported PNG image color type");
                }
            }

            int rowbytes = ::png_get_rowbytes(png_ptr, info_ptr);
            rowbytes += 3 - ((rowbytes - 1) % 4);

            png_byte *image_data = new png_byte[rowbytes * height];

            try
            {
                png_bytep *row_pointers = new png_bytep[height];

                for (png_uint_32 i = 0; i < height; i++)
                {
                    row_pointers[/*height - 1 - */ i] = image_data + i * rowbytes;
                }

                ::png_read_image(png_ptr, row_pointers);

                texture.configure(renderstack::graphics::Texture::Target::texture_2d,
                                 internal_format,
                                 false,
                                 width,
                                 height);
                texture.allocate_storage(renderer);

                unsigned int old_unit;
                auto         old_texture = renderer.set_texture(texture_unit, &texture, &old_unit);

                gl::pixel_store_i(GL_UNPACK_ALIGNMENT, 1);
                gl::tex_sub_image_2d(GL_TEXTURE_2D, 0, 0, 0,
                                     static_cast<GLsizei>(width),
                                     static_cast<GLsizei>(height),
                                     format,
                                     type,
                                     reinterpret_cast<void *>(image_data));

                texture.set_min_filter(gl::texture_min_filter::nearest);
                texture.set_mag_filter(gl::texture_mag_filter::nearest);

                texture.apply(renderer, texture_unit);

                renderer.restore_texture(renderstack::graphics::Texture::Target::texture_2d,
                                         old_texture,
                                         old_unit);

                ::png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
                delete[] row_pointers;
            }
            catch (...)
            {
                delete[] image_data;
                throw;
            }
            delete[] image_data;
        }
        catch (...)
        {
            ::png_destroy_read_struct(&png_ptr, (png_infopp) nullptr, (png_infopp) nullptr);
            throw;
        }
        ::png_destroy_read_struct(&png_ptr, (png_infopp) nullptr, (png_infopp) nullptr);
    }
    catch (runtime_error const &e)
    {
        ::fclose(fp);
        log_error(e.what());
        throw;
    }
    catch (...)
    {
        ::fclose(fp);

        throw;
    }
    ::fclose(fp);

    stringstream ss;
    ss << "load_png(): file loaded " << path << "\n";
    log_info(ss.str().c_str());

    return true;
}

} // namespace graphics
} // namespace renderstack
