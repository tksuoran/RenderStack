#if defined(RENDERSTACK_USE_FREEGLUT)
#    include "renderstack_toolkit/platform.hpp"
#    include "renderstack_toolkit/window.hpp"
#    include <GL/freeglut.h>
#    include <cstdlib>
#    include <stdexcept>

using namespace std;
#    if 0
#        if defined(_WIN32)

#            pragma warning(disable : 4074) //initializers put in compiler reserved initialization area
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

void window::set_time(double value)
{
    (void)value;
}

double window::time() const
{
    return 0.0;
}

#    if defined(WIN32)
HGLRC window::get_native_context()
{
    return NULL;
}
#    endif

// Context
static void init_context()
{
    // TODO
}
static void app_status_func(int status)
{
    switch (status)
    {
        case GLUT_APPSTATUS_PAUSE:
            break;
        case GLUT_APPSTATUS_RESUME:
            break;
        default:
            break;
    }
}

// Errors and warnings
static void error_func(const char *fmt, va_list ap)
{
    (void)fmt;
    (void)ap;
}
static void warning_func(const char *fmt, va_list ap)
{
    (void)fmt;
    (void)ap;
}

// Keyboard
#    define KEY_DOWN 1
#    define KEY_UP 0
static void keyboard_func(unsigned char key, int x, int y)
{
    class window *window = reinterpret_cast<class window *>(glutGetWindowData());
    if (window)
    {
        window->mouse_position_event(x, y);
        int mods = glutGetModifiers(); // TODO translate
        window->on_key(key, 0, KEY_DOWN, mods);
        window->on_mouse_moved(x, y);
    }
}
static void keyboard_up_func(unsigned char key, int x, int y)
{
    class window *window = reinterpret_cast<class window *>(glutGetWindowData());
    if (window)
    {
        window->mouse_position_event(x, y);
        int mods = glutGetModifiers(); // TODO translate
        window->on_key(key, 0, KEY_UP, mods);
        window->on_mouse_moved(x, y);
    }
}
static void special_func(int key, int x, int y)
{
    class window *window = reinterpret_cast<class window *>(glutGetWindowData());
    if (window)
    {
        window->mouse_position_event(x, y);
        int mods = glutGetModifiers(); // TODO translate
        window->on_key(key, 0, KEY_DOWN, mods);
        window->on_mouse_moved(x, y);
    }
}
static void special_up_func(int key, int x, int y)
{
    class window *window = reinterpret_cast<class window *>(glutGetWindowData());
    if (window)
    {
        window->mouse_position_event(x, y);
        int mods = glutGetModifiers(); // TODO translate
        window->on_key(key, 0, KEY_UP, mods);
        window->on_mouse_moved(x, y);
    }
}

// Window
static void reshape_func(int width, int height)
{
    class window *window = reinterpret_cast<class window *>(glutGetWindowData());
    if (window)
    {
        window->window_resize_event(width, height);
        window->on_resize(width, height);
    }
}
static void visibility_func(int visibility)
{
    (void)visibility;
}
static void display_func()
{
    class window *window = reinterpret_cast<class window *>(glutGetWindowData());
    if (window)
        window->update();
}
static void entry_func(int entry)
{
    (void)entry;
}
static void window_status_func(int window_status)
{
    (void)window_status;
}
static void close_func()
{
    class window *window = reinterpret_cast<class window *>(glutGetWindowData());
    if (window != nullptr)
        window->close();
}
static void wm_close_func()
{
    class window *window = reinterpret_cast<class window *>(glutGetWindowData());
    if (window != nullptr)
        window->close();
}

