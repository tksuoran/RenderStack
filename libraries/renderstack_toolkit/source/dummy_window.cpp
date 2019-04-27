#include "renderstack_toolkit/platform.hpp"
#include "renderstack_toolkit/window.hpp"
//#include <EGL/egl.h>

void init_memory_system()
{
}

void check_memory_system()
{
}

void begin_memory_compare()
{
}

void end_memory_compare()
{
}

namespace renderstack
{
namespace toolkit
{

void window::on_resize(int width, int height)
{
    (void)width;
    (void)height;
}

void window::set_time(double value)
{
    (void)value;
}

double window::time() const
{
    return 0.0;
}

window::window(int width, int height, const std::string &title, int major, int minor)
{
    (void)width;
    (void)height;
    (void)title;
    (void)major;
    (void)minor;
    m_running = true;

    get_extensions();
}

window::~window()
{
    on_exit();
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
        update();
    }

    on_exit();
    return;
}

int window::get_mouse_button(int button)
{
    (void)button;
    return 0;
}
void window::get_mouse_pos(int &xpos, int &ypos)
{
    xpos = 0;
    ypos = 0;
}

void window::set_mouse_pos(int xpos, int ypos)
{
    (void)xpos;
    (void)ypos;
}

void window::get_scroll_offset(double &xoffset, double &yoffset)
{
    xoffset = 0.0;
    yoffset = 0.0;
}

void window::capture_mouse(bool value)
{
    (void)value;
}

bool window::get_mouse_capture() const
{
    return false;
}

int window::width() const
{
    return 640;
}

int window::height() const
{
    return 480;
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
    //eglSwapBuffers();
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

bool window::is_extension_supported(char const *const extension)
{
    (void)extension;
    return false;
}

} // namespace toolkit
} // namespace renderstack
