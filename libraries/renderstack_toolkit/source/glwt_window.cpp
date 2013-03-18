#if defined(RENDERSTACK_USE_GLWT)
#include "renderstack_toolkit/platform.hpp"
#include "renderstack_toolkit/window.hpp"
#include <GLWT/glwt.h>
//#include <GLXW/glxw.h>
#include <stdexcept>

#if defined(__APPLE__)
# include <mach-o/dyld.h>
# include <cstdlib>
# include <cstring>
#endif

#include <cassert>

using namespace std;
#if 0
# if defined(_WIN32)

#  pragma warning(disable:4074)//initializers put in compiler reserved initialization area
#  pragma init_seg(compiler)//global objects in this file get constructed very early on

struct CrtBreakAllocSetter {
    CrtBreakAllocSetter() {
        _crtBreakAlloc = 185;
    }
};

CrtBreakAllocSetter g_crtBreakAllocSetter;
# endif
#endif

void init_memory_system()
{
#if defined(_WIN32)
   _CrtSetDbgFlag(
      _CRTDBG_ALLOC_MEM_DF       |
      _CRTDBG_DELAY_FREE_MEM_DF  |
      _CRTDBG_LEAK_CHECK_DF
   );
   /*
   _CRTDBG_ALLOC_MEM_DF       - Turn on debug allocation
   _CRTDBG_DELAY_FREE_MEM_DF  - Don't actually free memory
   _CRTDBG_CHECK_ALWAYS_DF    - Check heap every alloc/dealloc
   _CRTDBG_RESERVED_DF        - Reserved - do not use
   _CRTDBG_CHECK_CRT_DF       - Leak check/diff CRT blocks DO NOT USE IF YOU DON'T KNOW WHAT YOU ARE DOING
   _CRTDBG_LEAK_CHECK_DF      - Leak check at program exit 
   */
#endif
}

void check_memory_system()
{
#if defined(_WIN32)
   if (_CrtCheckMemory() == FALSE)
      throw std::runtime_error("memory corruption");
#endif
}

#if defined(_WIN32)
static _CrtMemState initial_memory_state;
#endif

void begin_memory_compare()
{
#if defined(_WIN32)
   _CrtMemCheckpoint(&initial_memory_state);
#endif
}

void end_memory_compare()
{
#if defined(_WIN32)
   _CrtMemState end_memory_state;
   _CrtMemState diff_memory_state;

   _CrtMemCheckpoint(&end_memory_state);

   (void)/*int res = */_CrtMemDifference(&diff_memory_state, &initial_memory_state, &end_memory_state);
   _CrtMemDumpStatistics(&diff_memory_state);

   _CrtMemDumpAllObjectsSince(&initial_memory_state);
#endif
}

