#include "renderstack_ui/ninepatch_style.hpp"
#include "renderstack_graphics/buffer.hpp"
#include "renderstack_graphics/load_png.hpp"
#include "renderstack_graphics/renderer.hpp"
#include "renderstack_graphics/texture.hpp"
#include "renderstack_toolkit/gl.hpp"
#include "renderstack_toolkit/platform.hpp"
#include "renderstack_toolkit/strong_gl_enums.hpp"
#include "renderstack_ui/log.hpp"
#include "renderstack_ui/style.hpp"
#include <stdexcept>

#define LOG_CATEGORY &log_ninepatch_style

namespace renderstack
{
namespace ui
{

using namespace renderstack::graphics;

//mesh *Ninepatch::s_shared_indices = nullptr
Ninepatch_style::Ninepatch_style(Renderer          &renderer,
                                 const std::string &path,
                                 Program           *program,
                                 int               texture_unit)
    : program(program)
    , texture_unit(texture_unit)
    , border_uv(0.25f, 0.25f)
{
    slog_trace("Ninepatch_style::Ninepatch_style(path = %s)\n", path.c_str());

    if (path.length() > 0)
    {
        texture = std::make_unique<Texture>();
        bool ok = load_png(*(texture.get()), renderer, texture_unit, path);
        if (ok)
        {
            texture->set_debug_label(path);
            texture->set_min_filter(gl::texture_min_filter::nearest);
            texture->set_mag_filter(gl::texture_mag_filter::nearest);

            border_pixels.x = border_uv.x * static_cast<float>(texture->width());
            border_pixels.y = border_uv.y * static_cast<float>(texture->height());
        }
    }
}

} // namespace ui
} // namespace renderstack
