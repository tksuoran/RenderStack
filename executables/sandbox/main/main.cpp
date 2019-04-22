#include "renderstack_toolkit/platform.hpp"
#include "renderstack_toolkit/gl.hpp"
#include "renderstack_toolkit/strong_gl_enums.hpp"
#include "renderstack_graphics/configuration.hpp"
#include "renderstack_graphics/shader_monitor.hpp"
#include "renderstack_graphics/program.hpp"
#include "renderstack_ui/font.hpp"

#include "main/application.hpp"
#include "main/log.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <fstream>
#include <sstream>
#include <sys/stat.h>
#include <iomanip>


using namespace gl;
using namespace glm;
using namespace std;
using namespace renderstack::graphics;
using namespace renderstack::ui;
using namespace renderstack::toolkit;

int main(int argc, char *argv[])
{
    console_init();

    init_memory_system();
    begin_memory_compare();

    static_cast<void>(argc);
    static_cast<void>(argv);
    shared_ptr<renderstack::toolkit::window> g_application;
    int return_value = EXIT_FAILURE;

#  if defined(NDEBUG)
   try
#  endif
    {
        g_application = make_shared<application>();
        if (g_application->on_load())
        {
            g_application->run();
            return_value = EXIT_SUCCESS;
        }
        else
        {
            return_value = EXIT_FAILURE;
        }

    }
#   if defined(NDEBUG)
    catch(runtime_error const &e)
    {
        fprintf(stderr, "exception:\n");
        fprintf(stderr, e.what());
        fprintf(stderr, "\nPress RETURN\n");
        fgetc(stdin);
    }
    catch(...)
    {
        ;
    }
#  endif

    if (g_application)
    {
        g_application->on_exit();
    }

    g_application.reset();

    end_memory_compare();

    exit(return_value);
}
