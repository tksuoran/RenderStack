#include "scene/scene_manager.hpp"
#include "main/programs.hpp"
#include "parsers/json_polyhedron.hpp"
#include "renderstack_geometry/operation/catmull_clark.hpp"
#include "renderstack_geometry/operation/clone.hpp"
#include "renderstack_geometry/shapes/cone.hpp"
#include "renderstack_geometry/shapes/cube.hpp"
#include "renderstack_geometry/shapes/disc.hpp"
#include "renderstack_geometry/shapes/sphere.hpp"
#include "renderstack_geometry/shapes/torus.hpp"
#include "renderstack_geometry/shapes/triangle.hpp"
#include "renderstack_mesh/geometry_mesh.hpp"
#include "renderstack_scene/camera.hpp"
#include "renderstack_scene/light.hpp"
#include "scene/material.hpp"
#include "scene/model.hpp"

using namespace renderstack::graphics;
using namespace renderstack::toolkit;
using namespace renderstack::scene;
using namespace renderstack::mesh;
using namespace std;
using namespace glm;

Scene_manager::Scene_manager()
    : service("Scene_manager")
{
}

void Scene_manager::connect(std::shared_ptr<Programs> programs,
                            std::shared_ptr<Renderer> renderer)
{
    m_programs = programs;
    m_renderer = renderer;

    initialization_depends_on(programs);
    initialization_depends_on(renderer);
}

void Scene_manager::initialize_service()
{
    m_camera                   = std::make_shared<Camera>();
    m_camera->projection.fov_y = 1.0f / 1.5f;
    m_camera->projection.near  = 0.01f;
    m_camera->projection.far   = 1000.0f;

    //add_floor(20.0f);
    add_simple_scene();

    initialize_cameras();
}

void Scene_manager::reset_build_info()
{
    m_format_info = Geometry_mesh::Format_info();
    m_buffer_info = Geometry_mesh::Buffer_info();
}

std::shared_ptr<Geometry_mesh> Scene_manager::make_geometry_mesh(
    const renderstack::geometry::Geometry &geometry)
{
    Geometry_mesh::prepare_vertex_format(geometry,
                                         m_format_info,
                                         m_buffer_info);

    auto &r = *m_renderer;

    auto gm = make_shared<Geometry_mesh>(r,
                                         geometry,
                                         m_format_info,
                                         m_buffer_info);

    return gm;
}

shared_ptr<Model> Scene_manager::make_model(
    const std::string &       name,
    shared_ptr<Frame>         parent,
    shared_ptr<Geometry_mesh> gm,
    shared_ptr<Material>      material,
    vec3                      position)
{
    assert(m_renderer);

    mat4 transform;
    create_translation(position, transform);

    auto m = make_shared<Model>(name);
    m->geometry_mesh = gm;
    m->material = material;
    m->frame.parent = parent.get();
    m->frame.parent_from_local.set(transform);
    m->frame.update_hierarchical_no_cache();

    return m;
}

void Scene_manager::add_floor(float size)
{
    (void)size;
    // TODO
}

