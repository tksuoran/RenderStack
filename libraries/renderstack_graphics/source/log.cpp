#include "renderstack_toolkit/platform.hpp"
#include "renderstack_graphics/log.hpp"

namespace renderstack { namespace graphics {

renderstack::toolkit::log_category log_buffer                  (C_YELLOW,  C_GRAY, LOG_INFO);
renderstack::toolkit::log_category log_configuration           (C_GREEN,   C_GRAY, LOG_INFO);
renderstack::toolkit::log_category log_program                 (C_YELLOW,  C_GRAY, LOG_INFO);
renderstack::toolkit::log_category log_renderer                (C_YELLOW,  C_GRAY, LOG_INFO);
renderstack::toolkit::log_category log_vertex_stream           (C_GREEN,   C_GRAY, LOG_INFO);
renderstack::toolkit::log_category log_vertex_stream_mappings  (C_GREEN,   C_GRAY, LOG_INFO);

} }
