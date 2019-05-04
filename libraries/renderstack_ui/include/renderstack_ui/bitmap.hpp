#ifndef bitmap_hpp_renderstack_ui
#define bitmap_hpp_renderstack_ui

#include "renderstack_toolkit/platform.hpp"
#include <cassert>
#include <cmath>
#include <stdexcept>
#include <vector>

namespace renderstack
{
namespace ui
{

class Bitmap
{
public:
    Bitmap(int width, int height, int components)
        : m_width(width), m_height(height), m_components(components)
    {
        if ((width < 0) || (height < 0) || (components < 1) || (components > 4))
        {
            throw std::runtime_error("bad dimension");
        }

        m_stride = m_width * m_components;

        size_t byte_count = static_cast<size_t>(m_stride * m_height);
        m_data.resize(byte_count);
        fill(0);
    }

    ~Bitmap() = default;

    int width() const
    {
        return m_width;
    }

    int height() const
    {
        return m_height;
    }

    int components() const
    {
        return m_components;
    }

    void fill(unsigned char value)
    {
        for (int y = 0; y < height(); ++y)
        {
            for (int x = 0; x < width(); ++x)
            {
                for (int c = 0; c < m_components; ++c)
                {
                    put(x, y, c, value);
                }
            }
        }
    }

    void post_process(Bitmap &destination, float gamma)
    {
#if 0
      float inv_gamma = 1.0f / gamma;
      for (int y = 0; y < height(); ++y)
         for (int x = 0; x < width(); ++x)
            for (int c = 0; c < components(); ++c) {
               unsigned char uc = get(x, y, c);
               float f = static_cast<float>(uc) / 255.0f;
               float gamma_f = std::pow(f, inv_gamma);
               unsigned char c_gamma_f = static_cast<unsigned char>(gamma_f * 255.0f);
               destination.put(x, y, c, c_gamma_f);
            }
#else
        // Tuned for two channel Font texture (rg or luminance_alpha)
        (void)gamma;
        for (int y = 0; y < height(); ++y)
        {
            for (int x = 0; x < width(); ++x)
            {
                unsigned char ic      = get(x, y, 0);
                unsigned char oc      = get(x, y, 1);
                float         inside  = static_cast<float>(ic) / 255.0f;
                float         outside = static_cast<float>(oc) / 255.0f;
                float         max     = std::max(inside, outside);
                float         alpha   = max;
                float         color   = inside * alpha; // premultiplied
                unsigned char cc      = static_cast<unsigned char>(color * 255.0f);
                unsigned char ac      = static_cast<unsigned char>(alpha * 255.0f);
                destination.put(x, y, 0, cc);
                destination.put(x, y, 1, ac);
            }
        }
#endif
    }

    void put(int x, int y, int c, unsigned char value)
    {
        if ((x < 0) ||
            (y < 0) ||
            (c < 0) ||
            (x >= m_width) ||
            (y >= m_height) ||
            (c >= m_components))
        {
            throw std::runtime_error("invalid index");
        }

        size_t offset = static_cast<size_t>((x * m_components) + (y * m_stride) + c);
        m_data[offset]     = value;
    }

    unsigned char get(int x, int y, int c) const
    {
        if ((x < 0) ||
            (y < 0) ||
            (c < 0) ||
            (x >= m_width) ||
            (y >= m_height) ||
            (c >= m_components))
        {
            throw std::runtime_error("invalid index");
        }

        size_t offset = static_cast<size_t>((x * m_components) + (y * m_stride) + c);
        return m_data[offset];
    }

    //  For teximage_2d
    unsigned char *ptr()
    {
        return &m_data[0];
    }

    void blit(Bitmap *src,
              int     src_x,
              int     src_y,
              int     width,
              int     height,
              int     dst_x,
              int     dst_y,
              int     src_components,
              int     dst_component_offset)
    {
        if ((src == nullptr) ||
            (width < 0) ||
            (height < 0) ||
            (src_x >= src->width()) ||
            (src_y >= src->height()) ||
            (src_x + width > src->width()) ||
            (src_y + height > src->height()) ||
            (dst_x >= this->width()) ||
            (dst_y >= this->height()) ||
            (dst_x + width > this->width()) ||
            (dst_y + height > this->height()))
        {
            throw std::runtime_error("invalid input");
        }

        for (int iy = 0; iy < height; ++iy)
        {
            int ly = dst_y + iy;
            if (ly < 0)
            {
                continue;
            }
            if (ly >= this->height())
            {
                continue;
            }
            int dst_y_ = ly;
            for (int ix = 0; ix < width; ++ix)
            {
                int lx = dst_x + ix;
                if (lx < 0)
                {
                    continue;
                }
                if (lx >= this->width())
                {
                    continue;
                }
                int dst_x_ = lx;

                for (int c = 0; c < src_components; ++c)
                {
                    unsigned char value = src->get(src_x + ix, src_y + iy, c);
                    put(dst_x_, dst_y_, c + dst_component_offset, value);
                }
            }
        }
    }

    void blit(int                               width,
              int                               height,
              int                               dst_x,
              int                               dst_y,
              std::vector<unsigned char> const &src_buffer,
              int                               src_pitch,
              int                               src_byte_width,
              int                               src_components,
              int                               dst_component_offset,
              bool                              rotated)
    {
        if (rotated)
        {
            blit_rotated(width, height, dst_x, dst_y, src_buffer, src_pitch, src_byte_width, src_components, dst_component_offset);
        }
        else
        {
            blit(width, height, dst_x, dst_y, src_buffer, src_pitch, src_byte_width, src_components, dst_component_offset);
        }
    }

