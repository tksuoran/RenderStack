#include "renderstack_toolkit/log.hpp"
#include "renderstack_toolkit/platform.hpp"
#include <cassert>
#include <cstdarg>
#include <iostream>

#if defined _WIN32
#    include <windows.h>
#else
#    include <unistd.h>
#endif

namespace renderstack
{
namespace toolkit
{

log_category log_services(LOG_COLORIZER_DEFAULT, C_WHITE, C_GRAY, LOG_INFO);
log_category log_gl(LOG_COLORIZER_DEFAULT, C_DARK_GREEN, C_GRAY, LOG_TRACE);
log_category log_file(LOG_COLORIZER_DEFAULT, C_CYAN, C_GRAY, LOG_TRACE);

using namespace std;

#if defined _WIN32
bool        print_color()
{
    return true;
}
void set_text_color(int c)
{
    HANDLE hConsoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsoleHandle, static_cast<WORD>(c));
}
void console_init()
{
    HWND   hwnd           = GetConsoleWindow();
    HICON  icon           = LoadIcon(NULL, MAKEINTRESOURCE(32516));
    HANDLE hConsoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD  mode           = 0;
    GetConsoleMode(hConsoleHandle, &mode);
    SetConsoleMode(hConsoleHandle, (mode & ~ENABLE_MOUSE_INPUT) | ENABLE_QUICK_EDIT_MODE | ENABLE_EXTENDED_FLAGS);

    SendMessage(hwnd, WM_SETICON, ICON_BIG, (LPARAM)(icon));
    SendMessage(hwnd, WM_SETICON, ICON_SMALL, (LPARAM)(icon));

    FILE *l = fopen("log.txt", "wb");
    if (l)
    {
        fprintf(l, "");
        fclose(l);
    }
}
#else
bool print_color()
{
#    if defined(__APPLE__)
    return false;
#    else
    return ::isatty(fileno(stdout));
#    endif
}
void set_text_color(int c)
{
#    if defined(__APPLE__)
    (void)c;
#    else
    switch (c)
    {
        case C_DARK_BLUE: printf("\033[22;34m"); break;
        case C_DARK_GREEN: printf("\033[22;32m"); break;
        case C_DARK_RED: printf("\033[22;31m"); break;
        case C_DARK_CYAN: printf("\033[22;36m"); break;
        case C_DARK_MAGENTA: printf("\033[22;35m"); break;
        case C_DARK_YELLOW: printf("\033[22;33m"); break;
        case C_BLUE: printf("\033[1;34m"); break;
        case C_GREEN: printf("\033[1;32m"); break;
        case C_RED: printf("\033[1;31m"); break;
        case C_CYAN: printf("\033[1;36m"); break;
        case C_MAGENTA: printf("\033[1;35m"); break;
        case C_YELLOW: printf("\033[1;33m"); break;
        case C_DARK_GREY: printf("\033[1;30m"); break;
        case C_GREY: printf("\033[22;37m"); break;
        case C_WHITE: printf("\033[1;37m"); break;
    }
#    endif
}
void console_init()
{
    FILE *l = fopen("log.txt", "wb");
    if (l)
    {
        fclose(l);
    }
}
#endif

static int s_log_indent = 0;

void log_indent(int indent_amount)
{
    s_log_indent += indent_amount;
}

void log_write(log_category *cat, bool indent, int level, const char *format, ...)
{
    char    buf[16384];
    char *  p = buf;
    char *  span;
    char    c, next, prev;
    va_list args;
    int     n;

    if (level < cat->level)
    {
        return;
    }

    if (indent)
    {
        for (n = 0; n < s_log_indent; ++n)
        {
            *p++ = ' ';
        }
    }

    va_start(args, format);
    vsnprintf(p, sizeof(buf) - 3 - cat->indent, format, args);
    va_end(args);

#if 0
    p += (n < 0) ? sizeof(buf) - 3 : static_cast<unsigned int>(n);

    // Fill end with 0
    while ((p > buf) && isspace(p[-1]) && (p[-1] != '\n'))
    {
        *--p = '\0';
    }
#endif

#if 0
    // Put newline to put
#    if defined(_WIN32)
    *p++ = '\r';
#    endif
    *p++ = '\n';
    *p   = '\0';
#endif

    // Log to console
    if (print_color())
    {
        switch (cat->colorizer)
        {
            case LOG_COLORIZER_DEFAULT:
            {
                set_text_color(cat->color[0]);
                p = span = buf;
                prev     = 0;
                for (;;)
                {
                    c = *p;
                    p++;
                    next = (c != 0) ? (*p) : 0;
                    if (c == '(' || (c == ':' && next != ':' && prev != ':'))
                    {
                        prev = c;
                        c    = *p;
                        *p   = 0;
                        cout << span;
                        cout.flush();
                        *p   = c;
                        span = p;
                        set_text_color(cat->color[1]);
                    }
                    else if (c == ')')
                    {
                        prev = c;
                        --p;
                        c  = *p;
                        *p = 0;
                        cout << span;
                        cout.flush();
                        *p   = c;
                        span = p;
                        set_text_color(cat->color[0]);
                        ++p;
                    }
                    else if (c == 0)
                    {
                        cout << span;
                        cout.flush();
                        set_text_color(cat->color[1]);
                        break;
                    }
                    else
                    {
                        prev = c;
                    }
                }
                break;
            }

            case LOG_COLORIZER_GLSL:
            {
                set_text_color(cat->color[0]);
                p = span = buf;
                for (;;)
                {
                    c = *p;
                    p++;
                    if (c == ':')
                    {
                        c  = *p;
                        *p = 0;
                        cout << span;
                        cout.flush();
                        *p   = c;
                        span = p;
                        set_text_color(cat->color[1]);
                    }
                    else if (c == '\n')
                    {
                        c  = *p;
                        *p = 0;
                        cout << span;
                        cout.flush();
                        *p   = c;
                        span = p;
                        set_text_color(cat->color[0]);
                    }
                    else if (c == 0)
                    {
                        cout << span;
                        cout.flush();
                        set_text_color(cat->color[1]);
                        break;
                    }
                }
                break;
            }

            default:
            {
                assert(0);
                break;
            }
        }
        set_text_color(C_GRAY);
    }

    // Log to file
    FILE *l = fopen("log.txt", "ab+");
    if (l)
    {
        fprintf(l, "%s", buf);
        fclose(l);
    }

    // Log to debugger
#if defined(_WIN32)
    ::OutputDebugStringA(buf);
#endif
#if defined(__APPLE__)
    printf("%s", buf);
#endif
}

} // namespace toolkit
} // namespace renderstack
