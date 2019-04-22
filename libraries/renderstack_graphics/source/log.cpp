#include "renderstack_graphics/log.hpp"
#include "renderstack_toolkit/platform.hpp"

namespace renderstack
{
namespace graphics
{

renderstack::toolkit::log_category log_buffer(LOG_COLORIZER_DEFAULT, C_YELLOW, C_GRAY, LOG_INFO);
renderstack::toolkit::log_category log_configuration(LOG_COLORIZER_DEFAULT, C_GREEN, C_GRAY, LOG_INFO);
renderstack::toolkit::log_category log_program(LOG_COLORIZER_DEFAULT, C_YELLOW, C_GRAY, LOG_INFO);
renderstack::toolkit::log_category log_glsl(LOG_COLORIZER_GLSL, C_YELLOW, C_GRAY, LOG_INFO);
renderstack::toolkit::log_category log_renderer(LOG_COLORIZER_DEFAULT, C_YELLOW, C_GRAY, LOG_INFO);
renderstack::toolkit::log_category log_vertex_stream(LOG_COLORIZER_DEFAULT, C_GREEN, C_GRAY, LOG_INFO);
renderstack::toolkit::log_category log_vertex_attribute_mappings(LOG_COLORIZER_DEFAULT, C_GREEN, C_GRAY, LOG_INFO);
renderstack::toolkit::log_category log_fragment_outputs(LOG_COLORIZER_DEFAULT, C_GREEN, C_GRAY, LOG_INFO);
renderstack::toolkit::log_category log_load_png(LOG_COLORIZER_DEFAULT, C_CYAN, C_GRAY, LOG_INFO);

} // namespace graphics
} // namespace renderstack