    void dump_data()
    {
        int counter = 0;
        for (int c = 0; c < m_components; ++c)
        {
            for (int iy = 0; iy < m_height; ++iy)
            {
                for (int ix = 0; ix < m_width; ++ix)
                {
                    unsigned char value = get(ix, iy, c);
                    printf("%u, ", value);
                    ++counter;
                    if (counter > 15)
                    {
                        printf("\n");
                        counter = 0;
                    }
                }
            }
        }
    }

    void load_data(const unsigned char *ptr)
    {
        for (int c = 0; c < m_components; ++c)
        {
            for (int iy = 0; iy < m_height; ++iy)
            {
                for (int ix = 0; ix < m_width; ++ix)
                {
                    put(ix, iy, c, *ptr++);
                }
            }
        }
    }

    void dump()
    {
        const char *shades = " -+#";
        for (int c = 0; c < m_components; ++c)
        {
            printf("\nbitmap dump w = %d h = %d c = %d\n", m_width, m_height, c);
            for (int iy = 0; iy < m_height; ++iy)
            {
                for (int ix = 0; ix < m_width; ++ix)
                {
                    unsigned char value = get(ix, iy, c);
                    printf("%c", shades[value / 64]);
                }
                printf("\n");
            }
        }
    }

    void blit(int                               width,
              int                               height,
              int                               dst_x,
              int                               dst_y,
              std::vector<unsigned char> const &src_buffer,
              int                               src_pitch,
              int                               src_byte_width,
              int                               src_components,
              int                               dst_component_offset)
    {
        if ((width < 0) ||
            (height < 0) ||
            (src_components < 0))
        {
            throw std::runtime_error("invalid index");
        }

#if 0
      printf("blit: w:%d h:%d dx:%d dy:%d sp: %d sbw: %d sc:%d dco: %d\n",
         width,
         height,
         dst_x,
         dst_y,
         src_pitch,
         src_byte_width,
         src_components,
         dst_component_offset
      );
#endif

        int dst_height = height;
        int dst_width  = width;

        //char *shades = " -+#";

        for (int iy = 0; iy < dst_height; ++iy)
        {
            int ly = dst_y + iy;
            if (ly < 0)
            {
                continue;
            }
            if (ly >= this->height())
            {
                continue;
            }
            int dst_y_ = ly;
            for (int ix = 0; ix < dst_width; ++ix)
            {
                int lx = dst_x + ix;
                if (lx < 0)
                {
                    continue;
                }
                if (lx >= this->width())
                {
                    continue;
                }
                int dst_x_ = lx;

                //unsigned int src_x = ix;
                int src_y_ = height - 1 - iy;

                for (int c = 0; c < src_components; ++c)
                {
                    int           src_byte_x = (ix * src_components) + c;
                    unsigned char value      = 0;
                    size_t        offset     = 0;
                    if (src_byte_x < src_byte_width)
                    {
                        offset = static_cast<size_t>(src_byte_x + (src_y_ * src_pitch));
                        value  = src_buffer[offset];
                    }
                    else
                    {
                        continue;
                    }
                    //if (value > 128)
                    //put(dst_x_, dst_y_, c + dst_component_offset, value);
                    put(dst_x_, dst_y_, c + dst_component_offset, value);
                    //printf("%c", shades[value / 64]);
                }
            }
            //printf("\n");
        }
        //printf("\nend of blit\n");
    }

    void blit_rotated(int                               width,
                      int                               height,
                      int                               dst_x,
                      int                               dst_y,
                      std::vector<unsigned char> const &src_buffer,
                      int                               src_pitch,
                      int                               src_byte_width,
                      int                               src_components,
                      int                               dst_component_offset)
    {
        if ((width < 0) ||
            (height < 0) ||
            (src_components < 0))
        {
            throw std::runtime_error("invalid index");
        }

        int dst_width  = height;
        int dst_height = width;

#if 0
      printf("blit_rotated: w:%d h:%d dx:%d dy:%d sp: %d sbw: %d sc:%d dco: %d\n",
         width,
         height,
         dst_x,
         dst_y,
         src_pitch,
         src_byte_width,
         src_components,
         dst_component_offset
      );
#endif

        //char *shades = " -+#";

        for (int iy = 0; iy < dst_height; ++iy)
        {
            int ly = dst_y + iy;
            if (ly < 0)
            {
                continue;
            }
            if (ly >= this->height())
            {
                continue;
            }
            int dst_y_ = ly;
            for (int ix = 0; ix < dst_width; ++ix)
            {
                int lx = dst_x + ix;
                if (lx < 0)
                {
                    continue;
                }
                if (lx >= this->width())
                {
                    continue;
                }
                int dst_x_ = lx;

                int src_x = iy;
                int src_y = /*height - 1 - */ ix;

                for (int c = 0; c < src_components; ++c)
                {
                    int           src_byte_x = (src_x * src_components) + c;
                    unsigned char value;
                    size_t        offset = 0;
                    if (src_byte_x < src_byte_width)
                    {
                        offset = src_byte_x + (src_y * src_pitch);
                        value  = src_buffer[offset];
                    }
                    else
                    {
                        value = 0;
                    }
                    //printf("%c", shades[value / 64]);
                    put(dst_x_, dst_y_, c + dst_component_offset, value);
                }
            }
            //printf("\n");
        }
        //printf("\nend of blit\n");
    }

private:
    int                        m_width;
    int                        m_height;
    int                        m_components;
    int                        m_stride{0};
    std::vector<unsigned char> m_data;
};

} // namespace ui
} // namespace renderstack

#endif
