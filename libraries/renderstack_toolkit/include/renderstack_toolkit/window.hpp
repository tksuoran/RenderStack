#ifndef window_hpp_renderstack_toolkit
#define window_hpp_renderstack_toolkit

#include "renderstack_toolkit/platform.hpp"
#include "renderstack_toolkit/gl.hpp"
#include <string>
#if defined(RENDERSTACK_USE_GLWT)
# include <GLWT/glwt.h>
#endif

namespace renderstack { namespace toolkit {

typedef void (*glproc)(void);

#if defined(RENDERSTACK_USE_GLWT)
#define RS_KEY_BACKSPACE           0x08
#define RS_KEY_TAB                 0x09
#define RS_KEY_RETURN              0x0d
#define RS_KEY_ESCAPE              0x1b
#define RS_KEY_SPACE               0x20
#define RS_KEY_PLUS                0x2b
#define RS_KEY_COMMA               0x2c
#define RS_KEY_MINUS               0x2d
#define RS_KEY_PERIOD              0x2e
#define RS_KEY_SLASH               0x2f
#define RS_KEY_0                   0x30
#define RS_KEY_1                   0x31
#define RS_KEY_2                   0x32
#define RS_KEY_3                   0x33
#define RS_KEY_4                   0x34
#define RS_KEY_5                   0x35
#define RS_KEY_6                   0x36
#define RS_KEY_7                   0x37
#define RS_KEY_8                   0x38
#define RS_KEY_9                   0x39
#define RS_KEY_A                   0x41
#define RS_KEY_B                   0x42
#define RS_KEY_C                   0x43
#define RS_KEY_D                   0x44
#define RS_KEY_E                   0x45
#define RS_KEY_F                   0x46
#define RS_KEY_G                   0x47
#define RS_KEY_H                   0x48
#define RS_KEY_I                   0x49
#define RS_KEY_J                   0x4a
#define RS_KEY_K                   0x4b
#define RS_KEY_L                   0x4c
#define RS_KEY_M                   0x4d
#define RS_KEY_N                   0x4e
#define RS_KEY_O                   0x4f
#define RS_KEY_P                   0x50
#define RS_KEY_Q                   0x51
#define RS_KEY_R                   0x52
#define RS_KEY_S                   0x53
#define RS_KEY_T                   0x54
#define RS_KEY_U                   0x55
#define RS_KEY_V                   0x56
#define RS_KEY_W                   0x57
#define RS_KEY_X                   0x58
#define RS_KEY_Y                   0x59
#define RS_KEY_Z                   0x5a
#define RS_KEY_DELETE              0x7f
#define RS_KEY_KEYPAD_0            0x80
#define RS_KEY_KEYPAD_1            0x81
#define RS_KEY_KEYPAD_2            0x82
#define RS_KEY_KEYPAD_3            0x83
#define RS_KEY_KEYPAD_4            0x84
#define RS_KEY_KEYPAD_5            0x85
#define RS_KEY_KEYPAD_6            0x86
#define RS_KEY_KEYPAD_7            0x87
#define RS_KEY_KEYPAD_8            0x88
#define RS_KEY_KEYPAD_9            0x89
#define RS_KEY_KEYPAD_SEPARATOR    0x8a
#define RS_KEY_KEYPAD_DIVIDE       0x8b
#define RS_KEY_KEYPAD_MULTIPLY     0x8c
#define RS_KEY_KEYPAD_PLUS         0x8d
#define RS_KEY_KEYPAD_MINUS        0x8e
#define RS_KEY_KEYPAD_ENTER        0x8f
#define RS_KEY_UP                  0x90
#define RS_KEY_DOWN                0x91
#define RS_KEY_LEFT                0x92
#define RS_KEY_RIGHT               0x93
#define RS_KEY_PAGE_UP             0x94
#define RS_KEY_PAGE_DOWN           0x95
#define RS_KEY_HOME                0x96
#define RS_KEY_END                 0x97
#define RS_KEY_INSERT              0x98
#define RS_KEY_F1                  0xa0
#define RS_KEY_F2                  0xa1
#define RS_KEY_F3                  0xa2
#define RS_KEY_F4                  0xa3
#define RS_KEY_F5                  0xa4
#define RS_KEY_F6                  0xa5
#define RS_KEY_F7                  0xa6
#define RS_KEY_F8                  0xa7
#define RS_KEY_F9                  0xa8
#define RS_KEY_F10                 0xa9
#define RS_KEY_F11                 0xaa
#define RS_KEY_F12                 0xab
#define RS_KEY_LEFT_SHIFT          0xb0
#define RS_KEY_RIGHT_SHIFT         0xb1
#define RS_KEY_LEFT_CTRL           0xb2
#define RS_KEY_RIGHT_CTRL          0xb3
#define RS_KEY_LALT                0xb4
#define RS_KEY_RALT                0xb5
#define RS_KEY_LSUPER              0xb6
#define RS_KEY_RSUPER              0xb7
#define RS_KEY_MENU                0xb8
#define RS_KEY_ALTGR               0xb9
#define RS_KEY_NUM_LOCK            0xba
#define RS_KEY_CAPS_LOCK           0xbb
#define RS_KEY_SCROLL_LOCK         0xbc
#endif

#if defined(RENDERSTACK_USE_GLFW)
#define RS_KEY_UNKNOWN            -1
#define RS_KEY_SPACE              32
#define RS_KEY_APOSTROPHE         39  /* ' */
#define RS_KEY_COMMA              44  /* , */
#define RS_KEY_MINUS              45  /* - */
#define RS_KEY_PERIOD             46  /* . */
#define RS_KEY_SLASH              47  /* / */
#define RS_KEY_0                  48
#define RS_KEY_1                  49
#define RS_KEY_2                  50
#define RS_KEY_3                  51
#define RS_KEY_4                  52
#define RS_KEY_5                  53
#define RS_KEY_6                  54
#define RS_KEY_7                  55
#define RS_KEY_8                  56
#define RS_KEY_9                  57
#define RS_KEY_SEMICOLON          59  /* ; */
#define RS_KEY_EQUAL              61  /* = */
#define RS_KEY_A                  65
#define RS_KEY_B                  66
#define RS_KEY_C                  67
#define RS_KEY_D                  68
#define RS_KEY_E                  69
#define RS_KEY_F                  70
#define RS_KEY_G                  71
#define RS_KEY_H                  72
#define RS_KEY_I                  73
#define RS_KEY_J                  74
#define RS_KEY_K                  75
#define RS_KEY_L                  76
#define RS_KEY_M                  77
#define RS_KEY_N                  78
#define RS_KEY_O                  79
#define RS_KEY_P                  80
#define RS_KEY_Q                  81
#define RS_KEY_R                  82
#define RS_KEY_S                  83
#define RS_KEY_T                  84
#define RS_KEY_U                  85
#define RS_KEY_V                  86
#define RS_KEY_W                  87
#define RS_KEY_X                  88
#define RS_KEY_Y                  89
#define RS_KEY_Z                  90
#define RS_KEY_LEFT_BRACKET       91  /* [ */
#define RS_KEY_BACKSLASH          92  /* \ */
#define RS_KEY_RIGHT_BRACKET      93  /* ] */
#define RS_KEY_GRAVE_ACCENT       96  /* ` */
#define RS_KEY_WORLD_1            161 /* non-US #1 */
#define RS_KEY_WORLD_2            162 /* non-US #2 */
#define RS_KEY_ESCAPE             256
#define RS_KEY_ENTER              257
#define RS_KEY_TAB                258
#define RS_KEY_BACKSPACE          259
#define RS_KEY_INSERT             260
#define RS_KEY_DELETE             261
#define RS_KEY_RIGHT              262
#define RS_KEY_LEFT               263
#define RS_KEY_DOWN               264
#define RS_KEY_UP                 265
#define RS_KEY_PAGE_UP            266
#define RS_KEY_PAGE_DOWN          267
#define RS_KEY_HOME               268
#define RS_KEY_END                269
#define RS_KEY_CAPS_LOCK          280
#define RS_KEY_SCROLL_LOCK        281
#define RS_KEY_NUM_LOCK           282
#define RS_KEY_PRINT_SCREEN       283
#define RS_KEY_PAUSE              284
#define RS_KEY_F1                 290
#define RS_KEY_F2                 291
#define RS_KEY_F3                 292
#define RS_KEY_F4                 293
#define RS_KEY_F5                 294
#define RS_KEY_F6                 295
#define RS_KEY_F7                 296
#define RS_KEY_F8                 297
#define RS_KEY_F9                 298
#define RS_KEY_F10                299
#define RS_KEY_F11                300
#define RS_KEY_F12                301
#define RS_KEY_F13                302
#define RS_KEY_F14                303
#define RS_KEY_F15                304
#define RS_KEY_F16                305
#define RS_KEY_F17                306
#define RS_KEY_F18                307
#define RS_KEY_F19                308
#define RS_KEY_F20                309
#define RS_KEY_F21                310
#define RS_KEY_F22                311
#define RS_KEY_F23                312
#define RS_KEY_F24                313
#define RS_KEY_F25                314
#define RS_KEY_KP_0               320
#define RS_KEY_KP_1               321
#define RS_KEY_KP_2               322
#define RS_KEY_KP_3               323
#define RS_KEY_KP_4               324
#define RS_KEY_KP_5               325
#define RS_KEY_KP_6               326
#define RS_KEY_KP_7               327
#define RS_KEY_KP_8               328
#define RS_KEY_KP_9               329
#define RS_KEY_KP_DECIMAL         330
#define RS_KEY_KP_DIVIDE          331
#define RS_KEY_KP_MULTIPLY        332
#define RS_KEY_KP_SUBTRACT        333
#define RS_KEY_KP_ADD             334
#define RS_KEY_KP_ENTER           335
#define RS_KEY_KP_EQUAL           336
#define RS_KEY_LEFT_SHIFT         340
#define RS_KEY_LEFT_CONTROL       341
#define RS_KEY_LEFT_ALT           342
#define RS_KEY_LEFT_SUPER         343
#define RS_KEY_RIGHT_SHIFT        344
#define RS_KEY_RIGHT_CONTROL      345
#define RS_KEY_RIGHT_ALT          346
#define RS_KEY_RIGHT_SUPER        347
#define RS_KEY_MENU               348
#define RS_KEY_LAST               RS_KEY_MENU
#endif

class window
{
private:
   void* m_window; /* ::GLWTWindow *  */

public:
   window();
   virtual ~window();

