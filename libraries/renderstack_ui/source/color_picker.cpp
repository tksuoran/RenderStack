#include "renderstack_ui/color_picker.hpp"
#include "renderstack_ui/context.hpp"
#include "renderstack_ui/gui_renderer.hpp"
#include "renderstack_ui/ninepatch_style.hpp"
#include "renderstack_toolkit/math_util.hpp"
#include "renderstack_geometry/shapes/triangle.hpp"
#include "renderstack_geometry/shapes/disc.hpp"
#include "renderstack_mesh/geometry_mesh.hpp"
#include "renderstack_graphics/vertex_format.hpp"
#include <glm/glm.hpp>
#include <glm/gtx/constants.hpp>

namespace renderstack { namespace ui {

using namespace renderstack::toolkit;
using namespace renderstack::geometry;
using namespace renderstack::graphics;
using namespace renderstack::mesh;
using namespace std;
using namespace glm;

color_picker::color_picker(
   shared_ptr<class gui_renderer> gui_renderer,
   shared_ptr<class style> style
)
:  area        (gui_renderer, style)
,  m_ninepatch (gui_renderer, style->ninepatch_style())
,  m_size      (251.0f)
,  m_h         (0.0f)
,  m_s         (1.0f)
,  m_v         (1.0f)
{
   float inner_radius = 0.70f;
   m_disc_handle_radius = 0.90f;
   m_quad_edge_length = std::sqrt(2.0f) * inner_radius;
   auto gr = gui_renderer;
   auto &r = *gr->renderer();

   m_color_mesh = make_shared<geometry_mesh>(
      //new renderstack::geometry::shapes::disc(1.0, 0.9, 256, 2),
      r,
      make_shared<renderstack::geometry::shapes::quad>(std::sqrt(2.0f)),
      normal_style::polygon_normals
   );
   m_handle_mesh = make_shared<geometry_mesh>(
      r, 
      make_shared<renderstack::geometry::shapes::disc>(4.0, 1.00f, 12, 2),
      normal_style::polygon_normals
   );
   m_handle2_mesh = make_shared<geometry_mesh>(
      r,
      make_shared<renderstack::geometry::shapes::disc>(3.0, 2.00f, 12, 2),
      normal_style::polygon_normals
   );
   m_hsv_disc_mesh = make_shared<geometry_mesh>(
      r,
      make_shared<renderstack::geometry::shapes::disc>(1.0, m_disc_handle_radius, 256, 2),
      normal_style::polygon_normals
   );
   m_hsv_disc2_mesh = make_shared<geometry_mesh>(
      r,
      make_shared<renderstack::geometry::shapes::disc>(m_disc_handle_radius, inner_radius, 256, 2),
      normal_style::polygon_normals
   );
   m_hsv_quad_mesh = make_shared<geometry_mesh>(
      r,
      make_shared<renderstack::geometry::shapes::quad>(inner_radius),
      normal_style::polygon_normals
   );

   set_fill_base_pixels(vec2(m_size, m_size));
}
void color_picker::begin_place(rectangle const &reference, vec2 const &grow_direction)
{
   area::begin_place(reference, grow_direction);
   mat4 b;
   create_translation(rect().min(), b);
   mat4 const &o = renderer()->ortho();
   m_background_frame = o * b;
}
void color_picker::draw_self(ui_context &context)
{
   auto gr = renderer();
   auto &r = *gr->renderer();

   //r->push();

   gr->set_program(style()->ninepatch_style()->program());
   gr->set_texture(style()->ninepatch_style()->texture_unit(), style()->ninepatch_style()->texture());
   gr->begin_edit();
   gr->set_transform(m_background_frame);
   gr->set_color_scale(vec4(1.0f, 1.0f, 1.0f, 1.0f));
   gr->set_color_add(vec4(0.0f, 0.0f, 0.0f, 0.0f));
   gr->end_edit();
   m_ninepatch.render(gr);

   {
      animate();
      mat4 d;
      d[0][0] = 1.0f; d[1][0] = 0.0f; d[2][0] = 0.0f; d[3][0] = 0.0f; // h
      d[0][1] = 0.0f; d[1][1] = 0.0f; d[2][1] = 0.0f; d[3][1] = 1.0f; // s
      d[0][2] = 0.0f; d[1][2] = 0.0f; d[2][2] = 0.0f; d[3][2] = 1.0f; // v
      d[0][3] = 0.0f; d[1][3] = 0.0f; d[2][3] = 0.0f; d[3][3] = 0.0f;
      mat4 d2;
      d2[0][0] = 1.0f; d2[1][0] = 0.0f; d2[2][0] = 0.0f; d2[3][0] = 0.0f; // h
      d2[0][1] = 0.0f; d2[1][1] = 0.0f; d2[2][1] = 0.0f; d2[3][1] = m_s;  // s
      d2[0][2] = 0.0f; d2[1][2] = 0.0f; d2[2][2] = 0.0f; d2[3][2] = m_v;  // v
      d2[0][3] = 0.0f; d2[1][3] = 0.0f; d2[2][3] = 0.0f; d2[3][3] = 0.0f;
      mat4 t;
      t[0][0] = 0.0f; t[1][0] = 0.0f; t[2][0] = 0.0f; t[3][0] = m_h; // h
      t[0][1] = 1.0f; t[1][1] = 0.0f; t[2][1] = 0.0f; t[3][1] = 0.0f; // s
      t[0][2] = 0.0f; t[1][2] = 1.0f; t[2][2] = 0.0f; t[3][2] = 0.0f; // v
      t[0][3] = 0.0f; t[1][3] = 0.0f; t[2][3] = 0.0f; t[3][3] = 0.0f;
      mat4 c;
      c[0][0] = 0.0f; c[1][0] = 0.0f; c[2][0] = 0.0f; c[3][0] = m_h; // h
      c[0][1] = 0.0f; c[1][1] = 0.0f; c[2][1] = 0.0f; c[3][1] = m_s; // s
      c[0][2] = 0.0f; c[1][2] = 0.0f; c[2][2] = 0.0f; c[3][2] = m_v; // v
      c[0][3] = 0.0f; c[1][3] = 0.0f; c[2][3] = 0.0f; c[3][3] = 0.0f;

      mat4 h;
      h[0][0] = 0.0f; h[1][0] = 0.0f; h[2][0] = 0.0f; h[3][0] = 0.0; // h
      h[0][1] = 0.0f; h[1][1] = 0.0f; h[2][1] = 0.0f; h[3][1] = 0.0; // s
      h[0][2] = 0.0f; h[1][2] = 0.0f; h[2][2] = 0.0f; h[3][2] = 1.0; // v
      h[0][3] = 0.0f; h[1][3] = 0.0f; h[2][3] = 0.0f; h[3][3] = 0.0f;

      mat4 h0;
      h0[0][0] = 0.0f; h0[1][0] = 0.0f; h0[2][0] = 0.0f; h0[3][0] = 0.0; // h
      h0[0][1] = 0.0f; h0[1][1] = 0.0f; h0[2][1] = 0.0f; h0[3][1] = 0.0; // s
      h0[0][2] = 0.0f; h0[1][2] = 0.0f; h0[2][2] = 0.0f; h0[3][2] = 0.0; // v
      h0[0][3] = 0.0f; h0[1][3] = 0.0f; h0[2][3] = 0.0f; h0[3][3] = 0.0f;

      gr->set_program(style()->program());

      gr->blend_disable();

      auto old_vbo = r.set_buffer(buffer_target::array_buffer, m_color_mesh->get_mesh()->vertex_buffer());

      // TODO must access VAO instead!
      auto old_ibo = r.set_buffer(buffer_target::element_array_buffer, m_color_mesh->get_mesh()->index_buffer());

      gr->begin_edit();
      gr->set_transform(m_hsv_transform);
      gr->set_hsv_matrix(c);
      gr->set_color_scale(vec4(1.0f, 1.0f, 1.0f, 1.0f));
      gr->set_color_add(vec4(0.0f, 0.0f, 0.0f, 1.0f));
      gr->end_edit();
      //m_color_mesh->render(m_color_mesh->fill_indices());

      gr->begin_edit();
      gr->set_hsv_matrix(d);
      gr->end_edit();
      //m_hsv_disc_mesh->render(gl::begin_mode::triangles, m_hsv_disc_mesh->fill_indices());

      gr->begin_edit();
      gr->set_hsv_matrix(d2);
      gr->end_edit();
      //m_hsv_disc2_mesh->render(gl::begin_mode::triangles, m_hsv_disc2_mesh->fill_indices());

      gr->begin_edit();
      gr->set_hsv_matrix(t);
      gr->set_color_add(vec4(0.0f, 0.0f, 0.0f, 0.5f));
      gr->end_edit();
      //m_hsv_quad_mesh->render(gl::begin_mode::triangles, m_hsv_quad_mesh->fill_indices());

      if (rect().hit(context.mouse))
      {
         if (context.mouse_buttons[0 /* TODO glfw_mouse_button_left*/])
         {
            double x = context.mouse.x;
            double y = context.mouse.y;
            unsigned char data[4];

            gl::read_pixels(dround(x), dround(y), 1, 1, gl::pixel_format::rgba, gl::pixel_type::unsigned_byte, data);

            float r2 = float(data[0]) / 255.0f;
            float g2 = float(data[1]) / 255.0f;
            float b2 = float(data[2]) / 255.0f;
            float h2, s2, v2;

            rgb_to_hsv(r2, g2, b2, h2, s2, v2);
            if (data[3] == 255)
            {
               m_h = h2 / 360.0f;
            }
            else if (data[3] == 128)
            {
               m_s = s2;
               m_v = v2;
            }
         }
      }

#if 0
      //  draw handles
      {
         gr->begin_edit();
         gr->set_transform(m_disc_handle_transform);
         gr->set_hsv_matrix(h0);
         gr->set_color_add(vec4(0.0f, 0.0f, 0.0f, 0.0f));
         gr->end_edit();
         m_handle_mesh->render(gl::begin_mode::triangles, m_handle_mesh->fill_indices());

         gr->begin_edit();
         gr->set_hsv_matrix(h);
         gr->end_edit();
         m_handle2_mesh->render(gl::begin_mode::triangles, m_handle_mesh->fill_indices());

         gr->begin_edit();
         gr->set_transform(m_quad_handle_transform);
         gr->set_hsv_matrix(h0);
         gr->end_edit();
         m_handle_mesh->render(gl::begin_mode::triangles, m_handle_mesh->fill_indices());

         gr->begin_edit();
         gr->set_hsv_matrix(h);
         gr->end_edit();
         m_handle2_mesh->render(gl::begin_mode::triangles, m_handle_mesh->fill_indices());
      }
#endif
   }

   gr->prepare();
}

void color_picker::animate()
{
   float scale = (m_size - 10.0f) / 2.0f;

   mat4 const &o = renderer()->ortho();

   mat4 t;
   create_translation(rect().min() + rect().half_size(), t);
   //m_background_frame = o * t;

   m_disc_handle_transform = o * t;

   mat4 qh;
   create_translation(
      rect().min() + 
      rect().half_size() - (0.5f * scale * vec2(m_quad_edge_length)) + 
      (scale * vec2(m_s * m_quad_edge_length, m_v * m_quad_edge_length)), 
      qh
   );
   m_quad_handle_transform = o * qh;

   mat4 dh;
   float sin = m_disc_handle_radius * std::sin(m_h * 2.0f * pi<float>());
   float cos = m_disc_handle_radius * std::cos(m_h * 2.0f * pi<float>());
   create_translation(
      rect().min() + rect().half_size() + scale * vec2(cos, sin), 
      dh
   );
   m_disc_handle_transform = o * dh;

   mat4 s;
   create_scale(scale, s);

   //mat4 r;
   //create_rotation(m_h * 2.0f * pi<float>(), vec3(0.0f, 0.0f, 1.0f), r);

   m_hsv_transform = o * t * s;
   //m_quad_model_to_clip = m_disc_model_to_clip * r;
}

} }
