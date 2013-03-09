#ifndef screen_hpp
#define screen_hpp

class screen
{
public:
   virtual ~screen()
   {
   }
   virtual void update           () = 0;
   virtual void on_enter         (){}
   virtual void on_exit          (){}
   virtual void on_resize        (int width, int height){(void)width;(void)height;}
   virtual void on_key_down      (int key){(void)key;}
   virtual void on_key_up        (int key){(void)key;}
   virtual void on_mouse_moved   (int x, int y){(void)x;(void)y;}
   virtual void on_mouse_button  (int button, int value){(void)button;(void)value;}
   virtual void on_scroll        (double x, double y){(void)x;(void)y;}
};

#endif
