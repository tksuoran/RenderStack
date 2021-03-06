#if defined(RENDERSTACK_USE_GLFW)
#    include "renderstack_toolkit/platform.hpp"
#    include "renderstack_toolkit/window.hpp"
#    include <GLFW/glfw3.h>
#    include <cstdlib>
#    include <stdexcept>

#    if defined(WIN32)
#        define GLFW_EXPOSE_NATIVE_WIN32 1
#        define GLFW_EXPOSE_NATIVE_WGL 1
#        include <GLFW/glfw3native.h>
#    else
// TODO
#    endif

using namespace std;
#    if 0
#        if defined(_WIN32)

#            pragma warning(disable : 4074) //initializers put in compiler reserved initialization Area
#            pragma init_seg(compiler)      //global objects in this file get constructed very early on

struct CrtBreakAllocSetter {
    CrtBreakAllocSetter() {
        _crtBreakAlloc = 1519;
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
        throw runtime_error("memory corruption");
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

static void s_key(GLFWwindow *win, int key, int scancode, int action, int mods)
{
    class window *window = reinterpret_cast<class window *>(::glfwGetWindowUserPointer(win));
    if (window)
    {
        window->on_key(key, scancode, action, mods);
    }
}

static void s_mouse_pos(GLFWwindow *win, double x, double y)
{
    class window *window = reinterpret_cast<class window *>(::glfwGetWindowUserPointer(win));
    if (window)
        window->on_mouse_moved(x, y);
}

static void s_mouse_button(GLFWwindow *win, int button, int action, int mods)
{
    class window *window = reinterpret_cast<class window *>(::glfwGetWindowUserPointer(win));
    if (window)
        window->on_mouse_button(button, action, mods);
}

static void s_mouse_wheel(GLFWwindow *win, double x, double y)
{
    class window *window = reinterpret_cast<class window *>(::glfwGetWindowUserPointer(win));
    if (window)
        window->on_scroll(x, y);
}

#    if RENDERSTACK_3DMOUSE_SUPPORT
static void s_3d_mouse(GLFWwindow *win, long tx, long ty, long tz, long rx, long ry, long rz, long period)
{
    class window *window = reinterpret_cast<class window *>(::glfwGetWindowUserPointer(win));
    if (window)
        window->on_3d_mouse(tx, ty, tz, rx, ry, rz, period);
}
#    endif

static void s_resize(GLFWwindow *win, int width, int height)
{
    class window *window = reinterpret_cast<class window *>(::glfwGetWindowUserPointer(win));

    if (window)
        window->on_resize(width, height);
}

static void s_window_close(GLFWwindow *win)
{
    class window *window = reinterpret_cast<class window *>(::glfwGetWindowUserPointer(win));
    if (window != nullptr)
        window->close();
}

void window::set_time(double value)
{
    ::glfwSetTime(value);
}

double window::time() const
{
    return ::glfwGetTime();
}

#    if defined(WIN32)
HGLRC window::get_native_context()
{
    return glfwGetWGLContext((GLFWwindow *)m_window);
}
#    endif

bool window::open(int width, int height, const std::string &title, int major, int minor)
{
    if (!::glfwInit())
    {
        ::fprintf(stderr, "Failed to initialize GLFW\n");
        return false;
    }

#    if defined(RENDERSTACK_GL_API_OPENGL)
    ::glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_API);
    ::glfwWindowHint(GLFW_RED_BITS, 8);
    ::glfwWindowHint(GLFW_GREEN_BITS, 8);
    ::glfwWindowHint(GLFW_BLUE_BITS, 8);
    ::glfwWindowHint(GLFW_DEPTH_BITS, 24);
    //::glfwWindowHint(GLFW_SAMPLES,      4);
    ::glfwWindowHint(GLFW_SRGB_CAPABLE, GL_TRUE);

    /*
   if (major >= 3)
      ::glfwWindowHint(GLFW_SAMPLES,   4);*/

    ::glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, major);
    ::glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, minor);

    if (
        (major > 3) ||
        ((major == 3) &&
         (minor >= 1)))
    {
#        if !defined(NDEBUG)
        ::glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#            if !defined(__APPLE__)
        ::glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
#            endif
#        endif
    }

    if (
        (major > 3) ||
        ((major == 3) &&
         (minor >= 2)))
    {
        //::glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
        ::glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        //::glfwWindowHint(GLFW_OPENGL_PROFILE,        GLFW_OPENGL_COMPAT_PROFILE);
    }
#    endif

#    if defined(RENDERSTACK_GL_API_OPENGL_ES_2) || defined(RENDERSTACK_GL_API_OPENGL_ES_3)
    ::glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
    ::glfwWindowHint(GLFW_RED_BITS, 0);
    ::glfwWindowHint(GLFW_GREEN_BITS, 0);
    ::glfwWindowHint(GLFW_BLUE_BITS, 0);
    ::glfwWindowHint(GLFW_DEPTH_BITS, 0);
    ::glfwWindowHint(GLFW_STENCIL_BITS, 0);
    ::glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, major);
    ::glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, minor);
