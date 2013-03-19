#ifndef renderstack_toolkit__window_hpp
#define renderstack_toolkit__window_hpp

#include "platform.hpp"
#include "renderstack_toolkit/gl.hpp"
#include <string>
#if defined(RENDERSTACK_USE_GLWT)
# include <GLWT/glwt.h>
#endif

namespace renderstack { namespace toolkit {

typedef void (*glproc)(void);

class window
{
private:
   void* m_window; /* ::GLWTWindow *  */

public:
   window(int width, int height, std::string const &title, int major, int minor);
   virtual ~window();

   int      width       () const;
   int      height      () const;
   void     swap_buffers();
   void     run         ();
   void     set_time    (double value);
   double   time        () const;
   void     close       ();
   void     make_current();

   int      get_mouse_button  (int button);
   void     get_mouse_pos     (int &xpos, int &ypos);
   void     set_mouse_pos     (int xpos, int ypos);
   void     capture_mouse     (bool capture);
   bool     get_mouse_capture () const;
   void     show_cursor       (bool show);

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

#if defined(RENDERSTACK_USE_GLWT)
   bool  m_buttons[10];
   int   m_cursor_x;
   int   m_cursor_y;
   int   m_width;
   int   m_height;

   ::GLWTConfig            m_glwt_config;

# if defined(_WIN32)
   void                    *m_libgl;
   void                    *m_wgl_get_proc_address;
# endif

   mutable
   double m_fake_time;

public:
   void glwt_callback(const GLWTWindowEvent *event);
#endif

public:
   //virtual bool on_close_request();
   virtual bool on_load();
   virtual bool on_exit();
   virtual void on_resize(int width, int height);
   virtual void update();
   virtual void on_key_down(int key);
   virtual void on_key_up(int key);
   virtual void on_mouse_moved(int x, int y);
   virtual void on_mouse_button(int button, int value);
   virtual void on_scroll(double x, double y);
};

} }

#endif
