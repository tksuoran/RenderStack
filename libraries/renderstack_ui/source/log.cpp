#include "renderstack_toolkit/platform.hpp"
#include "renderstack_ui/log.hpp"

namespace renderstack { namespace ui {

renderstack::toolkit::log_category log_gui_renderer      (C_GREEN,   C_GRAY, LOG_WARN);
renderstack::toolkit::log_category log_button            (C_YELLOW,  C_GRAY, LOG_WARN);
renderstack::toolkit::log_category log_ninepatch         (C_YELLOW,  C_GRAY, LOG_WARN);
renderstack::toolkit::log_category log_ninepatch_style   (C_YELLOW,  C_GRAY, LOG_WARN);
renderstack::toolkit::log_category log_text_buffer       (C_GREEN,   C_GRAY, LOG_WARN);
renderstack::toolkit::log_category log_font              (C_YELLOW,  C_GRAY, LOG_WARN);

} }
