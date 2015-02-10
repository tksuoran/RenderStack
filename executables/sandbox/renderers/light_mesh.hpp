#ifndef light_mesh_hpp_renderers
#define light_mesh_hpp_renderers

#include "renderstack_toolkit/platform.hpp"
#include "renderstack_toolkit/service.hpp"
#include <glm/glm.hpp>
#include <map>
#include <memory>

namespace renderstack {
   namespace graphics {
      class renderer;
   }
   namespace mesh {
      class geometry_mesh;
   }
   namespace scene {
      class light;
   }
}

class programs;

class light_mesh : public renderstack::toolkit::service
{
public:
   light_mesh();
   /*virtual*/ ~light_mesh();

   void connect(
      std::shared_ptr<renderstack::graphics::renderer>   renderer,
      std::shared_ptr<class programs>                    programs
   );

   /*virtual/*/ void initialize_service();

   glm::mat4 get_light_transform(
      std::shared_ptr<renderstack::scene::light> l
   );

   bool point_in_light(
      glm::vec3 point_in_world,
      std::shared_ptr<renderstack::scene::light> l
   );

   std::shared_ptr<renderstack::mesh::geometry_mesh> get_light_mesh(
      std::shared_ptr<renderstack::scene::light> l
   );

private:
   void update_light_model(std::shared_ptr<renderstack::scene::light> l);

private:
   std::shared_ptr<renderstack::graphics::renderer>   m_renderer;
   std::shared_ptr<programs>                          m_programs;

   std::shared_ptr<renderstack::mesh::geometry_mesh>  m_quad_mesh;
   std::shared_ptr<renderstack::mesh::geometry_mesh>  m_cone_mesh;

   int                                                m_light_cone_sides;
};


#endif
