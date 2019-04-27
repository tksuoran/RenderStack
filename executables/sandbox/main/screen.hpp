#ifndef screen_hpp
#define screen_hpp

class Screen
{
public:
    virtual ~Screen() = default;

    virtual void update() = 0;

    virtual void on_enter() = 0;

    virtual void on_exit() = 0;

    virtual void on_resize(int width, int height) = 0;

    virtual void on_focus(bool has_focus) = 0;

    virtual void on_key(int key, int scancode, int action, int mods) = 0;

    virtual void on_mouse_moved(double x, double y) = 0;

    virtual void on_mouse_button(int button, int action, int mods) = 0;

    virtual void on_scroll(double x, double y) = 0;

    virtual void on_3d_mouse(long tx, long ty, long tz, long rx, long ry, long rz, long period) = 0;
};

#endif
