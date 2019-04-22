#include "renderstack_toolkit/platform.hpp"
#include "renderstack_toolkit/math_util.hpp"
#include "renderers/light_mesh.hpp"
#include "main/programs.hpp"
#include "renderstack_geometry/shapes/cone.hpp"
#include "renderstack_geometry/shapes/triangle.hpp" // quad is currently here...
#include "renderstack_graphics/vertex_format.hpp"
#include "renderstack_mesh/geometry_mesh.hpp"
#include "renderstack_mesh/mesh.hpp"
#include "renderstack_scene/light.hpp"
#include <glm/glm.hpp>


using namespace renderstack::toolkit;
using namespace renderstack::mesh;
using namespace renderstack::scene;
using namespace renderstack;
using namespace glm;
using namespace std;


light_mesh::light_mesh()
:  service("light_mesh")
{
}

/*virtual*/ light_mesh::~light_mesh()
{
}

void light_mesh::connect(
   shared_ptr<renderstack::graphics::renderer> renderer,
   shared_ptr<class programs> programs
)
{
   m_renderer = renderer;
   m_programs = programs;

   initialization_depends_on(renderer);
   initialization_depends_on(programs);
}

void light_mesh::initialize_service()
{
   renderstack::mesh::geometry_mesh_format_info format_info;

   format_info.set_want_fill_triangles(true);
   format_info.set_want_edge_lines(true);
   format_info.set_want_position(true);
   format_info.set_vertex_attribute_mappings(m_programs->attribute_mappings);

   // Full screen quad
   {
      // -1 .. 1
      auto g = make_shared<renderstack::geometry::shapes::quad>(2.0f);
      g->build_edges();
      renderstack::mesh::geometry_mesh_buffer_info buffer_info;
      geometry_mesh::prepare_vertex_format(g, format_info, buffer_info);
      auto &r = *m_renderer;

      auto gm = make_shared<geometry_mesh>(
         r,
         g,
         format_info,
         buffer_info
      );

      m_quad_mesh = gm;
   }

   // Spot light cone
   {
      m_light_cone_sides = 11;

      auto g = make_shared<renderstack::geometry::shapes::conical_frustum>(
         0.0,                 // min x
         1.0,                 // max x
         0.0,                 // bottom radius
         1.0,                 // top radius
         false,               // use bottom
         true,                // use top (end)
         m_light_cone_sides,  // slice count
         0                    // stack division
      );

      g->transform(mat4_rotate_xz_cw);
      g->build_edges();
      renderstack::mesh::geometry_mesh_buffer_info buffer_info;
      geometry_mesh::prepare_vertex_format(g, format_info, buffer_info);
      auto &r = *m_renderer;

      auto gm = make_shared<renderstack::mesh::geometry_mesh>(
         r,
         g,
         format_info,
         buffer_info
      );

      m_cone_mesh = gm;
   }

}

glm::mat4 light_mesh::get_light_transform(
   shared_ptr<renderstack::scene::light> l
)
{
   switch (l->type())
   {
   case light_type::directional:
      return mat4(1.0f);

   case light_type::point:
      return mat4(1.0f);

   case light_type::spot:
      {
         //           Side:                     Bottom:              .
         //             .                    ______________          .
         //            /|\                  /       |    / \         .
         //           / | \                /   apothem  /   \        .
         //          /  +--+ alpha        /         | radius \       .
         //         /   |   \            /          | /       \      .
         //        /    |    \          /           |/         \     .
         //       /     |     \         \                      /     .
         //      /      |      \         \                    /      .
         //     /     length    \         \                  /       .
         //    /        |        \         \                /        .
         //   /         |         \         \______________/         .
         //  +----------+----------+                                 .

         float alpha    = l->spot_angle(); 
         float length   = l->range();
         float apothem  = length * std::tan(alpha * 0.5f);
         float radius   = apothem / std::cos(glm::pi<float>() / static_cast<float>(m_light_cone_sides));

         mat4 scale;
         create_scale(radius, radius, length, scale);

         return scale;
      }

   default:
      return mat4(1.0f);
   }
}

bool light_mesh::point_in_light(
   vec3 p,
   shared_ptr<renderstack::scene::light> l
)
{
   if (l->type() != light_type::spot)
      return true;

   float spot_angle  = l->spot_angle() * 0.5f;
   float outer_angle = spot_angle / std::cos(glm::pi<float>() / static_cast<float>(m_light_cone_sides));
   float spot_cutoff = std::cos(outer_angle);
   float range       = l->range();

   mat4 const &light_from_world = l->frame()->world_from_local().inverse_matrix();
   vec3 view_in_light = vec3(light_from_world * vec4(p, 1.0f));
   float distance = -view_in_light.z;
   view_in_light = normalize(view_in_light);

   float cos_angle = dot(view_in_light, vec3(0.0f, 0.0f, -1.0f));
   bool outside_cone_angle = (cos_angle < spot_cutoff);
   bool outside_cone_range = (distance < 0.0f) || (distance > range);
   if (outside_cone_angle || outside_cone_range)
      return false;
   else
      return true;
}

shared_ptr<renderstack::mesh::geometry_mesh> light_mesh::get_light_mesh(
   shared_ptr<light> l
)
{
   switch (l->type())
   {
   case light_type::directional:
      return m_quad_mesh;

   case light_type::point:
      return nullptr;

   case light_type::spot:
      return m_cone_mesh;

   default:
      return nullptr;
   }
}
