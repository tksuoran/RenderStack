#include "main/log.hpp"
#include "renderstack_toolkit/platform.hpp"

renderstack::toolkit::log_category log_game(LOG_COLORIZER_DEFAULT, C_CYAN, C_GRAY, LOG_INFO);
renderstack::toolkit::log_category log_menu(LOG_COLORIZER_DEFAULT, C_WHITE, C_GRAY, LOG_INFO);
renderstack::toolkit::log_category log_programs(LOG_COLORIZER_DEFAULT, C_WHITE, C_GRAY, LOG_WARN);
renderstack::toolkit::log_category log_textures(LOG_COLORIZER_DEFAULT, C_WHITE, C_GRAY, LOG_WARN);
renderstack::toolkit::log_category log_debug(LOG_COLORIZER_DEFAULT, C_WHITE, C_GRAY, LOG_WARN);
renderstack::toolkit::log_category log_renderer(LOG_COLORIZER_DEFAULT, C_GREEN, C_GRAY, LOG_INFO);
