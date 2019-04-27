#include "renderers/light_mesh.hpp"
#include "main/programs.hpp"
#include "renderstack_geometry/shapes/cone.hpp"
#include "renderstack_geometry/shapes/triangle.hpp" // quad is currently here...
#include "renderstack_graphics/vertex_format.hpp"
#include "renderstack_mesh/geometry_mesh.hpp"
#include "renderstack_mesh/mesh.hpp"
#include "renderstack_scene/light.hpp"
#include "renderstack_toolkit/math_util.hpp"
#include "renderstack_toolkit/platform.hpp"
#include <glm/glm.hpp>

using namespace renderstack::toolkit;
using namespace renderstack::mesh;
using namespace renderstack::scene;
using namespace renderstack;
using namespace glm;
using namespace std;

Light_mesh::Light_mesh()
    : service("Light_mesh")
{
}

void Light_mesh::connect(
    shared_ptr<renderstack::graphics::Renderer> renderer,
    shared_ptr<Programs>                  programs)
{
    m_renderer = renderer;
    m_programs = programs;

    initialization_depends_on(renderer);
    initialization_depends_on(programs);
}

void Light_mesh::initialize_service()
{
    Geometry_mesh::Format_info format_info;

    format_info.want_fill_triangles = true;
    format_info.want_edge_lines = true;
    format_info.want_position = true;
    format_info.vertex_attribute_mappings = &m_programs->attribute_mappings;

    // Full screen quad
    {
        // -1 .. 1
        auto quad_geometry = renderstack::geometry::shapes::make_quad(2.0f);
        quad_geometry.build_edges();
        Geometry_mesh::Buffer_info buffer_info;
        Geometry_mesh::prepare_vertex_format(quad_geometry, format_info, buffer_info);
        auto &r = *m_renderer;

        m_quad_mesh = make_shared<Geometry_mesh>(r, quad_geometry, format_info, buffer_info);
    }

    // Spot light cone
    {
        m_light_cone_sides = 11;

        auto cone_geometry = renderstack::geometry::shapes::make_conical_frustum(
            0.0,                // min x
            1.0,                // max x
            0.0,                // bottom radius
            1.0,                // top radius
            false,              // use bottom
            true,               // use top (end)
            m_light_cone_sides, // slice count
            0                   // stack division
        );

        cone_geometry.transform(mat4_rotate_xz_cw);
        cone_geometry.build_edges();
        Geometry_mesh::Buffer_info buffer_info;
        Geometry_mesh::prepare_vertex_format(cone_geometry, format_info, buffer_info);
        auto &r = *m_renderer;

        auto gm = make_shared<Geometry_mesh>(r,
                                             cone_geometry,
                                             format_info,
                                             buffer_info);

        m_cone_mesh = gm;
    }
}

glm::mat4 Light_mesh::get_light_transform(shared_ptr<Light> l)
{
    switch (l->type)
    {
        case Light::Type::directional:
        {
            return mat4(1.0f);
        }

        case Light::Type::point:
        {
            return mat4(1.0f);
        }

        case Light::Type::spot:
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

            float alpha   = l->spot_angle;
            float length  = l->range;
            float apothem = length * std::tan(alpha * 0.5f);
            float radius  = apothem / std::cos(glm::pi<float>() / static_cast<float>(m_light_cone_sides));

            mat4 scale;
            create_scale(radius, radius, length, scale);

            return scale;
        }

        default:
        {
            return mat4(1.0f);
        }
    }
}

bool Light_mesh::point_in_light(vec3 p, shared_ptr<Light> l)
{
    if (l->type != Light::Type::spot)
    {
        return true;
    }

    float spot_angle  = l->spot_angle * 0.5f;
    float outer_angle = spot_angle / std::cos(glm::pi<float>() / static_cast<float>(m_light_cone_sides));
    float spot_cutoff = std::cos(outer_angle);
    float range       = l->range;

    const mat4 &light_from_world = l->camera.frame.world_from_local.inverse_matrix();
    vec3        view_in_light    = vec3(light_from_world * vec4(p, 1.0f));
    float       distance         = -view_in_light.z;
    view_in_light                = normalize(view_in_light);

    float cos_angle          = dot(view_in_light, vec3(0.0f, 0.0f, -1.0f));
    bool  outside_cone_angle = (cos_angle < spot_cutoff);
    bool  outside_cone_range = (distance < 0.0f) || (distance > range);
    if (outside_cone_angle || outside_cone_range)
    {
        return false;
    }
    else
    {
        return true;
    }
}

shared_ptr<Geometry_mesh> Light_mesh::get_light_mesh(shared_ptr<Light> l)
{
    switch (l->type)
    {
        case Light::Type::directional:
        {
            return m_quad_mesh;
        }

        case Light::Type::point:
        {
            return nullptr;
        }

        case Light::Type::spot:
        {
            return m_cone_mesh;
        }

        default:
        {
            return nullptr;
        }
    }
}
