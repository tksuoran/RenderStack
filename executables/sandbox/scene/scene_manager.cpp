#include "scene/scene_manager.hpp"
#include "renderstack_scene/camera.hpp"
#include "renderstack_scene/light.hpp"
#include "renderstack_geometry/shapes/sphere.hpp"
#include "renderstack_geometry/shapes/disc.hpp"
#include "renderstack_geometry/shapes/triangle.hpp"
#include "renderstack_geometry/shapes/cone.hpp"
#include "renderstack_geometry/shapes/cube.hpp"
#include "renderstack_geometry/shapes/torus.hpp"
#include "renderstack_geometry/operation/clone.hpp"
#include "renderstack_geometry/operation/catmull_clark.hpp"
#include "renderstack_mesh/geometry_mesh.hpp"
#include "scene/model.hpp"
#include "main/programs.hpp"
#include "parsers/xml_polyhedron.hpp"

using namespace renderstack::graphics;
using namespace renderstack::toolkit;
using namespace renderstack::scene;
using namespace renderstack::mesh;
using namespace std;
using namespace glm;


scene_manager::scene_manager()
:  service("scene_manager")
,  m_camera          (nullptr)
,  m_camera_controls (nullptr)
{
}

/* virtual */ scene_manager::~scene_manager()
{
}

void scene_manager::connect(
   std::shared_ptr<programs>                          programs_,
   std::shared_ptr<renderstack::graphics::renderer>   renderer_
)
{
   m_programs = programs_;
   m_renderer = renderer_;

   initialization_depends_on(programs_);
   initialization_depends_on(renderer_);
}

/*virtual*/ void scene_manager::initialize_service()
{
   m_models = make_shared<vector<shared_ptr<model> > >();

   m_camera = make_shared<renderstack::scene::camera>();
   m_camera->projection().set_fov_y(1.0f / 1.5f);
   m_camera->projection().set_near(0.01f);
   m_camera->projection().set_far(1000.0f);

   m_lights = make_shared<vector<shared_ptr<light> > >();

   //add_floor(20.0f);
   add_simple_scene();

   initialize_cameras();
}

void scene_manager::reset_build_info()
{
   m_format_info = geometry_mesh_format_info();
   m_buffer_info = geometry_mesh_buffer_info();
}

shared_ptr<model> scene_manager::make_model(
   shared_ptr<renderstack::scene::frame> parent,
   shared_ptr<renderstack::geometry::geometry> g,
   vec3 position
)
{
   assert(m_renderer);

   auto &r = *m_renderer;

   geometry_mesh::prepare_vertex_format(g, m_format_info, m_buffer_info);
   mat4 transform;
   create_translation(position, transform);

   auto gm = make_shared<renderstack::mesh::geometry_mesh>(r, g, m_format_info, m_buffer_info);
   auto m = make_shared<model>();
   m->set_geometry_mesh(gm);
   m->frame()->set_parent(parent);
   m->frame()->parent_from_local().set(transform);
   m->frame()->update_hierarchical_no_cache();

   return m;
}


void scene_manager::add_floor(float size)
{
   (void) size;
   // TODO
}