void Scene_manager::add_simple_scene()
{
    auto material_default = make_shared<Material>(m_materials.size(),
                                                  "default",
                                                  vec4(1.0f, 1.0f, 1.0f, 1.0f),
                                                  0.5f,
                                                  0.5f);
    m_materials.push_back(material_default);

    auto material_floor = make_shared<Material>(m_materials.size(),
                                                "floor",
                                                vec4(0.6f, 0.6f, 1.0f, 1.0f),
                                                0.03f,
                                                0.005f);
    m_materials.push_back(material_floor);

#if 0
   {
      auto sun = make_shared<Light>();
      sun->set_type(Light::Type::directional);
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
#endif

    vector<renderstack::geometry::Geometry> g_collection;

    //g_collection.push_back(make_shared<renderstack::geometry::shapes::sphere>(1.0f, 12, 4));
    auto floor_g = renderstack::geometry::shapes::make_cube(vec3(40.0f, 1.0f, 40.0f),
                                                            ivec3(1, 1, 1),
                                                            1.0f);

    //g_collection.push_back(make_shared<renderstack::geometry::shapes::disc>          (1.0, 0.8, 32, 2));
    //g_collection.push_back(make_shared<renderstack::geometry::shapes::triangle>      (0.8f / 0.57735027f));
    g_collection.push_back(renderstack::geometry::shapes::make_sphere(1.0f, 12 * 4, 4 * 6));
    g_collection.push_back(renderstack::geometry::shapes::make_torus(0.6f, 0.3f, 42, 32));
    g_collection.push_back(renderstack::geometry::shapes::make_cuboctahedron(1.0));
    g_collection.push_back(renderstack::geometry::shapes::make_dodecahedron(1.0));
    g_collection.push_back(renderstack::geometry::shapes::make_icosahedron(1.0));
    g_collection.push_back(renderstack::geometry::shapes::make_octahedron(1.0));
    g_collection.push_back(renderstack::geometry::shapes::make_tetrahedron(1.0));

    {
        g_collection.push_back(renderstack::geometry::shapes::make_dodecahedron(2.0));
        auto g0 = renderstack::geometry::shapes::make_dodecahedron(2.0);
#if 0
        g0->build_edges();

        auto o1 = make_shared<renderstack::geometry::operation::catmull_clark>(g0);
        auto g1 = o1->destination();

        auto o2 = make_shared<renderstack::geometry::operation::catmull_clark>(g1);
        auto g2 = o2->destination();

        g2->compute_polygon_normals();
        g2->compute_point_normals("point_normals");
        g_collection.push_back(g2);
#endif
        g_collection.push_back(std::move(g0));
    }

    {
        auto mesh = make_json_polyhedron("res/polyhedra/127.json");
        mesh.compute_polygon_normals();
        g_collection.push_back(std::move(mesh));
    }

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
    renderstack::geometry::Geometry::Mesh_info total_info;
#endif
    renderstack::geometry::Geometry::Mesh_info info;
    reset_build_info();
    m_format_info.want_fill_triangles       = true;
    m_format_info.want_edge_lines           = true;
    m_format_info.want_position             = true;
    m_format_info.want_normal               = true;
    m_format_info.want_tangent              = true;
    m_format_info.want_texcoord             = true;
    m_format_info.want_color                = true;
    m_format_info.want_id                   = true;
    m_format_info.normal_style              = Normal_style::corner_normals;
    m_format_info.vertex_attribute_mappings = &m_programs->attribute_mappings;
    for (auto &geometry : g_collection)
    {
        geometry.build_edges();
        geometry.info(info);
#if SHARED_BUFFERS
        total_info += info;
#endif
        Geometry_mesh::prepare_vertex_format(geometry, m_format_info, m_buffer_info);
    }

    floor_g.build_edges();
    floor_g.info(info);
    Geometry_mesh::prepare_vertex_format(floor_g, m_format_info, m_buffer_info);

#if SHARED_BUFFERS
    size_t total_vertex_count = 0;
    size_t total_index_count  = 0;
    total_vertex_count += total_info.vertex_count_corners;
    if (format_info.want_centroid_points)
    {
        total_vertex_count += total_info.vertex_count_centroids;
    }

    if (format_info.want_fill_triangles)
    {
        total_index_count += total_info.index_count_fill_triangles;
    }

    if (format_info.want_edge_lines)
    {
        total_index_count += total_info.index_count_edge_lines;
    }

    if (format_info.want_corner_points)
    {
        total_index_count += total_info.index_count_corner_points;
    }

    if (format_info.want_centroid_points)
    {
        total_index_count += total_info.index_count_centroid_points;
    }

    // Allocate a single VBO big enough to hold all vertices
    auto vbo = make_shared<Buffer>(
        Buffer::Target::array_buffer,
        total_vertex_count,
        buffer_info.vertex_format()->stride(),
        gl::buffer_usage_hint::static_draw);
    vbo->allocate_storage(*m_renderer);
    m_buffer_info.set_vertex_buffer(vbo);

    // Allocate a single IBO big enough to hold all indices
    auto ibo = make_shared<Buffer>(
        Buffer::Target::element_array_buffer,
        total_index_count,
        4, // stride
        gl::buffer_usage_hint::static_draw);
    ibo->allocate_storage(*m_renderer);

    m_buffer_info.set_index_buffer(ibo);
#endif

    float min_x = 0.0f;
    float max_x = 0.0f;
    float gap   = 0.5f;

    size_t material_start = m_materials.size();
    for (float z = -15.0f; z < 15.1f; z += 5.0f)
    {
        float rel = (z + 15.0f) / 30.0f;
        rel       = 0.7f * rel + 0.3f;
        float r   = 0.020f; // pow(rel, 5.0);
        float p   = 0.005f; //pow(rel, 5.0);
        float h   = rel * 360.0f;
        float s   = 0.9f;
        float v   = 1.0f;
        float R, G, B;

        hsv_to_rgb(h, s, v, R, G, B);

        auto m = make_shared<Material>(m_materials.size(),
                                       "m",
                                       vec4(1.0f, 1.0f, 1.0f, 1.0f),
                                       //vec4(R, G, B, 1.0f),
                                       r,
                                       p);
        m_materials.push_back(m);
    }

    if (true)
    {
        for (auto &geometry : g_collection)
        {
            auto gm = make_geometry_mesh(geometry);

            vec3  min   = gm->min();
            vec3  max   = gm->max();
            float width = max.x - min.x;

            float x;
            if (fabs(min_x) < fabs(max_x))
            {
                min_x -= 0.5f * gap;
                min_x -= width * 0.5f;
                x = min_x;
                min_x -= width * 0.5f;
                min_x -= 0.5f * gap;
            }
            else
            {
                max_x += 0.5f * gap;
                max_x += width * 0.5f;
                x = max_x;
                max_x += width * 0.5f;
                max_x += 0.5f * gap;
            }

            size_t material_index = material_start;
            for (float z = -15.0f; z < 15.1f; z += 5.0f)
            {
                shared_ptr<Material> mat = m_materials.at(material_index);

                auto m = make_model(geometry.name(),
                                    nullptr,
                                    gm,
                                    mat,
                                    vec3(x, -min.y, z));
                m_models.push_back(m);
                ++material_index;
            }
        }
    }

    if (false)
    {
        auto l = make_shared<Light>();
        l->type = Light::Type::directional;
        l->color = vec3(0.11f, 0.23f, 1.00f);
        l->intensity = 0.0663f;
        l->camera.name = "atmosphere";

        mat4 m;
        create_look_at(vec3(0.0f, 10.0f, 0.0f), // eye
                       vec3(0.0f, 0.0f, 0.0f),  // center
                       vec3(0.0f, 0.0f, 1.0f),  // up
                       m);
        l->range = 1000.0f;
        l->spot_angle = glm::pi<float>() * 1.0f;
        l->camera.frame.parent_from_local.set(m);
        l->camera.frame.update_hierarchical_no_cache();
        add(l);
    }

    {
        auto l = make_shared<Light>();
        l->type = Light::Type::spot;
        l->color = vec3(1.0f, 1.0f, 1.0f);
        l->intensity = 20.0f;
        l->camera.name = "white spot";

        mat4 m;
        create_look_at(vec3(0.0f, 0.2f, 9.0f), // eye
                       vec3(0.0f, 0.0f, 0.0f), // center
                       vec3(0.0f, 0.0f, 1.0f), // up
                       m);
        l->range = 25.0f;
        l->spot_angle = glm::pi<float>() / 4.0f;
        l->camera.frame.parent_from_local.set(m);
        l->camera.frame.update_hierarchical_no_cache();
        add(l);
    }

    int n_lights = 100;
    for (int i = 0; i < n_lights; ++i)
    {
        float rel   = static_cast<float>(i) / static_cast<float>(n_lights);
        float t     = std::pow(rel, 0.5f);
        float theta = t * 6.0f;
        float R     = 0.5f + 20.0f * t;
        float h     = fract(theta) * 360.0f;
        float s     = 0.9f;
        float v     = 1.0f;
        float r, g, b;

        hsv_to_rgb(h, s, v, r, g, b);

        auto l = make_shared<Light>();
        l->type = Light::Type::spot;
        l->color = vec3(r, g, b);
        l->intensity = 20.0f;
        l->camera.name = "spot";

        float x_pos = R * sin(t * 6.0f * 2.0f * glm::pi<float>());
        float z_pos = R * cos(t * 6.0f * 2.0f * glm::pi<float>());

        mat4 m;
        create_look_at(vec3(x_pos, 10.0f, z_pos),             // eye
                       vec3(x_pos * 0.5, 0.0f, z_pos * 0.5f), // center
                       vec3(0.0f, 0.0f, 1.0f),                // up
                       m);
        l->range = 25.0f;
        l->spot_angle = glm::pi<float>() / 5.0f;
        l->camera.frame.parent_from_local.set(m);
        l->camera.frame.update_hierarchical_no_cache();
        add(l);
    }

    m_format_info.normal_style = Normal_style::polygon_normals;

    auto floor_m = make_model("floor",
                              nullptr,
                              make_geometry_mesh(floor_g),
                              material_floor,
                              vec3(0, -0.5f, 0.0f));
    m_models.push_back(floor_m);

#if 0
   {
      reset_build_info();
      m_format_info.set_want_fill_triangles(true);
      m_format_info.set_want_position(true);
      m_format_info.set_want_normal(true);
      m_format_info.set_want_tangent(true);
      m_format_info.set_want_color(true);
      m_format_info.set_want_texcoord(true);
      m_format_info.set_want_id(true);
      m_format_info.set_normal_style(Normal_style::corner_normals);
      m_format_info.set_mappings(m_programs->mappings);

      m_manipulator_frame = make_shared<Frame>();
      m_format_info.set_constant_color(vec4(1.0f, 0.0f, 0.0f, 1.0f));
      mat4 x_to_y;
      mat4 x_to_z;
         
      auto x_tip_g  = make_shared<renderstack::geometry::shapes::cone      >(0.75f, 1.00f, 0.15f, true, 18, 2);
      auto x_tail_g = make_shared<renderstack::geometry::shapes::cylinder  >(0.00f, 0.75f, 0.03f, true, false, 12, 2);
      auto x_tip_m  = make_model(m_manipulator_frame, x_tip_g);
      auto x_tail_m = make_model(m_manipulator_frame, x_tail_g);
      m_manipulator_models = make_shared<vector<shared_ptr<Model> > >();
      m_manipulator_models->push_back(x_tip_m);
      m_manipulator_models->push_back(x_tail_m);
   }
#endif
}

void Scene_manager::initialize_cameras()
{
    m_camera = make_shared<Camera>();
    m_camera->projection.fov_y = degrees_to_radians(50.0f);
    m_camera->projection.projection_type = Projection::Type::perspective_vertical;
    m_camera->projection.near = 0.02f;
    m_camera->projection.far = 100.0f;

    glm::mat4 m;
    create_look_at(glm::vec3(0.0f, 4.0f, 15.0f),
                   glm::vec3(0.0f, 0.0f, 0.0f),
                   glm::vec3(0.0f, 1.0f, 0.0f),
                   m);
    m_camera->frame.parent_from_local.set(m);

    //m_camera_controls->s
}

shared_ptr<Model> Scene_manager::add(shared_ptr<Model> m)
{
    if (!m)
    {
        throw runtime_error("Scene_manager::add() no model to add");
    }

    m_models.push_back(m);

    return m;
}

shared_ptr<Light> Scene_manager::add(shared_ptr<Light> l)
{
    if (!l)
        throw runtime_error("Scene_manager::add() no light to add");

    m_lights.push_back(l);

    return l;
}

vector<shared_ptr<Model>> &Scene_manager::models()
{
    return m_models;
}

vector<shared_ptr<Model>> const &Scene_manager::models() const
{
    return m_models;
}

vector<shared_ptr<Light>> &Scene_manager::lights()
{
    return m_lights;
}

vector<shared_ptr<Light>> const &Scene_manager::lights() const
{
    return m_lights;
}
