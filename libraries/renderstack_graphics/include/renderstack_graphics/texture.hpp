#ifndef texture_hpp_renderstack_graphics
#define texture_hpp_renderstack_graphics

#include "renderstack_toolkit/strong_gl_enums.hpp"
#include <cassert>
#include <memory>
#include <string>

namespace renderstack
{
namespace graphics
{

class Buffer;
class Renderer;

class Texture
{
public:
    enum class Target
    {
        texture_1d                   = 0,
        texture_2d                   = 1,
        texture_3d                   = 2,
        texture_rectangle            = 3,
        texture_cubemap              = 4,
        texture_1d_array             = 5,
        texture_2d_array             = 6,
        texture_buffer               = 7,
        texture_cubemap_array        = 8,
        texture_2d_multisample       = 9,
        texture_2d_multisample_array = 10,
    };

    static constexpr int target_count  = 11;

    static bool is_valid(Target target);

    static gl::texture_target::value gl_texture_target(Target rs_target);

    static unsigned int dimensions(Target target);

    static const char *const desc(Target target);

    Texture() = default;

    Texture(std::shared_ptr<Buffer> buffer, unsigned int internal_format);

    ~Texture();

    Texture(Target       target,
            unsigned int internal_format,
            bool         mipmaps,
            unsigned int width,
            unsigned int height = 0,
            unsigned int depth  = 0);

    void configure(Target       target,
                   unsigned int internal_format,
                   bool         mipmaps,
                   unsigned int width,
                   unsigned int height = 0,
                   unsigned int depth  = 0);

    void allocate_storage(Renderer &renderer);

    void set_debug_label(const std::string &value);

    const std::string &debug_label() const;

    void set_swizzle(unsigned int i, unsigned int value);

    void set_min_filter(gl::texture_min_filter::value value);

    void set_mag_filter(gl::texture_mag_filter::value value);

    void set_wrap(unsigned int i, gl::texture_wrap_mode::value value);

    void set_compare_mode(gl::texture_compare_mode::value value);

    void set_compare_func(gl::depth_function::value value);

    Target target() const;

    unsigned int width() const;

    unsigned int height() const;

    unsigned int depth() const;

    unsigned int swizzle(unsigned int i) const;

    gl::texture_min_filter::value min_filter() const;

    gl::texture_mag_filter::value mag_filter() const;

    gl::texture_wrap_mode::value wrap(unsigned int i) const;

    gl::texture_compare_mode::value compare_mode() const;

    gl::depth_function::value compare_func() const;

    void apply(Renderer &renderer, unsigned int unit);

    static unsigned int levels(unsigned int size);

    static void format_type(unsigned int internal_format, unsigned int &format, unsigned int &type);

    // TODO For now, this is public, since we don't yet have a class for framebuffer object.
    //friend class Renderer;
    //private:
    unsigned int gl_name() const
    {
        return m_gl_name;
    }

private:
    unsigned int m_gl_name{0U};
    std::string  m_debug_label;
    Target       m_target{Target::texture_2d};
    unsigned int m_internal_format{0U};

    unsigned int m_levels{0U};
    unsigned int m_width{0U};
    unsigned int m_height{0U};
    unsigned int m_depth{0U};

    std::shared_ptr<Buffer> m_buffer;

    bool                            m_dirty = true;
    std::array<unsigned int, 4>     m_swizzle{ gl::swizzle::red, gl::swizzle::green, gl::swizzle::blue, gl::swizzle::alpha };
    gl::texture_min_filter::value   m_min_filter{gl::texture_min_filter::nearest_mipmap_linear};
    gl::texture_mag_filter::value   m_mag_filter{gl::texture_mag_filter::linear};
    std::array<gl::texture_wrap_mode::value, 3> m_wrap{gl::texture_wrap_mode::repeat, gl::texture_wrap_mode::repeat, gl::texture_wrap_mode::repeat};
    int                             m_min_lod{-1000};
    int                             m_max_lod{1000};
    int                             m_base_level{0};
    int                             m_max_level{1000};
    gl::texture_compare_mode::value m_compare_mode{gl::texture_compare_mode::none};
    gl::depth_function::value       m_compare_func{gl::depth_function::less};
};

} // namespace graphics
} // namespace renderstack

#endif