void scene_manager::add_simple_scene()
{
   {
      auto sun = make_shared<light>();
      sun->set_type(light_type::directional);
      sun->set_color(vec3(1.0f, 1.0f, 1.0f));
      sun->set_intensity(1.0f);
      sun->set_name("sun");
      mat4 m;
      create_look_at(
         vec3(0.0f, 10.0f, 0.0f), // eye
         vec3(0.0f,  0.0f, 0.0f), // center
         vec3(0.0f,  0.0f, 1.0f), // up
         m
      );
      sun->frame()->parent_from_local().set(m);
      sun->frame()->update_hierarchical_no_cache();
      add(sun);
   }

   vector<shared_ptr<renderstack::geometry::geometry>> g_collection;

   //g_collection.push_back(make_shared<renderstack::geometry::shapes::sphere>(1.0f, 12, 4));
#if 1
   auto floor_g = make_shared<renderstack::geometry::shapes::cube>(
      vec3(40.0f, 1.0f, 40.0f),
      ivec3(1, 1, 1),
      1.0f
   );

   g_collection.push_back(make_shared<renderstack::geometry::shapes::disc>          (1.0, 0.8, 32, 2));
   g_collection.push_back(make_shared<renderstack::geometry::shapes::triangle>      (0.8f / 0.57735027f));
   g_collection.push_back(make_shared<renderstack::geometry::shapes::sphere>        (1.0f, 12 * 4, 4 * 6));
   g_collection.push_back(make_shared<renderstack::geometry::shapes::torus>         (0.6f, 0.3f, 42, 32));
   g_collection.push_back(make_shared<renderstack::geometry::shapes::cuboctahedron> (1.0));
   g_collection.push_back(make_shared<renderstack::geometry::shapes::dodecahedron>  (1.0));
   g_collection.push_back(make_shared<renderstack::geometry::shapes::icosahedron>   (1.0));
   g_collection.push_back(make_shared<renderstack::geometry::shapes::octahedron>    (1.0));
   g_collection.push_back(make_shared<renderstack::geometry::shapes::tetrahedron>   (1.0));

   g_collection.push_back(make_shared<renderstack::geometry::shapes::dodecahedron>(2.0));
   auto g0 = make_shared<renderstack::geometry::shapes::dodecahedron>(2.0);
   g0->build_edges();

   auto o1 = make_shared<renderstack::geometry::operation::catmull_clark>(g0);
   auto g1 = o1->destination();

   auto o2 = make_shared<renderstack::geometry::operation::catmull_clark>(g1);
   auto g2 = o2->destination();

   g2->compute_polygon_normals();
   g2->compute_point_normals("point_normals");
   g_collection.push_back(g2);

   auto xml = make_shared<xml_polyhedron>("res/polyhedra/127.xml");
   g2->compute_polygon_normals();
   g_collection.push_back(xml);

#endif

#if 0
   {
      auto body = make_shared<cube>(8, 12, 4);
      //auto head = make_shared<cube>(8,  8, 8);
      //auto hand = make_shared<cube>(4, 12, 4);
      //auto foot = make_shared<cube>(4, 12, 4);
      g_collection.push_back(body->mesh());
   }
#endif

   // Count how big VBO and IBO is needed,
   // and which vertex attributes are needed.
   // Edges need to be built here in order for buffer size
   // computations to be able to include line indices.
#if SHARED_BUFFERS
   renderstack::geometry::geometry::mesh_info total_info;
#endif
   renderstack::geometry::geometry::mesh_info info;
   reset_build_info();
   m_format_info.set_want_fill_triangles(true);
   m_format_info.set_want_edge_lines(true);
   m_format_info.set_want_position(true);
   m_format_info.set_want_normal(true);
   m_format_info.set_want_tangent(true);
   m_format_info.set_want_texcoord(true);
   m_format_info.set_want_color(true);
   m_format_info.set_want_id(true);
   m_format_info.set_normal_style(normal_style::corner_normals);
   m_format_info.set_mappings(m_programs->mappings);
   for (auto i = g_collection.begin(); i != g_collection.end(); ++i)
   {
      (*i)->build_edges();
      (*i)->info(info);
#if SHARED_BUFFERS
      total_info += info;
#endif
      geometry_mesh::prepare_vertex_format(*i, m_format_info, m_buffer_info);
   }

   floor_g->build_edges();
   floor_g->info(info);
   geometry_mesh::prepare_vertex_format(floor_g, m_format_info, m_buffer_info);

#if SHARED_BUFFERS
   size_t total_vertex_count = 0;
   size_t total_index_count = 0;
   total_vertex_count += total_info.vertex_count_corners;
   if (format_info.want_centroid_points())
      total_vertex_count += total_info.vertex_count_centroids;
      
   if (format_info.want_fill_triangles())
      total_index_count += total_info.index_count_fill_triangles;

   if (format_info.want_edge_lines())
      total_index_count += total_info.index_count_edge_lines;

   if (format_info.want_corner_points())
      total_index_count += total_info.index_count_corner_points;

   if (format_info.want_centroid_points())
      total_index_count += total_info.index_count_centroid_points;

   // Allocate a single VBO big enough to hold all vertices
   auto vbo = make_shared<renderstack::graphics::buffer>(
      renderstack::graphics::buffer_target::array_buffer,
      total_vertex_count,
      buffer_info.vertex_format()->stride(),
      gl::buffer_usage_hint::static_draw
   );
   vbo->allocate_storage(*m_renderer);
   m_buffer_info.set_vertex_buffer(vbo);

   // Allocate a single IBO big enough to hold all indices
   auto ibo = make_shared<renderstack::graphics::buffer>(
      renderstack::graphics::buffer_target::element_array_buffer,
      total_index_count,
      4,                                  // stride
      gl::buffer_usage_hint::static_draw
   );
   ibo->allocate_storage(*m_renderer);

   m_buffer_info.set_index_buffer(ibo);
#endif
      
   for (auto i = g_collection.begin(); i != g_collection.end(); ++i)
   {
      auto m = make_model(nullptr, *i, vec3(0, 0.0f, 0.0f));
      m_models->push_back(m);
   }

   float total_width = 0.0f;
   for (auto i = m_models->cbegin(); i != m_models->cend(); ++i)
   {
      auto m = *i;
      auto gm = m->geometry_mesh();
      vec3 min = gm->min();
      vec3 max = gm->max();
      float width = max.x - min.x;
      total_width += width;
   }

   float gap = 0.5f;
   float x_pos = -total_width + m_models->size() * gap;
   for (auto i = m_models->cbegin(); i != m_models->cend(); ++i)
   {
      auto m = *i;
      auto gm = m->geometry_mesh();
      vec3 min = gm->min();
      vec3 max = gm->max();
      float width = max.x - min.x;

      mat4 transform;

      x_pos += 0.5f * (width + gap);
      create_translation(x_pos, -min.y, 0.0f, transform);
      x_pos += 0.5f * (width + gap);

      m->frame()->parent_from_local().set(transform);
      m->frame()->update_hierarchical_no_cache();

   }

   int n_lights = 10;
   for (int i = 0; i < n_lights; ++i)
   {
      float rel = static_cast<float>(i) / static_cast<float>(n_lights);
      float h = rel * 360.0f;
      float s = 0.7f;
      float v = 1.0f;
      float r, g, b;

      hsv_to_rgb(h, s, v, r, g, b);

      auto l = make_shared<light>();
      l->set_type(light_type::spot);
      l->set_color(vec3(r, g, b));
      l->set_intensity(1.0f);
      l->set_name("spot");
      mat4 m;

      x_pos = (rel - 0.5f) * total_width;

      create_look_at(
         vec3(x_pos, 8.0f, 0.0f), // eye
         vec3(x_pos, 0.0f, 0.0f), // center
         vec3( 0.0f, 0.0f, 1.0f), // up
         m
      );
      l->set_range(16.0f);
      l->set_spot_angle(glm::pi<float>() / 8.0f);
      l->frame()->parent_from_local().set(m);
      l->frame()->update_hierarchical_no_cache();
      add(l);
   } 


   m_format_info.set_normal_style(normal_style::polygon_normals);
   auto floor_m = make_model(nullptr, floor_g, vec3(0, -0.5f, 0.0f));
   m_models->push_back(floor_m);

# if 0
   {
      reset_build_info();
      m_format_info.set_want_fill_triangles(true);
      m_format_info.set_want_position(true);
      m_format_info.set_want_normal(true);
      m_format_info.set_want_tangent(true);
      m_format_info.set_want_color(true);
      m_format_info.set_want_texcoord(true);
      m_format_info.set_want_id(true);
      m_format_info.set_normal_style(normal_style::corner_normals);
      m_format_info.set_mappings(m_programs->mappings);

      m_manipulator_frame = make_shared<renderstack::scene::frame>();
      m_format_info.set_constant_color(vec4(1.0f, 0.0f, 0.0f, 1.0f));
      mat4 x_to_y;
      mat4 x_to_z;
         
      auto x_tip_g  = make_shared<renderstack::geometry::shapes::cone      >(0.75f, 1.00f, 0.15f, true, 18, 2);
      auto x_tail_g = make_shared<renderstack::geometry::shapes::cylinder  >(0.00f, 0.75f, 0.03f, true, false, 12, 2);
      auto x_tip_m  = make_model(m_manipulator_frame, x_tip_g);
      auto x_tail_m = make_model(m_manipulator_frame, x_tail_g);
      m_manipulator_models = make_shared<vector<shared_ptr<model> > >();
      m_manipulator_models->push_back(x_tip_m);
      m_manipulator_models->push_back(x_tail_m);
   }
# endif
}

