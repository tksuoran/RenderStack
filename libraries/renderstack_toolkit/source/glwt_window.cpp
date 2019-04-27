#if defined(RENDERSTACK_USE_GLWT)
#    include "renderstack_toolkit/platform.hpp"
#    include "renderstack_toolkit/window.hpp"
#    include <GLWT/glwt.h>
//#include <GLXW/glxw.h>
#    include <stdexcept>

#    if defined(RENDERSTACK_GL_API_OPENGL_ES_2) || defined(RENDERSTACK_GL_API_OPENGL_ES_3)
#        include <EGL/egl.h>
#    endif

#    if defined(_WIN32)
typedef DWORD(WINAPI *PFNTIMEGETTIME)(void);
#    endif

#    if defined(__APPLE__)
#        include <cstdlib>
#        include <cstring>
#        include <mach-o/dyld.h>
#    endif

#    include <cassert>

using namespace std;
#    if 0
#        if defined(_WIN32)

#            pragma warning(disable : 4074) //initializers put in compiler reserved initialization area
#            pragma init_seg(compiler)      //global objects in this file get constructed very early on

struct CrtBreakAllocSetter {
    CrtBreakAllocSetter() {
        _crtBreakAlloc = 185;
    }
};

CrtBreakAllocSetter g_crtBreakAllocSetter;
#        endif
#    endif

void init_memory_system()
{
#    if defined(_WIN32)
    _CrtSetDbgFlag(
        _CRTDBG_ALLOC_MEM_DF |
        _CRTDBG_DELAY_FREE_MEM_DF |
        _CRTDBG_LEAK_CHECK_DF);
    /*
   _CRTDBG_ALLOC_MEM_DF       - Turn on debug allocation
   _CRTDBG_DELAY_FREE_MEM_DF  - Don't actually free memory
   _CRTDBG_CHECK_ALWAYS_DF    - Check heap every alloc/dealloc
   _CRTDBG_RESERVED_DF        - Reserved - do not use
   _CRTDBG_CHECK_CRT_DF       - Leak check/diff CRT blocks DO NOT USE IF YOU DON'T KNOW WHAT YOU ARE DOING
   _CRTDBG_LEAK_CHECK_DF      - Leak check at program exit 
   */
#    endif
}

void check_memory_system()
{
#    if defined(_WIN32)
    if (_CrtCheckMemory() == FALSE)
        throw std::runtime_error("memory corruption");
#    endif
}

#    if defined(_WIN32)
static _CrtMemState initial_memory_state;
#    endif

void begin_memory_compare()
{
#    if defined(_WIN32)
    _CrtMemCheckpoint(&initial_memory_state);
#    endif
}

void end_memory_compare()
{
#    if defined(_WIN32)
    _CrtMemState end_memory_state;
    _CrtMemState diff_memory_state;

    _CrtMemCheckpoint(&end_memory_state);

    (void)/*int res = */ _CrtMemDifference(&diff_memory_state, &initial_memory_state, &end_memory_state);
    _CrtMemDumpStatistics(&diff_memory_state);

    _CrtMemDumpAllObjectsSince(&initial_memory_state);
#    endif
}