namespace renderstack { namespace toolkit {

static void s_error_callback(const char *msg, void *userdata)
{
   (void)userdata;
   printf("%s\n", msg);
}

static void s_win_callback(GLWTWindow *window, const GLWTWindowEvent *event, void *userdata)
{
   (void)window;
   (void)event;
   (void)userdata;
}

void window::glwt_key_callback(int down, int keysym, int scancode, int mod)
{
   (void)scancode;
   (void)mod;
   if (down == 1)
      on_key_down(keysym);
   else
      on_key_up(keysym);
}
static void s_key_callback(GLWTWindow *window, int down, int keysym, int scancode, int mod, void *userdata)
{
   (void)window;
   class window *rs_window = reinterpret_cast<class window *>(userdata);
   if (rs_window)
      rs_window->glwt_key_callback(down, keysym, scancode, mod);
}

void window::glwt_motion_callback(int x, int y, int buttons)
{
   m_cursor_x = x;
   m_cursor_y = y;
   for (int i = 0; i < sizeof(m_buttons); ++i)
      m_buttons[i] = !!(buttons & (1 << i));
   on_mouse_moved(x, y);
}

static void s_motion_callback(GLWTWindow *window, int x, int y, int buttons, void *userdata)
{
   (void)window;
   class window *rs_window = reinterpret_cast<class window *>(userdata);
   if (rs_window)
      rs_window->glwt_motion_callback(x, y, buttons);
}

void window::glwt_button_callback(int down, int x, int y, int button, int mod)
{
   (void)mod;
   m_cursor_x = x;
   m_cursor_y = y;
   on_mouse_button(button, down);
}

static void s_button_callback(GLWTWindow *window, int down, int x, int y, int button, int mod, void *userdata)
{
   (void)window;
   class window *rs_window = reinterpret_cast<class window *>(userdata);
   if (rs_window)
      rs_window->glwt_button_callback(down, x, y, button, mod);
}

void window::glwt_resize_callback(int width, int height)
{
   m_width = width;
   m_height = height;
   on_resize(width, height);
}

static void s_resize_callback(GLWTWindow *window, int width, int height, void *userdata)
{
   (void)window;
   class window *rs_window = reinterpret_cast<class window *>(userdata);

   if (rs_window)
      rs_window->glwt_resize_callback(width, height);
}

static void s_close_callback(GLWTWindow *window, void *userdata)
{
   (void)window;
   class window *rs_window = reinterpret_cast<class window *>(userdata);
   if (rs_window)
      rs_window->close();
}

void window::on_resize(int width, int height)
{
   (void)width;
   (void)height;
}

void window::set_time(double value)
{
   (void)(value); // TODO
}

double window::time() const
{
   double res = m_fake_time;
   m_fake_time += (1.0 / 50.0);
   return res; // TODO
}

# if defined(_WIN32)
typedef WINGDIAPI PROC (WINAPI * PFNWGLGETPROCADDRESS) (LPCSTR);
#endif

window::window(int width, int height, std::string const &title, int major, int minor)
{
# if defined(_WIN32)
   HMODULE libgl = LoadLibraryA("opengl32.dll");
   PFNWGLGETPROCADDRESS wgl_get_proc_address = reinterpret_cast<PFNWGLGETPROCADDRESS>(GetProcAddress(libgl, "wglGetProcAddress"));
   m_libgl = reinterpret_cast<void*>(libgl);
   m_wgl_get_proc_address = reinterpret_cast<void*>(wgl_get_proc_address);
#endif

   // WINGDIAPI PROC  WINAPI wglGetProcAddress(LPCSTR);

   //typedef WINGDIAPI PROC (WINAPI * PFNWGLGETPROCADDRESS) (LPCSTR);
   //typedef void *(WINAPI *PFNwglGetProcAddress) (const char *);

#if 0
   ::memset(&m_app_callbacks, sizeof(::GLWTAppCallbacks), 0);
   m_app_callbacks.error_callback = s_error_callback;
   m_app_callbacks.userdata = nullptr;
#endif

   ::memset(&m_glwt_config, sizeof(::GLWTConfig), 0);
   m_glwt_config.red_bits            = 8;
   m_glwt_config.green_bits          = 8;
   m_glwt_config.blue_bits           = 8;
   m_glwt_config.alpha_bits          = 0;
   m_glwt_config.depth_bits          = 24;
   m_glwt_config.stencil_bits        = 0;
   m_glwt_config.api                 = GLWT_API_ANY;
   m_glwt_config.api_version_major   = major;
   m_glwt_config.api_version_minor   = minor;

   if (major >= 3) {
      m_glwt_config.sample_buffers   = 1;
      m_glwt_config.samples          = 4;
   } else {
      m_glwt_config.sample_buffers   = 0;
      m_glwt_config.samples          = 0;
   }

#if defined(_DEBUG)
   if (major >= 3) {
      // TODO request forward compatible
      m_glwt_config.api |= GLWT_PROFILE_DEBUG;
   }
#endif
   if (major >= 3)
      m_glwt_config.api |= GLWT_PROFILE_CORE;

   if (::glwtInit(&m_glwt_config, &s_error_callback, NULL) != 0)
      throw runtime_error("Failed to initialize GLWT");

   m_width = width;
   m_height = height;

   GLWTWindow *win = ::glwtWindowCreate(
      title.c_str(),
      width,
      height,
      NULL, 
      s_win_callback, 
      reinterpret_cast<void*>(this)
   );
   if (!win)
   {
      ::glwtQuit();
      throw runtime_error("Failed to open GLWt window");
   }

   m_window = win;
   m_running = true;
   m_capture = false;
   m_show = true;

   ::glwtWindowShow(win, 1);
   ::glwtMakeCurrent(win);

   get_extensions();
}

void window::make_current()
{
   assert(m_window);
   ::glwtMakeCurrent(reinterpret_cast<GLWTWindow *>(m_window));
}

window::~window()
{
   on_exit();
   ::glwtWindowDestroy(reinterpret_cast<GLWTWindow*>(m_window));
   ::glwtQuit();
}

glproc window::get_proc_address(const char* procname)
{
#if defined(_WIN32)
   HMODULE libgl = reinterpret_cast<HMODULE>(m_libgl);
   glproc ptr = reinterpret_cast<glproc>(GetProcAddress(libgl, procname));
   if (ptr == nullptr) {
      PFNWGLGETPROCADDRESS wgl_get_proc_address = reinterpret_cast<PFNWGLGETPROCADDRESS>(m_wgl_get_proc_address);
      ptr = reinterpret_cast<glproc>(wgl_get_proc_address(procname));
   }
   return ptr;

   // TODO try to get with ARB postfix if glproc == NULL
#endif
#if defined(__APPLE__)
   NSSymbol symbol;
   char *symbolName = (char*)malloc (strlen (procname) + 2); // 1
   strcpy(symbolName + 1, procname); // 2
   symbolName[0] = '_'; // 3
   symbol = NULL;
   if (NSIsSymbolNameDefined (symbolName)) // 4
      symbol = NSLookupAndBindSymbol (symbolName);
   free (symbolName); // 5
   return (glproc)(symbol ? NSAddressOfSymbol (symbol) : NULL); // 6
#endif
   return nullptr;
}

void window::close()
{
   m_running = false;
}

void window::run()
{
   for (;;) {
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
void window::get_mouse_pos(int &xpos, int &ypos)
{
   xpos = m_cursor_x;
   ypos = m_cursor_y;
}

void window::set_mouse_pos(int xpos, int ypos)
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

void window::on_key_down(int key)
{
   (void)key;
}

void window::on_key_up(int key)
{
   (void)key;
}

void window::on_mouse_moved(int x, int y)
{
   (void)x;
   (void)y;
}

void window::on_mouse_button(int button, int value)
{
   (void)button;
   (void)value;
}

void window::on_scroll(double x, double y)
{
   (void)x;
   (void)y;
}

void window::swap_buffers()
{
   ::glwtSwapBuffers(reinterpret_cast<GLWTWindow*>(m_window));
}

bool window::on_load()
{
   update();
   return true;
}

bool window::on_exit()
{
   return true;
}

void window::update()
{
}

} }
#endif