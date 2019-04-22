#ifndef log_hpp_renderstack_toolkit
#define log_hpp_renderstack_toolkit

#include "renderstack_toolkit/platform.hpp"

namespace renderstack
{
namespace toolkit
{

#define LOG_COLORIZER_DEFAULT 0
#define LOG_COLORIZER_GLSL 1

struct log_category
{
    int colorizer;
    int color[2];
    int level;
    int indent;

    log_category(int colorizer_, int color0, int color1, int level_)
    {
        colorizer = colorizer_;
        color[0]  = color0;
        color[1]  = color1;
        level     = level_;
    }
};

extern log_category log_services;
extern log_category log_gl;

extern void set_text_color(int c);
extern void console_init();
extern void log_write(log_category *cat, bool indent, int level, const char *format, ...);
extern void log_indent(int indent_amount);

class log_indenter
{
private:
    int m_amount;

public:
    log_indenter(int amount = 3)
        : m_amount(amount)
    {
        log_indent(m_amount);
    }
    ~log_indenter()
    {
        log_indent(-m_amount);
    }
};

extern renderstack::toolkit::log_category log_file;

} // namespace toolkit
} // namespace renderstack

#define C_DARK_BLUE 1
#define C_DARK_GREEN 2
#define C_DARK_RED 4
#define C_DARK_CYAN 1 | 2
#define C_DARK_MAGENTA 1 | 4
#define C_DARK_YELLOW 2 | 4
#define C_BLUE 1 | 8
#define C_GREEN 2 | 8
#define C_RED 4 | 8
#define C_CYAN 1 | 2 | 8
#define C_MAGENTA 1 | 4 | 8
#define C_YELLOW 2 | 4 | 8
#define C_DARK_GRAY 8
#define C_DARK_GREY 8
#define C_GRAY 1 | 2 | 4
#define C_GREY 1 | 2 | 4
#define C_WHITE 1 | 2 | 4 | 8

#define LOG_OFF 0
#define LOG_TRACE 1
#define LOG_INFO 2
#define LOG_WARN 3
#define LOG_ERROR 4

#define log_trace(...) log_write(LOG_CATEGORY, true, LOG_TRACE, __VA_ARGS__)
#define log_info(...) log_write(LOG_CATEGORY, true, LOG_INFO, __VA_ARGS__)
#define log_warn(...) log_write(LOG_CATEGORY, true, LOG_WARN, __VA_ARGS__)
#define log_error(...) log_write(LOG_CATEGORY, true, LOG_ERROR, __VA_ARGS__)
#define log_trace_ni(...) log_write(LOG_CATEGORY, false, LOG_TRACE, __VA_ARGS__)
#define log_info_ni(...) log_write(LOG_CATEGORY, false, LOG_INFO, __VA_ARGS__)
#define log_warn_ni(...) log_write(LOG_CATEGORY, false, LOG_WARN, __VA_ARGS__)
#define log_error_ni(...) log_write(LOG_CATEGORY, false, LOG_ERROR, __VA_ARGS__)
#define slog_trace(...)                                    \
    log_write(LOG_CATEGORY, true, LOG_TRACE, __VA_ARGS__); \
    renderstack::toolkit::log_indenter scope
#define slog_info(...)                                    \
    log_write(LOG_CATEGORY, true, LOG_INFO, __VA_ARGS__); \
    renderstack::toolkit::log_indenter scope
#define slog_warn(...)                                    \
    log_write(LOG_CATEGORY, true, LOG_WARN, __VA_ARGS__); \
    renderstack::toolkit::log_indenter scope
#define slog_error(...)                                    \
    log_write(LOG_CATEGORY, true, LOG_ERROR, __VA_ARGS__); \
    renderstack::toolkit::log_indenter scope

#endif