namespace renderstack
{
namespace toolkit
{

static void s_error_callback(const char *msg, void *userdata)
{
    (void)userdata;
    printf("%s\n", msg);
}

static void s_win_callback(GLWTWindow *window, const GLWTWindowEvent *event, void *userdata)
{
    (void)window;
    class window *rs_window = static_cast<class window *>(userdata);
    if (rs_window)
        rs_window->glwt_callback(event);
}

void window::glwt_callback(const GLWTWindowEvent *event)
{
#    if 0
    double time = glwtGetTime();

    switch(event->type)
    {
        case GLWT_WINDOW_CLOSE:
            printf("[%lf] Window closed\n", time);
            break;
        case GLWT_WINDOW_EXPOSE:
            printf("[%lf] Window exposed\n", time);
#        if 0
            {
                glwtMakeCurrent(window);
                glwtSwapInterval(window, 1);

                glxwInit();

                glClearColor(0.2f, 0.4f, 0.7f, 1.0f);
                glClear(GL_COLOR_BUFFER_BIT);
                glwtSwapBuffers(window);

                glwtMakeCurrent(NULL);
            }
#        endif
            break;
        case GLWT_WINDOW_RESIZE:
            printf("[%lf] Window resized  width: %d  height: %d\n", time, event->resize.width, event->resize.height);
            break;
        case GLWT_WINDOW_SHOW:
        case GLWT_WINDOW_HIDE:
            printf("[%lf] Window %s\n", time, (event->type == GLWT_WINDOW_SHOW) ? "show" : "hide");
            break;
        case GLWT_WINDOW_FOCUS_IN:
        case GLWT_WINDOW_FOCUS_OUT:
            printf("[%lf] Window focus %s\n", time, (event->type == GLWT_WINDOW_FOCUS_IN) ? "in" : "out");
            break;
        case GLWT_WINDOW_KEY_UP:
        case GLWT_WINDOW_KEY_DOWN:
            printf("[%lf] Key %s  keysym: 0x%x  scancode: %d  mod: %X\n", time,
                (event->type == GLWT_WINDOW_KEY_DOWN) ? "down" : "up",
                event->key.keysym, event->key.scancode, event->key.mod);
            break;
        case GLWT_WINDOW_BUTTON_UP:
        case GLWT_WINDOW_BUTTON_DOWN:
            printf("[%lf] Button %s  x: %d  y: %d  button: %d  mod: %X\n", time,
                (event->type == GLWT_WINDOW_BUTTON_DOWN) ? "down" : "up",
                event->button.x, event->button.y, event->button.button, event->button.mod);
            break;
        case GLWT_WINDOW_MOUSE_MOTION:
            printf("[%lf] Motion  x: %d  y: %d  buttons: %X\n", time,
                event->motion.x, event->motion.y, event->motion.buttons);
            break;
        case GLWT_WINDOW_MOUSE_ENTER:
        case GLWT_WINDOW_MOUSE_LEAVE:
            printf("[%lf] Mouse %s\n", time, (event->type == GLWT_WINDOW_MOUSE_ENTER) ? "enter" : "leave");
            break;
        default:
            break;
    }
#    endif

    switch (event->type)
    {
        //case GLWT_WINDOW_NO_EVENT:
        break;
        case GLWT_WINDOW_CLOSE:
            close();
            break;

        case GLWT_WINDOW_EXPOSE:
            break;
        case GLWT_WINDOW_RESIZE:
            m_width  = event->resize.width;
            m_height = event->resize.height;
            on_resize(m_width, m_height);
            break;
        case GLWT_WINDOW_SHOW:
            break;
        case GLWT_WINDOW_HIDE:
            break;
        case GLWT_WINDOW_FOCUS_IN:
            on_focus(true);
            break;
        case GLWT_WINDOW_FOCUS_OUT:
            on_focus(false);
            break;
        case GLWT_WINDOW_KEY_UP:
            on_key(event->key.keysym, event->key.scancode, 0, event->key.mod);
            break;
        case GLWT_WINDOW_KEY_DOWN:
            on_key(event->key.keysym, event->key.scancode, 1, event->key.mod);
            break;
        case GLWT_WINDOW_BUTTON_UP:
            if (event->button.button < 10)
                m_buttons[event->button.button] = 0;
            if (
                (m_cursor_x != event->button.x) ||
                (m_cursor_y != event->button.y))
            {
                m_cursor_x = event->button.x;
                m_cursor_y = event->button.y;
                on_mouse_moved(m_cursor_x, m_cursor_y);
            }
            on_mouse_button(event->button.button, 0, event->button.mod);
            break;
        case GLWT_WINDOW_BUTTON_DOWN:
            if (event->button.button < 10)
                m_buttons[event->button.button] = 1;
            if (
                (m_cursor_x != event->button.x) ||
                (m_cursor_y != event->button.y))
            {
                m_cursor_x = event->button.x;
                m_cursor_y = event->button.y;
                on_mouse_moved(m_cursor_x, m_cursor_y);
            }
            on_mouse_button(event->button.button, 1, event->button.mod);
            break;
        case GLWT_WINDOW_MOUSE_MOTION:
            m_cursor_x = event->motion.x;
            m_cursor_y = event->motion.y;
            on_mouse_moved(m_cursor_x, m_cursor_y);
            for (int i = 0; i < 10; ++i)
            {
                bool new_button_state = !!(event->motion.buttons & (1 << i));
                if (m_buttons[i] != new_button_state)
                {
                    m_buttons[i] = new_button_state;
                    on_mouse_button(i, new_button_state, 0);
                }
            }
            break;
        case GLWT_WINDOW_MOUSE_ENTER:
        case GLWT_WINDOW_MOUSE_LEAVE:
            break;
    }
}

void window::set_time(double value)
{
    (void)(value); // TODO
}

double window::time() const
{
#    if defined(_WIN32) && 0
    if (m_time_get_time)
    {
        PFNTIMEGETTIME time_get_time = static_cast<PFNTIMEGETTIME>(m_time_get_time);
        winmm_time                   = time_get_time() * 0.001;
        return winmm_time;
    }
    else
#    endif
    {
        double time = glwtGetTime();
        return time;
    }
}

#    if defined(_WIN32)
typedef WINGDIAPI PROC(WINAPI *PFNWGLGETPROCADDRESS)(LPCSTR);
#    endif

window::window()
#    if defined(_WIN32) && 0
    : m_winmm_dll(0), m_time_get_time(nullptr)
#    endif
{
#    if defined(RENDERSTACK_GL_API_OPENGL)
#        if defined(_WIN32)
    HMODULE opengl32_dll = LoadLibraryA("opengl32.dll");
    if (opengl32_dll)
    {
        PFNWGLGETPROCADDRESS wgl_get_proc_address = reinterpret_cast<PFNWGLGETPROCADDRESS>(GetProcAddress(opengl32_dll, "wglGetProcAddress"));
        m_opengl32_dll                            = static_cast<void *>(opengl32_dll);
        m_wgl_get_proc_address                    = static_cast<void *>(wgl_get_proc_address);
    }
    else
    {
        m_wgl_get_proc_address = nullptr;
    }
#        endif
#    endif

#    if defined(_WIN32) && 0
    HMODULE winmm_dll = LoadLibraryA("winmm.dll");
    if (winmm_dll)
    {
        PFNTIMEGETTIME time_get_time = reinterpret_cast<PFNTIMEGETTIME>(GetProcAddress(winmm_dll, "timeGetTime"));
        m_winmm_dll                  = static_cast<void *>(winmm_dll);
        m_time_get_time              = static_cast<void *>(time_get_time);
    }
    else
    {
        m_time_get_time = nullptr;
    }
#    endif
}

bool window::open(int width, int height, const std::string &title, int major, int minor)
{
    ::memset(&m_glwt_config, sizeof(::GLWTConfig), 0);
    m_glwt_config.red_bits          = 8;
    m_glwt_config.green_bits        = 8;
    m_glwt_config.blue_bits         = 8;
    m_glwt_config.alpha_bits        = 0;
    m_glwt_config.depth_bits        = 24;
    m_glwt_config.stencil_bits      = 0;
    m_glwt_config.api               = GLWT_API_ANY;
    m_glwt_config.api_version_major = major;
    m_glwt_config.api_version_minor = minor;

    if (major >= 3)
    {
        m_glwt_config.sample_buffers = 1;
        m_glwt_config.samples        = 4;
    }
    else
    {
        m_glwt_config.sample_buffers = 0;
        m_glwt_config.samples        = 0;
    }

#    if defined(RENDERSTACK_GL_API_OPENGL)
    m_glwt_config.api |= GLWT_API_OPENGL;
#    endif
#    if defined(RENDERSTACK_GL_API_OPENGL_ES_2) || defined(RENDERSTACK_GL_API_OPENGL_ES_3)
    m_glwt_config.api |= GLWT_API_OPENGL_ES;
#    endif

#    if defined(_DEBUG) && !defined(__APPLE__)
    if (major >= 3)
    {
        // TODO request forward compatible
        m_glwt_config.api |= GLWT_PROFILE_DEBUG;
    }
#    endif
    if ((major >= 3) && (minor >= 2))
        m_glwt_config.api |= GLWT_PROFILE_CORE;

    if (::glwtInit(&m_glwt_config, &s_error_callback, NULL) != 0)
        // throw runtime_error("Failed to initialize GLWT");
        return false;

    m_width  = width;
    m_height = height;

    GLWTWindow *win = ::glwtWindowCreate(
        title.c_str(),
        width,
        height,
        nullptr,
        s_win_callback,
        static_cast<void *>(this));
    if (!win)
    {
        ::glwtQuit();
        return false;
        //throw runtime_error("Failed to open GLWT window");
    }

    m_window  = win;
    m_running = true;
    m_capture = false;
    m_show    = true;

    m_cursor_x = -1;
    m_cursor_y = -1;

    ::glwtWindowShow(win, 1);
    ::glwtMakeCurrent(win);
    ::glwtSwapInterval(win, 1);

    get_extensions();

    return true;
}

void window::make_current()
{
    assert(m_window);
    ::glwtMakeCurrent(static_cast<GLWTWindow *>(m_window));
}

window::~window()
{
    ::glwtWindowDestroy(static_cast<GLWTWindow *>(m_window));
    ::glwtQuit();
}

glproc window::get_proc_address(const char *procname)
{
#    if defined(RENDERSTACK_GL_API_OPENGL)
#        if defined(_WIN32)
    glproc ptr = nullptr;
    if (m_opengl32_dll)
    {
        HMODULE opengl32_dll = static_cast<HMODULE>(m_opengl32_dll);
        ptr                  = reinterpret_cast<glproc>(GetProcAddress(opengl32_dll, procname));
    }
    if (ptr == nullptr && m_wgl_get_proc_address)
    {
        PFNWGLGETPROCADDRESS wgl_get_proc_address = reinterpret_cast<PFNWGLGETPROCADDRESS>(m_wgl_get_proc_address);
        ptr                                       = reinterpret_cast<glproc>(wgl_get_proc_address(procname));
    }
    return ptr;

    // TODO try to get with ARB postfix if glproc == NULL
#        endif
#        if defined(__APPLE__)
    NSSymbol symbol;
    char *   symbolName = (char *)malloc(strlen(procname) + 2); // 1
    strcpy(symbolName + 1, procname);                           // 2
    symbolName[0] = '_';                                        // 3
    symbol        = NULL;
    if (NSIsSymbolNameDefined(symbolName)) // 4
        symbol = NSLookupAndBindSymbol(symbolName);
    free(symbolName);                                           // 5
    return (glproc)(symbol ? NSAddressOfSymbol(symbol) : NULL); // 6
#        endif
#    else
    glproc ptr = nullptr;
    ptr        = static_cast<glproc>(eglGetProcAddress(procname));

    return ptr;
#    endif
}

void window::close()
{
    m_running = false;
}

void window::run()
{
    for (;;)
    {
        if (!m_running)
            break;
        ::glwtEventHandle(0);
        if (!m_running)
            break;
        make_current();
        update();
    }

    on_exit();
}

int window::get_mouse_button(int button)
{
    return m_buttons[button];
}
void window::get_mouse_pos(double &xpos, double &ypos)
{
    xpos = m_cursor_x;
    ypos = m_cursor_y;
}

void window::set_mouse_pos(double xpos, double ypos)
{
    (void)xpos;
    (void)ypos;
}

void window::show_cursor(bool show)
{
    (void)show;
}

void window::capture_mouse(bool capture)
{
    (void)capture;
}

bool window::get_mouse_capture() const
{
    return false;
}

int window::width() const
{
    return m_width;
}

int window::height() const
{
    return m_height;
}

void window::swap_buffers()
{
    ::glwtSwapBuffers(static_cast<GLWTWindow *>(m_window));
}

} // namespace toolkit
} // namespace renderstack
#endif
