#include "renderstack_ui/log.hpp"
#include "renderstack_toolkit/platform.hpp"

namespace renderstack
{
namespace ui
{

renderstack::toolkit::log_category log_gui_renderer(LOG_COLORIZER_DEFAULT, C_GREEN, C_GRAY, LOG_WARN);
renderstack::toolkit::log_category log_button(LOG_COLORIZER_DEFAULT, C_YELLOW, C_GRAY, LOG_WARN);
renderstack::toolkit::log_category log_ninepatch(LOG_COLORIZER_DEFAULT, C_YELLOW, C_GRAY, LOG_WARN);
renderstack::toolkit::log_category log_ninepatch_style(LOG_COLORIZER_DEFAULT, C_YELLOW, C_GRAY, LOG_WARN);
renderstack::toolkit::log_category log_text_buffer(LOG_COLORIZER_DEFAULT, C_GREEN, C_GRAY, LOG_WARN);
renderstack::toolkit::log_category log_font(LOG_COLORIZER_DEFAULT, C_YELLOW, C_GRAY, LOG_WARN);
renderstack::toolkit::log_category log_layout(LOG_COLORIZER_DEFAULT, C_YELLOW, C_GRAY, LOG_WARN);

} // namespace ui
} // namespace renderstack
