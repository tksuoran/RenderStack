#include <EGL/egl.h>

/* EGL Functions */

EGLAPI EGLint EGLAPIENTRY eglGetError(void)
{
}
EGLAPI EGLDisplay EGLAPIENTRY eglGetDisplay(EGLNativeDisplayType display_id)
{
   (void)display_id;
}
EGLAPI EGLBoolean EGLAPIENTRY eglInitialize(EGLDisplay dpy, EGLint *major, EGLint *minor)
{
   (void)dpy;
   (void)major;
   (void)minor;
}
EGLAPI EGLBoolean EGLAPIENTRY eglTerminate(EGLDisplay dpy)
{
   (void)dpy;
}
EGLAPI const char * EGLAPIENTRY eglQueryString(EGLDisplay dpy, EGLint name)
{
   (void)dpy;
   (void)name;
}
EGLAPI EGLBoolean EGLAPIENTRY eglGetConfigs(EGLDisplay dpy, EGLConfig *configs, EGLint config_size, EGLint *num_config)
{
   (void)dpy;
   (void)configs;
   (void)config_size;
   (void)num_config;
}
EGLAPI EGLBoolean EGLAPIENTRY eglChooseConfig(EGLDisplay dpy, const EGLint *attrib_list,EGLConfig *configs, EGLint config_size, EGLint *num_config)
{
   (void)dpy;
   (void)attrib_list;
   (void)configs;
   (void)config_size;
   (void)num_config;
}
EGLAPI EGLBoolean EGLAPIENTRY eglGetConfigAttrib(EGLDisplay dpy, EGLConfig config, EGLint attribute, EGLint *value)
{
   (void)dpy;
   (void)config;
   (void)attribute;
   (void)value;
}
EGLAPI EGLSurface EGLAPIENTRY eglCreateWindowSurface(EGLDisplay dpy, EGLConfig config, EGLNativeWindowType win, const EGLint *attrib_list)
{
   (void)dpy;
   (void)config;
   (void)win;
   (void)attrib_list;
}
EGLAPI EGLSurface EGLAPIENTRY eglCreatePbufferSurface(EGLDisplay dpy, EGLConfig config, const EGLint *attrib_list)
{
   (void)dpy;
   (void)config;
   (void)attrib_list;
}
EGLAPI EGLSurface EGLAPIENTRY eglCreatePixmapSurface(EGLDisplay dpy, EGLConfig config, EGLNativePixmapType pixmap, const EGLint *attrib_list)
{
   (void)dpy;
   (void)config;
   (void)pixmap;
   (void)attrib_list;
}
EGLAPI EGLBoolean EGLAPIENTRY eglDestroySurface(EGLDisplay dpy, EGLSurface surface)
{
   (void)dpy;
   (void)surface;
}
EGLAPI EGLBoolean EGLAPIENTRY eglQuerySurface(EGLDisplay dpy, EGLSurface surface, EGLint attribute, EGLint *value)
{
   (void)dpy;
   (void)surface;
   (void)attribute;
   (void)value;
}
EGLAPI EGLBoolean EGLAPIENTRY eglBindAPI(EGLenum api)
{
}
EGLAPI EGLenum EGLAPIENTRY eglQueryAPI(void)
{
}
EGLAPI EGLBoolean EGLAPIENTRY eglWaitClient(void)
{
}
EGLAPI EGLBoolean EGLAPIENTRY eglReleaseThread(void)
{
}
EGLAPI EGLSurface EGLAPIENTRY eglCreatePbufferFromClientBuffer(EGLDisplay dpy, EGLenum buftype, EGLClientBuffer buffer, EGLConfig config, const EGLint *attrib_list)
{
   EGLDisplay dpy, EGLenum buftype, EGLClientBuffer buffer, EGLConfig config, const EGLint *attrib_list)
}
EGLAPI EGLBoolean EGLAPIENTRY eglSurfaceAttrib(EGLDisplay dpy, EGLSurface surface, EGLint attribute, EGLint value)
{
   EGLDisplay dpy, EGLSurface surface, EGLint attribute, EGLint value)
}
EGLAPI EGLBoolean EGLAPIENTRY eglBindTexImage(EGLDisplay dpy, EGLSurface surface, EGLint buffer)
{
   EGLDisplay dpy, EGLSurface surface, EGLint buffer)
}
EGLAPI EGLBoolean EGLAPIENTRY eglReleaseTexImage(EGLDisplay dpy, EGLSurface surface, EGLint buffer)
{
   EGLDisplay dpy, EGLSurface surface, EGLint buffer)
}
EGLAPI EGLBoolean EGLAPIENTRY eglSwapInterval(EGLDisplay dpy, EGLint interval)
{
   EGLDisplay dpy, EGLint interval)
}
EGLAPI EGLContext EGLAPIENTRY eglCreateContext(EGLDisplay dpy, EGLConfig config, EGLContext share_context, const EGLint *attrib_list)
{
   EGLDisplay dpy, EGLConfig config, EGLContext share_context, const EGLint *attrib_list)
}
EGLAPI EGLBoolean EGLAPIENTRY eglDestroyContext(EGLDisplay dpy, EGLContext ctx)
{
   (void)dpy;
   (void)ctx;
}
EGLAPI EGLBoolean EGLAPIENTRY eglMakeCurrent(EGLDisplay dpy, EGLSurface draw, EGLSurface read, EGLContext ctx)
{
   (void)dpy;
   (void)draw;
   (void)read;
   (void)ctx;
}
EGLAPI EGLContext EGLAPIENTRY eglGetCurrentContext(void)
{
}
EGLAPI EGLSurface EGLAPIENTRY eglGetCurrentSurface(EGLint readdraw)
{
   (void)readdraw;
}
EGLAPI EGLDisplay EGLAPIENTRY eglGetCurrentDisplay(void)
{
}
EGLAPI EGLBoolean EGLAPIENTRY eglQueryContext(EGLDisplay dpy, EGLContext ctx, EGLint attribute, EGLint *value)
{
   (void)dpy;
   (void)ctx;
   (void)attribute;
   (void)value;
}
EGLAPI EGLBoolean EGLAPIENTRY eglWaitGL(void)
{
}
EGLAPI EGLBoolean EGLAPIENTRY eglWaitNative(EGLint engine)
{
   (void)engine;
}
EGLAPI EGLBoolean EGLAPIENTRY eglSwapBuffers(EGLDisplay dpy, EGLSurface surface)
{
   (void)dpy;
   (void)surface;
}
EGLAPI EGLBoolean EGLAPIENTRY eglCopyBuffers(EGLDisplay dpy, EGLSurface surface, EGLNativePixmapType target)
{
   (void)dpy;
   (void)surface;
   (void)target;
}