#    endif

    m_window = ::glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
    if (!m_window)
    {
        ::fprintf(stderr, "Failed to open GLFW window for GL %d, %d\n", major, minor);
        ::glfwTerminate(); // TODO Move elsewhere!
        return false;
    }

    m_running = true;
    m_capture = false;
    m_show    = true;

    //int forward =::glfwGetWindowParam((GLFWwindow*)m_window, GLFW_OPENGL_FORWARD_COMPAT);

    ::glfwSetWindowUserPointer((GLFWwindow *)m_window, this);
    ::glfwSetWindowSizeCallback((GLFWwindow *)m_window, s_resize);
    ::glfwSetKeyCallback((GLFWwindow *)m_window, s_key);
    ::glfwSetCursorPosCallback((GLFWwindow *)m_window, s_mouse_pos);
    ::glfwSetMouseButtonCallback((GLFWwindow *)m_window, s_mouse_button);
    ::glfwSetScrollCallback((GLFWwindow *)m_window, s_mouse_wheel);
    ::glfwSetWindowCloseCallback((GLFWwindow *)m_window, s_window_close);
#    if RENDERSTACK_3DMOUSE_SUPPORT
    ::glfwSet3DMouseCallback((GLFWwindow *)m_window, s_3d_mouse);
#    endif

    ::glfwSetInputMode((GLFWwindow *)m_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    ::glfwSetInputMode((GLFWwindow *)m_window, GLFW_STICKY_KEYS, GL_FALSE);
    ::glfwSetInputMode((GLFWwindow *)m_window, GLFW_STICKY_MOUSE_BUTTONS, GL_FALSE);
    ::glfwSwapInterval(0);

    ::glfwShowWindow((GLFWwindow *)m_window);
    ::glfwMakeContextCurrent((GLFWwindow *)m_window);
    get_extensions();

    ::glfwSwapInterval(0);

    return true;
}

window::window()
{
}

window::~window()
{
    ::glfwDestroyWindow((GLFWwindow *)m_window);
    ::glfwTerminate();
}

#    if defined(RENDERSTACK_GL_API_OPENGL_ES_3)
void window::get_extensions()
{
}
#    endif

glproc window::get_proc_address(const char *procname)
{
    glproc proc = ::glfwGetProcAddress(procname);
    // TODO try to get with ARB postfix if glproc == NULL
    return proc;
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
        ::glfwPollEvents();
        if (!m_running)
            break;
        update();
    }

    on_exit();
}

int window::get_mouse_button(int button)
{
    return ::glfwGetMouseButton((GLFWwindow *)m_window, button);
}
void window::get_mouse_pos(double &xpos, double &ypos)
{
    ::glfwGetCursorPos((GLFWwindow *)m_window, &xpos, &ypos);
}

void window::set_mouse_pos(double xpos, double ypos)
{
    ::glfwSetCursorPos((GLFWwindow *)m_window, xpos, ypos);
}

void window::show_cursor(bool show)
{
    if (m_show != show)
    {
        m_show = show;
        if (!m_capture)
            ::glfwSetInputMode(
                (GLFWwindow *)m_window,
                GLFW_CURSOR,
                m_capture
                    ? GLFW_CURSOR_DISABLED
                    : m_show
                          ? GLFW_CURSOR_NORMAL
                          : GLFW_CURSOR_HIDDEN);
    }
}

void window::capture_mouse(bool capture)
{
    if (m_capture != capture)
    {
        m_capture = capture;
        ::glfwSetInputMode(
            (GLFWwindow *)m_window,
            GLFW_CURSOR,
            m_capture
                ? GLFW_CURSOR_DISABLED
                : m_show
                      ? GLFW_CURSOR_NORMAL
                      : GLFW_CURSOR_HIDDEN);
    }
}

bool window::get_mouse_capture() const
{
    int mode = ::glfwGetInputMode((GLFWwindow *)m_window, GLFW_CURSOR);
    if (mode == GLFW_CURSOR_DISABLED)
        return true;
    if (mode == GLFW_CURSOR_NORMAL)
        return false;

    throw runtime_error("unexpected GLFW_CURSOR_MODE");
}

int window::width() const
{
#    if 1
    int width;
    int height;
    ::glfwGetWindowSize((GLFWwindow *)m_window, &width, &height);
    return width;
#    else
    return m_width;
#    endif
}

int window::height() const
{
#    if 1
    int width;
    int height;
    ::glfwGetWindowSize((GLFWwindow *)m_window, &width, &height);
    return height;
#    else
    return m_height;
#    endif
}

void window::swap_buffers()
{
    ::glfwSwapBuffers((GLFWwindow *)m_window);
}

} // namespace toolkit
} // namespace renderstack
#endif
