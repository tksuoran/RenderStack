#if 0

#include <algorithm>
#include <iterator>
#include <string>
#include <memory>
#include <cassert>
#include <iostream>
#include <set>
#include <stdexcept>

namespace test {

   class service
   {  
   public:
      service(std::string const& name) : m_name(name), m_initialized(false) { }
      virtual ~service() { }
      void initialize()
      {
         initialize_service();
         m_initialized = true;
      }
      std::string const &name() const { return m_name; }
      bool ready() const { return m_dependencies.size() == 0; }
      virtual void initialize_service() = 0;
      void initialization_depends_on(std::shared_ptr<service> s)
      {
         if (s)
            m_dependencies.insert(s);
      }
      void remove_dependencies(std::set<std::shared_ptr<service> > const &remove_set)
      {
         for (auto i = remove_set.cbegin(); i != remove_set.cend(); ++i)
            m_dependencies.erase(*i);
      }

   private:
      std::string m_name;
      bool m_initialized;
      std::set<std::shared_ptr<service> > m_dependencies;
   };

   class renderer : public service
   {
   public:
      renderer() : service("renderer") { }
      ~renderer() { }
      void initialize_service() { }
   };

   class gui_renderer : public service
   {
   public:
      gui_renderer() : service("gui_renderer") { }
      ~gui_renderer() { }
      void connect(std::shared_ptr<renderer> renderer_)
      {
         m_renderer = renderer_;
         initialization_depends_on(renderer_);
      }
      void initialize_service() { assert(m_renderer); }

   private:
      std::shared_ptr<renderer> m_renderer;
   };

}

using namespace std;

int main(int argc, char *argv[])
{
   (void)argc;
   (void)argv;

   set<shared_ptr<test::service> > uninitialized;

   auto renderer_ = make_shared<test::renderer>();
   auto gui_renderer_ = make_shared<test::gui_renderer>();
   
   gui_renderer_->connect(renderer_);

   uninitialized.insert(renderer_);
   uninitialized.insert(gui_renderer_);

   set<shared_ptr<test::service> > remove_set;

   size_t uninitialized_count = uninitialized.size();

   cout << "Initializing " << uninitialized_count << " services:\n";

   while (uninitialized_count > 0)
   {
      remove_set.clear();

      for (auto i = uninitialized.begin(); i != uninitialized.end();)
      {
         auto s = *i;

         if (s->ready())
         {
            cout << "Initializing service: " << s->name() << "\n";
            s->initialize();  
            remove_set.insert(s);
            --uninitialized_count;
            i = uninitialized.erase(i);
         }
         else
         {
            ++i;
         }
      }

      if (remove_set.size() == 0)
         throw runtime_error("circular dependencies detected");

      cout << "Removing initialized services from dependencies...\n";

      for (auto i = uninitialized.begin(); i != uninitialized.end(); ++i)
      {
         auto s = *i;
         s->remove_dependencies(remove_set);
      }

      cout << "Done.\n";
   }
}

#else

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

   (void)argc;
   (void)argv;
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
#  if defined(NDEBUG)
   catch(...)
   {
      ;
   }
#  endif

   if (g_application)
      g_application->on_exit();

   g_application.reset();

   end_memory_compare();

   exit(return_value);
}


#endif