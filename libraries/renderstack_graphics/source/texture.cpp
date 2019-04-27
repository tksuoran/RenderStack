#include "renderstack_graphics/texture.hpp"
#include "renderstack_graphics/buffer.hpp"
#include "renderstack_graphics/configuration.hpp"
#include "renderstack_graphics/renderer.hpp"
#include "renderstack_toolkit/gl.hpp"
#include "renderstack_toolkit/platform.hpp"
#include <algorithm>
#include <stdexcept>

namespace renderstack
{
namespace graphics
{

using namespace std;

bool Texture::is_valid(Target target)
{
    switch (target)
    {
        case Target::texture_1d: return true;
        case Target::texture_2d: return true;
        case Target::texture_3d: return configuration::can_use.texture_3d;
        case Target::texture_rectangle: return true; // TODO configuration;
        case Target::texture_cubemap: return true;   // TODO configuration
        case Target::texture_1d_array: return configuration::can_use.texture_array;
        case Target::texture_2d_array: return configuration::can_use.texture_array;
        case Target::texture_buffer: return configuration::can_use.texture_buffer_object;
        case Target::texture_cubemap_array: return configuration::can_use.texture_array;
        case Target::texture_2d_multisample: return true; // TODO configuration
        case Target::texture_2d_multisample_array: return configuration::can_use.texture_array;
        default:
            return false;
    }
}

gl::texture_target::value Texture::gl_texture_target(Target rs_target)
{
    switch (rs_target)
    {
        case Target::texture_1d: return gl::texture_target::texture_1d;
        case Target::texture_2d: return gl::texture_target::texture_2d;
        case Target::texture_3d: return gl::texture_target::texture_3d;
        case Target::texture_rectangle: return gl::texture_target::texture_rectangle;
        case Target::texture_cubemap: return gl::texture_target::texture_cubemap;
        case Target::texture_1d_array: return gl::texture_target::texture_1d_array;
        case Target::texture_2d_array: return gl::texture_target::texture_2d_array;
        case Target::texture_buffer: return gl::texture_target::texture_buffer;
        case Target::texture_cubemap_array: return gl::texture_target::texture_cubemap_array;
        case Target::texture_2d_multisample: return gl::texture_target::texture_2d_multisample;
        case Target::texture_2d_multisample_array: return gl::texture_target::texture_2d_multisample_array;
        default:
            throw runtime_error("invalid texture target");
    }
}

unsigned int Texture::dimensions(Texture::Target target)
{
    switch (target)
    {
        case Target::texture_1d: return 1;
        case Target::texture_2d: return 2;
        case Target::texture_3d: return 3;
        case Target::texture_rectangle: return 2;
        case Target::texture_cubemap: return 2;
        case Target::texture_1d_array: return 2;
        case Target::texture_2d_array: return 3;
        case Target::texture_buffer: return 0; // TODO
        case Target::texture_cubemap_array: return 3;
        case Target::texture_2d_multisample: return 2;
        case Target::texture_2d_multisample_array: return 3;
        default:
            throw runtime_error("invalid texture target");
    }
}

const char *const Texture::desc(Texture::Target target)
{
    switch (target)
    {
        case Target::texture_1d: return "texture_1d";
        case Target::texture_2d: return "texture_2d";
        case Target::texture_3d: return "texture_3d";
        case Target::texture_rectangle: return "texture_rectangle";
        case Target::texture_cubemap: return "texture_cubemap";
        case Target::texture_1d_array: return "texture_1d_array";
        case Target::texture_2d_array: return "texture_2d_array";
        case Target::texture_buffer: return "texture_buffer";
        case Target::texture_cubemap_array: return "texture_cubemap_array";
        case Target::texture_2d_multisample: return "texture_2d_multisample";
        case Target::texture_2d_multisample_array: return "texture_2d_multisample_array";
        default:
            throw runtime_error("invalid texture target");
    }
}

using namespace std;

/*static*/ unsigned int Texture::levels(unsigned int size)
{
    unsigned int levels = size > 0 ? 1 : 0;

    while (size > 1)
    {
        size = size / 2;
        ++levels;
    }
    return levels;
}

// OpenGL ES 3.0 specification Table 3.2: Valid combinations of format, type, and sized internalformat.
struct format_type_bpp_internalformat
{
    unsigned int format;
    unsigned int type;
    unsigned int bpp;
    unsigned int internalformat;
};

static format_type_bpp_internalformat valid_format_type_internalformat_combinations[] =
    {
        {GL_RGBA, GL_UNSIGNED_BYTE, 4, GL_RGBA8},
        {GL_RGBA, GL_UNSIGNED_BYTE, 4, GL_RGB5_A1},
        {GL_RGBA, GL_UNSIGNED_BYTE, 4, GL_RGBA4},
        {GL_RGBA, GL_UNSIGNED_BYTE, 4, GL_SRGB8_ALPHA8},
        {GL_RGBA, GL_BYTE, 4, GL_RGBA8_SNORM},
        {GL_RGBA, GL_UNSIGNED_SHORT_4_4_4_4, 2, GL_RGBA4},
        {GL_RGBA, GL_UNSIGNED_SHORT_5_5_5_1, 2, GL_RGB5_A1},
        {GL_RGBA, GL_UNSIGNED_INT_2_10_10_10_REV, 4, GL_RGB10_A2},
        {GL_RGBA, GL_UNSIGNED_INT_2_10_10_10_REV, 4, GL_RGB5_A1},
        {GL_RGBA, GL_HALF_FLOAT, 8, GL_RGBA16F},
        {GL_RGBA, GL_FLOAT, 16, GL_RGBA32F},
        {GL_RGBA, GL_FLOAT, 16, GL_RGBA16F},
        {GL_RGBA_INTEGER, GL_UNSIGNED_BYTE, 4, GL_RGBA8UI},
        {GL_RGBA_INTEGER, GL_BYTE, 4, GL_RGBA8I},
        {GL_RGBA_INTEGER, GL_UNSIGNED_SHORT, 8, GL_RGBA16UI},
        {GL_RGBA_INTEGER, GL_SHORT, 8, GL_RGBA16I},
        {GL_RGBA_INTEGER, GL_UNSIGNED_INT, 16, GL_RGBA32UI},
        {GL_RGBA_INTEGER, GL_INT, 16, GL_RGBA32I},
        {GL_RGBA_INTEGER, GL_UNSIGNED_INT_2_10_10_10_REV, 4, GL_RGB10_A2UI},
        {GL_RGB, GL_UNSIGNED_BYTE, 3, GL_RGB8},
        {GL_RGB, GL_UNSIGNED_BYTE, 3, GL_RGB565},
        {GL_RGB, GL_UNSIGNED_BYTE, 3, GL_SRGB8},
        {GL_RGB, GL_BYTE, 3, GL_RGB8_SNORM},
        {GL_RGB, GL_UNSIGNED_SHORT_5_6_5, 2, GL_RGB565},
        {GL_RGB, GL_UNSIGNED_INT_10F_11F_11F_REV, 4, GL_R11F_G11F_B10F},
        {GL_RGB, GL_UNSIGNED_INT_5_9_9_9_REV, 4, GL_RGB9_E5},
        {GL_RGB, GL_HALF_FLOAT, 6, GL_RGB16F},
        {GL_RGB, GL_HALF_FLOAT, 6, GL_R11F_G11F_B10F},
        {GL_RGB, GL_HALF_FLOAT, 6, GL_RGB9_E5},
        {GL_RGB, GL_FLOAT, 12, GL_RGB32F},
        {GL_RGB, GL_FLOAT, 12, GL_RGB16F},
        {GL_RGB, GL_FLOAT, 12, GL_R11F_G11F_B10F},
        {GL_RGB, GL_FLOAT, 12, GL_RGB9_E5},
        {GL_RGB_INTEGER, GL_UNSIGNED_BYTE, 3, GL_RGB8UI},
        {GL_RGB_INTEGER, GL_BYTE, 3, GL_RGB8I},
        {GL_RGB_INTEGER, GL_UNSIGNED_SHORT, 6, GL_RGB16UI},
        {GL_RGB_INTEGER, GL_SHORT, 6, GL_RGB16I},
        {GL_RGB_INTEGER, GL_UNSIGNED_INT, 12, GL_RGB32UI},
        {GL_RGB_INTEGER, GL_INT, 12, GL_RGB32I},
        {GL_RG, GL_UNSIGNED_BYTE, 2, GL_RG8},
        {GL_RG, GL_BYTE, 2, GL_RG8_SNORM},
        {GL_RG, GL_HALF_FLOAT, 4, GL_RG16F},
        {GL_RG, GL_FLOAT, 8, GL_RG32F},
        {GL_RG, GL_FLOAT, 8, GL_RG16F},
        {GL_RG_INTEGER, GL_UNSIGNED_BYTE, 2, GL_RG8UI},
        {GL_RG_INTEGER, GL_BYTE, 2, GL_RG8I},
        {GL_RG_INTEGER, GL_UNSIGNED_SHORT, 4, GL_RG16UI},
        {GL_RG_INTEGER, GL_SHORT, 4, GL_RG16I},
        {GL_RG_INTEGER, GL_UNSIGNED_INT, 8, GL_RG32UI},
        {GL_RG_INTEGER, GL_INT, 8, GL_RG32I},
        {GL_RED, GL_UNSIGNED_BYTE, 1, GL_R8},
        {GL_RED, GL_BYTE, 1, GL_R8_SNORM},
        {GL_RED, GL_HALF_FLOAT, 2, GL_R16F},
        {GL_RED, GL_FLOAT, 4, GL_R32F},
        {GL_RED, GL_FLOAT, 4, GL_R16F},
        {GL_RED_INTEGER, GL_UNSIGNED_BYTE, 1, GL_R8UI},
        {GL_RED_INTEGER, GL_BYTE, 1, GL_R8I},
        {GL_RED_INTEGER, GL_UNSIGNED_SHORT, 2, GL_R16UI},
        {GL_RED_INTEGER, GL_SHORT, 2, GL_R16I},
        {GL_RED_INTEGER, GL_UNSIGNED_INT, 4, GL_R32UI},
        {GL_RED_INTEGER, GL_INT, 4, GL_R32I},
        {GL_DEPTH_COMPONENT, GL_UNSIGNED_SHORT, 2, GL_DEPTH_COMPONENT16},
        {GL_DEPTH_COMPONENT, GL_UNSIGNED_INT, 4, GL_DEPTH_COMPONENT24},
        {GL_DEPTH_COMPONENT, GL_UNSIGNED_INT, 4, GL_DEPTH_COMPONENT16},
        {GL_DEPTH_COMPONENT, GL_FLOAT, 4, GL_DEPTH_COMPONENT32F},
        {GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, 4, GL_DEPTH24_STENCIL8},
        {GL_DEPTH_STENCIL, GL_FLOAT_32_UNSIGNED_INT_24_8_REV, 8, GL_DEPTH32F_STENCIL8},
};

static const unsigned int combination_count =
    sizeof(valid_format_type_internalformat_combinations) / sizeof(format_type_bpp_internalformat);

/*static*/ void Texture::format_type(unsigned int internal_format, unsigned int &format, unsigned int &type)
{
    for (unsigned int i = 0; i < combination_count; ++i)
    {
        format_type_bpp_internalformat const &combination = valid_format_type_internalformat_combinations[i];
        if (combination.internalformat == internal_format)
        {
            format = combination.format;
            type   = combination.type;
            return;
        }
    }

    throw runtime_error("invalid texture internal format");
}

Texture::Texture(Texture::Target target,
                 unsigned int    internal_format,
                 bool            mipmaps,
                 unsigned int    width,
                 unsigned int    height,
                 unsigned int    depth)
{
    configure(target, internal_format, mipmaps, width, height, depth);
}

void Texture::configure(Texture::Target target,
                        unsigned int    internal_format,
                        bool            mipmaps,
                        unsigned int    width,
                        unsigned int    height,
                        unsigned int    depth)
{
    m_target = target;
    m_internal_format = internal_format;
    m_width = width;
    m_height = height;
    m_depth = depth;

    unsigned int dimensions = Texture::dimensions(m_target);

    if (dimensions >= 1)
    {
        if (width == 0)
        {
            throw runtime_error("zero texture width");
        }
    }

    if (dimensions >= 2)
    {
        if (height == 0)
        {
            throw runtime_error("zero texture height");
        }
    }

    if (dimensions == 3)
    {
        if (depth == 0)
        {
            throw runtime_error("zero texture depth");
        }
    }

    // TODO should we throw error instead?
    if (dimensions < 3)
    {
        depth = 0;
    }

    if (dimensions < 2)
    {
        height = 0;
    }

    if (mipmaps)
    {
        unsigned int x_levels = levels(width);
        unsigned int y_levels = levels(height);
        unsigned int z_levels = levels(depth);
        m_levels              = std::max(x_levels, y_levels);
        m_levels              = std::max(m_levels, z_levels);
    }
    else
    {
        m_levels = 1;
    }

    // Clamp to edge would be nicer default?
}

Texture::Texture(shared_ptr<Buffer> buffer,
                 unsigned int       internal_format)
    : m_target(renderstack::graphics::Texture::Target::texture_buffer)
    , m_internal_format(internal_format)
{
}

Texture::~Texture()
{
    gl::delete_textures(1, &m_gl_name);
}

void Texture::allocate_storage(Renderer &renderer)
{
    gl::gen_textures(1, &m_gl_name);

    gl::texture_target::value gl_target  = Texture::gl_texture_target(m_target);
    unsigned int              dimensions = Texture::dimensions(m_target);

    unsigned int old_unit;
    unsigned int unit        = renderer.effective_texture_unit();
    auto         old_texture = renderer.set_texture(unit, this, &old_unit);
    assert(unit == old_unit);

#if !defined(__APPLE__)
    if (configuration::can_use.tex_storage)
    {
        switch (dimensions)
        {
            case 0:
                if (!m_buffer)
                {
                    throw runtime_error("texture buffer missing");
                }

                gl::tex_buffer(gl::texture_target::texture_buffer, m_internal_format, m_buffer->gl_name());
                break;

            case 1:
                gl::tex_storage_1d(gl_target, m_levels, m_internal_format, m_width);
                break;

            case 2:
                gl::tex_storage_2d(gl_target, m_levels, m_internal_format, m_width, m_height);
                break;

            case 3:
                gl::tex_storage_3d(gl_target, m_levels, m_internal_format, m_width, m_height, m_depth);
                break;

            default:
                throw runtime_error("invalid texture target");
        }
    }
    else
#endif
    {
        unsigned int format;
        unsigned int type;
        unsigned int width  = m_width;
        unsigned int height = m_height;
        unsigned int depth  = m_depth;

        format_type(m_internal_format, format, type);

        switch (dimensions)
        {
            case 0:
                break;

            case 1:
                for (unsigned int level = 0; level < m_levels; ++level)
                {
                    gl::tex_image_1d(gl_target, level, m_internal_format, width, 0, format, type, nullptr);
                    width = std::max(width / 2, 1u);
                }
                break;

            case 2:
                for (unsigned int level = 0; level < m_levels; ++level)
                {
                    gl::tex_image_2d(gl_target, level, m_internal_format, width, height, 0, format, type, nullptr);
                    width  = std::max(width / 2, 1u);
                    height = std::max(height / 2, 1u);
                }
                break;

            case 3:
                for (unsigned int level = 0; level < m_levels; ++level)
                {
                    gl::tex_image_3d(gl_target, level, m_internal_format, width, height, depth, 0, format, type, nullptr);
                    width  = std::max(width / 2, 1u);
                    height = std::max(height / 2, 1u);
                }
                break;

            default:
                throw runtime_error("invalid texture target");
        }
    }

    renderer.restore_texture(m_target, old_texture, old_unit);
}

void Texture::set_debug_label(const std::string &value)
{
    m_debug_label = value;
}

const std::string &Texture::debug_label() const
{
    return m_debug_label;
}

void Texture::set_swizzle(unsigned int i, unsigned int value)
{
    assert(i < 4);
    if (m_swizzle[i] != value)
    {
        m_swizzle[i] = value;
        m_dirty      = true;
    }
}

void Texture::set_min_filter(gl::texture_min_filter::value value)
{
    if (m_min_filter != value)
    {
        m_min_filter = value;
        m_dirty      = true;
    }
}

void Texture::set_mag_filter(gl::texture_mag_filter::value value)
{
    if (m_mag_filter != value)
    {
        m_mag_filter = value;
        m_dirty      = true;
    }
}

void Texture::set_wrap(unsigned int i, gl::texture_wrap_mode::value value)
{
    assert(i < 3);
    if (m_wrap[i] != value)
    {
        m_wrap[i] = value;
        m_dirty   = true;
    }
}

void Texture::set_compare_mode(gl::texture_compare_mode::value value)
{
    if (m_compare_mode != value)
    {
        m_compare_mode = value;
        m_dirty        = true;
    }
}

void Texture::set_compare_func(gl::depth_function::value value)
{
    if (m_compare_func != value)
    {
        m_compare_func = value;
        m_dirty        = true;
    }
}

Texture::Target Texture::target() const
{
    return m_target;
}

unsigned int Texture::width() const
{
    return m_width;
}

unsigned int Texture::height() const
{
    return m_height;
}

unsigned int Texture::depth() const
{
    return m_depth;
}

unsigned int Texture::swizzle(unsigned int i) const
{
    assert(i < 4);
    return m_swizzle[i];
}

gl::texture_min_filter::value Texture::min_filter() const
{
    return m_min_filter;
}

gl::texture_mag_filter::value Texture::mag_filter() const
{
    return m_mag_filter;
}

gl::texture_wrap_mode::value Texture::wrap(unsigned int i) const
{
    assert(i < 3);
    return m_wrap[i];
}

gl::texture_compare_mode::value Texture::compare_mode() const
{
    return m_compare_mode;
}

gl::depth_function::value Texture::compare_func() const
{
    return m_compare_func;
}

void Texture::apply(Renderer &renderer, unsigned int unit)
{
    if (!renderer.texture_is_bound(unit, m_target, this))
    {
        throw runtime_error("texture is not bound and cannot be applied");
    }

    // TODO Cache - make these immutable?

    if (!m_dirty)
    {
        return;
    }

    unsigned int gl_target = Texture::gl_texture_target(m_target);

    if (configuration::gl_version >= 300) // TODO
    {
        gl::tex_parameter_i(gl_target, gl::texture_parameter_name::texture_swizzle_r, m_swizzle[0]);
        gl::tex_parameter_i(gl_target, gl::texture_parameter_name::texture_swizzle_g, m_swizzle[1]);
        gl::tex_parameter_i(gl_target, gl::texture_parameter_name::texture_swizzle_b, m_swizzle[2]);
        gl::tex_parameter_i(gl_target, gl::texture_parameter_name::texture_swizzle_a, m_swizzle[3]);
    }

    gl::tex_parameter_i(gl_target, gl::texture_parameter_name::texture_min_filter, m_min_filter);
    gl::tex_parameter_i(gl_target, gl::texture_parameter_name::texture_mag_filter, m_mag_filter);

    gl::tex_parameter_i(gl_target, gl::texture_parameter_name::texture_wrap_s, m_wrap[0]);
    gl::tex_parameter_i(gl_target, gl::texture_parameter_name::texture_wrap_t, m_wrap[1]);

#if !defined(RENDERSTACK_GL_API_OPENGL_ES_3)
    if (configuration::can_use.texture_3d)
#endif
    {
        gl::tex_parameter_i(gl_target, gl::texture_parameter_name::texture_wrap_r, m_wrap[2]);
    }

    gl::tex_parameter_i(gl_target, gl::texture_parameter_name::texture_min_lod, m_min_lod);
    gl::tex_parameter_i(gl_target, gl::texture_parameter_name::texture_max_lod, m_max_lod);
    gl::tex_parameter_i(gl_target, gl::texture_parameter_name::texture_base_level, m_base_level);
    gl::tex_parameter_i(gl_target, gl::texture_parameter_name::texture_max_level, m_max_level);

    if (configuration::gl_version >= 300) // TODO
    {
        gl::tex_parameter_i(gl_target, gl::texture_parameter_name::texture_compare_mode, m_compare_mode);
        gl::tex_parameter_i(gl_target, gl::texture_parameter_name::texture_compare_func, m_compare_func);
    }

    m_dirty = false;
}

} // namespace graphics
} // namespace renderstack
