#include <glwt_internal.h>

int glwtWindowCreateGLX(GLWTWindow *win, GLWTWindow *share)
{
    if((win->glx.surface = glXCreateWindow(
        glwt.x11.display,
        glwt.glx.fbconfig,
        win->x11.window, 0)) == 0)
    {
        glwtErrorPrintf("GLX Surface create failed");
        goto error;
    }

    const int context_attribs[] = {
        GLX_CONTEXT_MAJOR_VERSION_ARB, glwt.api_version_major != 0 ? glwt.api_version_major : 1,
        GLX_CONTEXT_MINOR_VERSION_ARB, glwt.api_version_minor,
        GLX_CONTEXT_PROFILE_MASK_ARB,
            (glwt.api & GLWT_PROFILE_COMPATIBILITY) ?
                GLX_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB :
                GLX_CONTEXT_CORE_PROFILE_BIT_ARB,
        GLX_CONTEXT_FLAGS_ARB,
            (glwt.api & GLWT_PROFILE_DEBUG) ? GLX_CONTEXT_DEBUG_BIT_ARB : 0 |
            (glwt.api & GLWT_PROFILE_ROBUSTNESS) ? GLX_CONTEXT_ROBUST_ACCESS_BIT_ARB : 0,
        0, 0
    };

    if((win->glx.context = glXCreateContextAttribsARB(
        glwt.x11.display,
        glwt.glx.fbconfig,
        share ? share->glx.context : 0,
        True, // direct
        context_attribs)) == 0)
    {
        glwtErrorPrintf("GLX context create failed");
        goto error;
    }

    return 0;
error:
    glwtWindowDestroyGLX(win);
    return -1;
}

void glwtWindowDestroyGLX(GLWTWindow *win)
{
    if(win->glx.surface)
        glXDestroyWindow(glwt.x11.display, win->glx.surface);
    if(win->glx.context)
        glXDestroyContext(glwt.x11.display, win->glx.context);
}

void glwtMakeCurrent(GLWTWindow *win)
{
    if(win)
        glXMakeContextCurrent(
            glwt.x11.display,
            win->glx.surface,
            win->glx.surface,
            win->glx.context);
    else
        glXMakeContextCurrent(glwt.x11.display, 0, 0, 0);
}

void glwtSwapBuffers(GLWTWindow *win)
{
    glXSwapBuffers(glwt.x11.display, win->glx.surface);
}

void glwtSwapInterval(GLWTWindow *win, int interval)
{
    glXSwapIntervalEXT(glwt.x11.display, win->glx.surface, interval);
}

void glwtWindowGetSize(GLWTWindow *win, int *width, int *height)
{
    unsigned int w, h;
    glXQueryDrawable(glwt.x11.display, win->glx.surface, GLX_WIDTH, &w);
    glXQueryDrawable(glwt.x11.display, win->glx.surface, GLX_HEIGHT, &h);
    *width = (int)w; *height = (int)h;
}
