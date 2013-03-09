#include <glwt_internal.h>

GLWTWindow *glwtWindowCreate(
    const char *title,
    int width, int height,
    const GLWTWindowCallbacks *win_callbacks,
    GLWTWindow *share)
{
    GLWTWindow *win;
    RECT rect;
    WCHAR nullterm = 0;
    intptr_t classptr;
    PIXELFORMATDESCRIPTOR pfd;
    int style = WS_OVERLAPPEDWINDOW | WS_THICKFRAME;
    int exstyle = WS_EX_OVERLAPPEDWINDOW;

    win = malloc(sizeof(GLWTWindow));
    if(!win)
        return 0;

    if(win_callbacks)
        win->win_callbacks = *win_callbacks;

    rect.left = rect.top = 0;
    rect.right = width;
    rect.bottom = height;

    if(!AdjustWindowRectEx(&rect, style, 0, exstyle))
    {
        glwtWin32Error("AdjustWindowRect failed");
        goto error;
    }

    classptr = glwt.win32.classatom;
    win->win32.hwnd = CreateWindowExW(
        exstyle,
        (LPCWSTR)classptr,
        &nullterm,  // window title
        style,
        CW_USEDEFAULT, CW_USEDEFAULT,
        rect.right - rect.left, rect.bottom - rect.top,
        0,
        0,
        glwt.win32.hinstance,
        0);
    if(!win->win32.hwnd)
    {
        glwtWin32Error("CreateWindowExW failed");
        goto error;
    }

    //SetWindowLongPtr(win->win32.hwnd, GWLP_USERDATA, (LONG_PTR)win);
    //SetWindowLong(win->win32.hwnd, GWLP_USERDATA, (LONG)win);
    SetWindowLong(win->win32.hwnd, 0, (LONG)win);
    /*
    if()
    {
        glwtWin32Error("SetWindowLongPtr failed");
        goto error;
    }
    */

    win->win32.hdc = GetDC(win->win32.hwnd);
    if(!win->win32.hdc)
    {
        glwtWin32Error("GetDC failed");
        goto error;
    }

    if(!SetPixelFormat(win->win32.hdc, glwt.win32.pixel_format, &pfd))
    {
        glwtWin32Error("SetPixelFormat failed");
        goto error;
    }

#ifdef GLWT_USE_EGL
    if(glwtWindowCreateEGL(win, share))
#else
    if(glwtWindowCreateWGL(win, share))
#endif
        goto error;

    (void)title;

    return win;
error:
    glwtWindowDestroy(win);
    return 0;
}

void glwtWindowDestroy(GLWTWindow *win)
{
    if(!win)
        return;

    glwtWindowDestroyWGL(win);

    if(win->win32.hdc)
        ReleaseDC(win->win32.hwnd, win->win32.hdc);

    if(win->win32.hwnd)
        DestroyWindow(win->win32.hwnd);

    free(win);
}

void glwtWindowShow(GLWTWindow *win, int show)
{
    ShowWindow(win->win32.hwnd, show ? SW_SHOWNORMAL : SW_HIDE);
}