void scene_manager::initialize_cameras()
{
   m_camera = make_shared<renderstack::scene::camera>();
   m_camera->projection().set_fov_y(degrees_to_radians(50.0f));
   m_camera->projection().set_projection_type(renderstack::scene::projection_type::perspective_vertical);
   m_camera->projection().set_near(0.02f);
   m_camera->projection().set_far(40.0f);

   glm::mat4 m;
   create_look_at(
      glm::vec3(0.0f, 4.0f, 15.0f),
      glm::vec3(0.0f, 0.0f, 0.0f),
      glm::vec3(0.0f, 1.0f, 0.0f),
      m
   );
   m_camera->frame()->parent_from_local().set(m);

   //m_camera_controls->s
}

shared_ptr<model> scene_manager::add(shared_ptr<model> m)
{
   if (!m)
      throw runtime_error("scene_manager::add() no model to add");

   m_models->push_back(m);

   return m;
}

shared_ptr<light> scene_manager::add(shared_ptr<light> l)
{
   if (!l)
      throw runtime_error("scene_manager::add() no light to add");

   m_lights->push_back(l);

   return l;
}

shared_ptr<vector<shared_ptr<model> > > &scene_manager::models()
{
   return m_models;
}

shared_ptr<vector<shared_ptr<model> > > const &scene_manager::models() const
{
   return m_models;
}

shared_ptr<vector<shared_ptr<light> > > &scene_manager::lights()
{
   return m_lights;
}

shared_ptr<vector<shared_ptr<light> > > const &scene_manager::lights() const
{
   return m_lights;
}

