#include "renderstack_toolkit/platform.hpp"
#include "main/application.hpp"
#include "renderstack_toolkit/window.hpp"
#include "renderstack_toolkit/gl.hpp"
#include "renderstack_toolkit/strong_gl_enums.hpp"
#include "renderstack_ui/button.hpp"
#include "renderstack_ui/choice.hpp"
#include "renderstack_ui/color_picker.hpp"
#include "renderstack_ui/context.hpp"
#include "renderstack_ui/dock.hpp"
#include "renderstack_ui/layer.hpp"
#include "renderstack_ui/menulist.hpp"
#include "renderstack_ui/push_button.hpp"
#include "renderstack_ui/slider.hpp"
#include "renderstack_geometry/cone.hpp"
#include "renderstack_graphics/configuration.hpp"
#include "renderstack_geometry/disc.hpp"
#include "renderstack_graphics/renderer.hpp"
#include "renderstack_geometry/sphere.hpp"
#include "renderstack_geometry/triangle.hpp"
#include "renderstack_graphics/vertex_stream_mappings.hpp"
#include "renderstack_mesh/geometry_mesh.hpp"

#include <cassert>
//#include <GL/glfw3.h>

using namespace gl;
using namespace glm;
using namespace std;
using namespace renderstack::graphics;
using namespace renderstack::toolkit;
using namespace renderstack::mesh;
using namespace renderstack::ui;


void application::execute_tests()
{
#if 1
   check_memory_system();

   m_gui_renderer.reset();
   m_renderer.reset();

   begin_memory_compare();

   m_renderer = make_shared<renderstack::graphics::renderer>();
   m_gui_renderer = make_shared<renderstack::ui::gui_renderer>(m_renderer);

   /*auto cone = make_shared<renderstack::geometry::shapes::cone>(
   0.0f, 1.0f, 1.0f, 0.1f, true, true, 24, 8
   );*/
   auto triangle = make_shared<renderstack::geometry::shapes::triangle>(1.0);

   auto tri_mesh = make_shared<geometry_mesh>(*m_renderer, triangle, normal_style::corner_normals);

   tri_mesh.reset();
   //cone.reset();
   triangle.reset();

   m_gui_renderer.reset();
   m_renderer.reset();

   end_memory_compare();

   check_memory_system();
#endif
}

