#ifndef font_hpp_renderstack_ui
#define font_hpp_renderstack_ui

#include "renderstack_graphics/texture.hpp"
#include "renderstack_ui/bitmap.hpp"
#include "renderstack_ui/rectangle.hpp"
#include <memory>
#include <string>
#include <vector>

namespace renderstack
{
namespace graphics
{

class Renderer;

} // namespace graphics
} // namespace renderstack

namespace renderstack
{
namespace ui
{

class Font
{
public:
    Font(renderstack::graphics::Renderer &renderer,
         const std::string &              path,
         unsigned int                     size,
         float                            outline_thickness = 0.0f);

    ~Font() = default;

    void save() const;

    float line_height() const
    {
        return m_line_height;
    }

    size_t print(const std::string &text, Rectangle &bounds, float *&ptr, float x, float y, size_t max_chars) const;

    void measure(const std::string &text, Rectangle &bounds) const;

    renderstack::graphics::Texture *texture()
    {
        return m_texture.get();
    }

    bool hinting() const
    {
        return m_hinting;
    }

    bool rgb() const
    {
        return m_rgb;
    }

    bool regular_grid() const
    {
        return m_regular_grid;
    }

    unsigned int dpi() const
    {
        return m_dpi;
    }

    float gamma() const
    {
        return m_gamma;
    }

    float saturation() const
    {
        return m_saturation;
    }

    const std::string &chars() const
    {
        return m_chars;
    }

    unsigned int pixel_size() const
    {
        return m_pixel_size;
    }

    float bolding() const
    {
        return m_bolding;
    }

    float outline_thickness() const
    {
        return m_outline_thickness;
    }

    void set_hinting(bool value)
    {
        m_hinting = value;
    }

    void set_rgb(bool value)
    {
        m_rgb = value;
    }

    void set_regular_grid(bool value)
    {
        m_regular_grid = value;
    }

    void set_dpi(unsigned int value)
    {
        m_dpi = value;
    }

    void set_gamma(float value)
    {
        m_gamma = value;
    }

    void set_saturation(float value)
    {
        m_saturation = value;
    }

    void set_chars(const std::string &value)
    {
        m_chars = value;
    }

    void set_pixel_size(unsigned int value)
    {
        m_pixel_size = value;
    }

    void set_bolding(float value)
    {
        m_bolding = value;
    }

    void set_outline_thickness(float value)
    {
        m_outline_thickness = value;
    }

    void render(renderstack::graphics::Renderer &renderer);

    void post_process(renderstack::graphics::Renderer &renderer);

private:
    void validate(int error);

    struct kerning
    {
        kerning(unsigned short _second) : second(_second) {}
        kerning(unsigned short _second, short _amount) : second(_second), amount(_amount) {}
        unsigned short second;
        short          amount;
    };

    struct compare_kerning
    {
        bool operator()(kerning const &a, const kerning &b) const
        {
            return a.second < b.second;
        }
    };

    struct ft_char
    {
        ft_char()
        {
            width    = 0;
            height   = 0;
            xoffset  = 0;
            yoffset  = 0;
            xadvance = 0;
            u[0]     = 0.0f;
            u[1]     = 0.0f;
            u[2]     = 0.0f;
            u[3]     = 0.0f;
            v[0]     = 0.0f;
            v[1]     = 0.0f;
            v[2]     = 0.0f;
            v[3]     = 0.0f;
        }
        unsigned short width;
        unsigned short height;
        short          xoffset;
        short          yoffset;
        short          xadvance;

        float                u[4];
        float                v[4];
        std::vector<kerning> kernings;
    };

    ft_char      m_chars_256[256];
    std::string  m_chars;
    std::string  m_path;
    bool         m_hinting{true};
    bool         m_regular_grid{false};
    bool         m_rgb{false};
    unsigned int m_dpi{96};
    float        m_gamma{1.0f};
    float        m_saturation{1.0f};
    unsigned int m_pixel_size;
    float        m_bolding;
    float        m_outline_thickness;
    int          m_spacing_delta{0};
    int          m_hint_mode;

    float m_line_height;

    unsigned int m_texture_width;
    unsigned int m_texture_height;

    std::unique_ptr<renderstack::graphics::Texture> m_texture;
    std::unique_ptr<Bitmap>                         m_bitmap;
};

} // namespace ui
} // namespace renderstack

#endif
