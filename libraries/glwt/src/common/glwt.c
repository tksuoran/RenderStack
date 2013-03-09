#include <stdarg.h>
#include <stdio.h>

#include <GLWT/glwt.h>
#include <glwt_internal.h>

struct glwt glwt;

int glwtErrorPrintf(const char *fmt, ...)
{
    va_list ap;
    int size;
    int val;
    va_start(ap, fmt);
    size = vsnprintf(NULL, 0, fmt, ap);
    va_end(ap);

    {
       char buffer[4096];

       va_start(ap, fmt);
       val = vsnprintf(buffer, size+1, fmt, ap);
       va_end(ap);

       if(glwt.app_callbacks.error_callback)
           glwt.app_callbacks.error_callback(buffer, glwt.app_callbacks.userdata);
       else
           fprintf(stderr, "%s\n", buffer);
    }

    return val;
}

int glwtWindowClosed(GLWTWindow *window)
{
    return window->closed;
}