// Mouse
static void mouse_func(int button, int state, int x, int y)
{
    class window *window = reinterpret_cast<class window *>(glutGetWindowData());
    if (window != nullptr)
    {
        window->mouse_position_event(x, y);
        window->on_mouse_button(button, state, 0);
        window->on_mouse_moved(x, y);
    }
}
static void motion_func(int x, int y)
{
    class window *window = reinterpret_cast<class window *>(glutGetWindowData());
    if (window != nullptr)
    {
        window->mouse_position_event(x, y);
        window->on_mouse_moved(x, y);
    }
}
static void passive_motion_func(int x, int y)
{
    class window *window = reinterpret_cast<class window *>(glutGetWindowData());
    if (window != nullptr)
    {
        window->mouse_position_event(x, y);
        window->on_mouse_moved(x, y);
    }
}
static void mouse_wheel_func(int wheel, int direction, int x, int y)
{
    (void)wheel;
    (void)direction;
    class window *window = reinterpret_cast<class window *>(glutGetWindowData());
    if (window != nullptr)
    {
        // TODO
        window->on_scroll(x, y);
    }
}
static void tablet_motion_func(int x, int y)
{
    class window *window = reinterpret_cast<class window *>(glutGetWindowData());
    if (window != nullptr)
    {
        window->mouse_position_event(x, y);
        window->on_mouse_moved(x, y);
    }
}
static void tablet_button_func(int button, int state, int x, int y)
{
    class window *window = reinterpret_cast<class window *>(glutGetWindowData());
    if (window != nullptr)
    {
        window->mouse_button_event(button, state);
        window->mouse_position_event(x, y);
        window->on_mouse_button(button, state, 0);
        window->on_mouse_moved(x, y);
    }
}
static void idle_func()
{
    class window *window = reinterpret_cast<class window *>(glutGetWindowData());
    if (window != nullptr)
        window->update();
}

void window::window_resize_event(int width, int height)
{
    m_width  = width;
    m_height = height;
}

void window::mouse_position_event(int x, int y)
{
    m_x = x;
    m_y = y;
}
void window::mouse_button_event(int button, int state)
{
    m_buttons[button] = state;
}

bool window::open(int width, int height, string const &title, int major, int minor)
{
    int   argc = 0;
    char *argv = NULL;
    glutInitContextVersion(major, minor);
    //glutInitContextFlags();
    glutInitContextProfile(GLUT_CORE_PROFILE);
    glutInitWarningFunc(warning_func);
    glutInitErrorFunc(error_func);
    glutInitWindowSize(width, height);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
    glutInit(&argc, &argv);

    // Keyboard
    glutKeyboardFunc(keyboard_func);
    glutKeyboardUpFunc(keyboard_up_func);
    glutSpecialFunc(special_func);
    glutSpecialUpFunc(special_up_func);

    // Window
    glutReshapeFunc(reshape_func);
    glutDisplayFunc(display_func);
    glutVisibilityFunc(visibility_func);
    glutEntryFunc(entry_func);
    glutCloseFunc(close_func);
    glutWMCloseFunc(wm_close_func);

    // Mouse
    glutMouseFunc(mouse_func);
    glutMotionFunc(motion_func);
    glutMouseWheelFunc(mouse_wheel_func);
    glutPassiveMotionFunc(passive_motion_func);
    glutWindowStatusFunc(window_status_func);
    glutTabletMotionFunc(tablet_motion_func);
    glutTabletButtonFunc(tablet_button_func);

    // Joystick
    //glutJoystickFunc(joystick_func, 10);

    glutInitWindowSize(width, height);
    m_window = glutCreateWindow(title.c_str());
    if (!m_window)
    {
        fprintf(stderr, "Failed to open GLFW window for GL %d, %d\n", major, minor);
        return false;
    }
    glutSetWindowData(this);

    m_running = true;
    m_capture = false;
    m_show    = true;

    get_extensions();

    return true;
}

window::window()
{
}

window::~window()
{
    glutDestroyWindow(m_window);
    glutExit();
}

#    if defined(RENDERSTACK_GL_API_OPENGL_ES_3)
void window::get_extensions()
{
}
#    endif

glproc window::get_proc_address(const char *procname)
{
    glproc proc = glutGetProcAddress(procname);
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
        glutMainLoopEvent();
        if (!m_running)
            break;
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
    xpos = m_x;
    ypos = m_y;
}

void window::set_mouse_pos(double xpos, double ypos)
{
    glutWarpPointer((int)xpos, (int)ypos);
}

void window::show_cursor(bool show)
{
    glutSetCursor(show ? GLUT_CURSOR_LEFT_ARROW : GLUT_CURSOR_NONE);
}

void window::capture_mouse(bool capture)
{
    m_capture = capture;
}

bool window::get_mouse_capture() const
{
    return m_capture;
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
    glutSwapBuffers();
}

} // namespace toolkit
} // namespace renderstack
#endif