   bool     open        (int width, int height, std::string const &title, int major, int minor);

   int      width       () const;
   int      height      () const;
   void     swap_buffers();
   void     run         ();
   void     set_time    (double value);
   double   time        () const;
   void     close       ();
   void     make_current();

   int      get_mouse_button  (int button);
   void     get_mouse_pos     (double &xpos, double &ypos);
   void     set_mouse_pos     (double xpos, double ypos);
   void     capture_mouse     (bool capture);
   bool     get_mouse_capture () const;
   void     show_cursor       (bool show);

#if defined(WIN32)
   HGLRC    get_native_context();
#endif

#if defined(RENDERSTACK_USE_FREEGLUT)
   void window_resize_event(int width, int height);
   void mouse_button_event(int button, int state);
   void mouse_position_event(int x, int y);
#endif

private:
   bool string_list_check(const char *check_str, const char *str_list);
   void get_extensions();
   glproc get_proc_address(const char* procname);


#if 0
private:
   void resize(int width, int height);

private:
   int m_width;
   int m_height;
#endif
private:
   bool m_running;
   bool m_capture;
   bool m_show;

#if defined(_WIN32)
# if defined(RENDERSTACK_USE_AMD_GPU_PERF_API)
   void *m_gpuperfapigl_dll;
# endif
#endif

#if defined(RENDERSTACK_USE_GLWT)
   bool     m_buttons[10];
   double   m_cursor_x;
   double   m_cursor_y;
   int      m_width;
   int      m_height;

   ::GLWTConfig            m_glwt_config;

# if defined(_WIN32)
#  if defined(RENDERSTACK_GL_API_OPENGL)
   void                    *m_opengl32_dll;
   void                    *m_wgl_get_proc_address;
#  endif

   void                    *m_winmm_dll;
   void                    *m_time_get_time;
# endif

public:
   void glwt_callback(const GLWTWindowEvent *event);
#endif

public:
   //virtual bool on_close_request();
   virtual bool on_load() = 0;
   virtual bool on_exit() = 0;
   virtual void on_resize(int width, int height) = 0;
   virtual void on_focus(bool has_focus) = 0;
   virtual void update() = 0;
   virtual void on_key(int key, int scancode, int action, int mods) = 0;
   virtual void on_mouse_moved(double x, double y) = 0;
   virtual void on_mouse_button(int button, int action, int mods) = 0;
   virtual void on_scroll(double x, double y) = 0;
   virtual void on_3d_mouse(long tx, long ty, long tz, long rx, long ry, long rz, long period) = 0;
};

} }

#endif
