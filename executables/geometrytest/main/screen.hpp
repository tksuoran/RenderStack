#ifndef screen_hpp
#define screen_hpp

class screen
{
public:
   virtual ~screen()
   {
   }
   virtual void update           () = 0;
   virtual void on_enter         () = 0;
   virtual void on_exit          () = 0;
   virtual void on_resize        (int width, int height) = 0;
   virtual void on_key           (int key, int action, int mods) = 0;
   virtual void on_mouse_moved   (double x, double y) = 0;
   virtual void on_mouse_button  (int button, int action, int mods) =0;
   virtual void on_scroll        (double x, double y) = 0;
};

#endif
