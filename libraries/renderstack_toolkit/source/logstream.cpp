#include "renderstack_toolkit/platform.hpp"
#include "renderstack_toolkit/logstream.hpp"
#if defined(WIN32)
# include "renderstack_toolkit/debugstream.hpp"
#else
# include <stdio.h>
# include <unistd.h>
#endif
#include <iostream>
#include <cstdarg>

#if defined(WIN32) && 0
debugstream s_debug;
#endif

std::basic_ostream<char, std::char_traits<char> > &log()
{
#if defined(WIN32) && !defined(NDEBUG) && 0
   return s_debug;
#else
   return std::cout;
#endif
}


#if defined _WIN32
# include <windows.h>
void set_text_color(int c)
{
   HANDLE hConsoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
   SetConsoleTextAttribute(hConsoleHandle, static_cast<WORD>(c));
}
void console_init()
{
   HWND hwnd = GetConsoleWindow();
   HICON icon = LoadIcon(NULL, MAKEINTRESOURCE(32516));
   HANDLE hConsoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
   DWORD mode = 0;
   GetConsoleMode(hConsoleHandle, &mode);
   SetConsoleMode(hConsoleHandle, (mode & ~ENABLE_MOUSE_INPUT) | ENABLE_QUICK_EDIT_MODE | ENABLE_EXTENDED_FLAGS);

   SendMessage(hwnd, WM_SETICON, ICON_BIG, (LPARAM)(icon));
   SendMessage(hwnd, WM_SETICON, ICON_SMALL, (LPARAM)(icon));
}
#else
void set_text_color(int c)
{
#if defined(__APPLE__)
   (void)c;
#else
   if (!::isatty(fileno(stdout)))
      return;
   switch(c)
   {
   case C_DARK_BLUE   : printf("\033[22;34m"); break;
   case C_DARK_GREEN  : printf("\033[22;32m"); break;
   case C_DARK_RED    : printf("\033[22;31m"); break;
   case C_DARK_CYAN   : printf("\033[22;36m"); break;
   case C_DARK_MAGENTA: printf("\033[22;35m"); break;
   case C_DARK_YELLOW : printf("\033[22;33m"); break;
   case C_BLUE        : printf("\033[1;34m"); break;
   case C_GREEN       : printf("\033[1;32m"); break;
   case C_RED         : printf("\033[1;31m"); break;
   case C_CYAN        : printf("\033[1;36m"); break;
   case C_MAGENTA     : printf("\033[1;35m"); break;
   case C_YELLOW      : printf("\033[1;33m"); break;
   case C_DARK_GREY   : printf("\033[1;30m"); break;
   case C_GREY        : printf("\033[22;37m"); break;
   case C_WHITE       : printf("\033[1;37m"); break;
   }
#endif
}
void console_init()
{
}
#endif
static int s_log_indent = 0;
void log_indent(int indent_amount)
{
   s_log_indent += indent_amount;
}
void log_write(log_category *cat, int level, const char *format, ...)
{
   char     buf[4096];
   char     *p = buf;
   char     *span;
   char     c, next, prev;
   va_list  args;
   int      n;

   if (level < cat->level)
      return;

   for (n = 0; n < s_log_indent; ++n)
      *p++ = ' ';

   va_start(args, format);
   n = vsnprintf(p, sizeof(buf) - 3 - cat->indent, format, args);
   va_end(args);

   p += (n < 0) ? sizeof(buf) - 3 : static_cast<unsigned int>(n);

   // Fill end with 0
   while ((p > buf) && isspace(p[-1]))
      *--p = '\0';

   // Put newline to put
# if defined(_WIN32)
   *p++ = '\r';
# endif
   *p++ = '\n';
   *p   = '\0';

   set_text_color(cat->color[0]);
   p = span = buf;
   prev = 0;
   next = 0;
   for (;;)
   {
      c = *p;
      p++;
      next = (c != 0) ? (*p) : 0;
      if (c == '(' || (c == ':' && next != ':' && prev != ':'))
      {
         prev = c;
         c = *p;
         *p = 0;
         std::cout << span;
         std::cout.flush();
         *p = c;
         span = p;
         set_text_color(cat->color[1]);
      }
      else if (c == ')')      
      {
         prev = c;
         --p;
         c = *p;
         *p = 0;
         std::cout << span;
         std::cout.flush();
         *p = c;
         span = p;
         set_text_color(cat->color[0]);
         ++p;
      }
      else if (c == 0)
      {
         std::cout << span;
         std::cout.flush();
         set_text_color(cat->color[1]);
         break;
      }
      else
      {
         prev = c;
      }
   }
   set_text_color(C_GRAY);

# if defined(_WIN32)
   ::OutputDebugStringA(buf);
# endif
}


log_category log_graphics_buffer                (C_YELLOW,  C_GRAY, LOG_INFO);
log_category log_graphics_configuration         (C_GREEN,   C_GRAY, LOG_INFO);
log_category log_graphics_program               (C_YELLOW,  C_GRAY, LOG_INFO);
log_category log_graphics_vertex_stream         (C_GREEN,   C_GRAY, LOG_WARN);
log_category log_graphics_vertex_stream_mappings(C_GREEN,   C_GRAY, LOG_WARN);
log_category log_mesh_geometry_mesh             (C_RED,     C_GRAY, LOG_WARN);
log_category log_mesh_mesh                      (C_MAGENTA, C_GRAY, LOG_WARN);
log_category log_graphics_renderer              (C_YELLOW,  C_GRAY, LOG_INFO);
log_category log_ui_gui_renderer                (C_GREEN,   C_GRAY, LOG_WARN);
log_category log_ui_button                      (C_YELLOW,  C_GRAY, LOG_WARN);
log_category log_ui_ninepatch                   (C_YELLOW,  C_GRAY, LOG_WARN);
log_category log_ui_ninepatch_style             (C_YELLOW,  C_GRAY, LOG_WARN);
log_category log_ui_text_buffer                 (C_GREEN,   C_GRAY, LOG_WARN);


