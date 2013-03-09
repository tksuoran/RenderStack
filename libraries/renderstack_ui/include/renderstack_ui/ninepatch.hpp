#ifndef renderstack_ui__ninepatch_hpp
#define renderstack_ui__ninepatch_hpp

#include "renderstack_toolkit/platform.hpp"
#include "renderstack_mesh/mesh.hpp"
#include "renderstack_graphics/vertex_format.hpp"
#include "renderstack_graphics/vertex_stream.hpp"
#include <glm/glm.hpp>
#include <string>
#include <stdexcept>
#include <memory>

namespace renderstack { namespace ui {

class ninepatch_style;

class context_ninepatch;

class ninepatch
{
private:
   std::shared_ptr<ninepatch_style>                      m_style;
   std::shared_ptr<renderstack::graphics::vertex_stream> m_vertex_stream;
   renderstack::mesh::mesh                               m_mesh;
   glm::vec2                                             m_size;

public:
   std::shared_ptr<ninepatch_style> style() const { return m_style; }
   renderstack::mesh::mesh const    &mesh() const { return m_mesh; }
   glm::vec2 const                  &size() const { return m_size; }

public:
   ninepatch(std::shared_ptr<ninepatch_style> style);

   void place(float x0, float y0, float width, float height);
   void render();

private:
   void prepare_gl_resources();
};

class context_ninepatch
{
private:
   std::shared_ptr<renderstack::graphics::vertex_format>   s_vertex_format;

public:
   std::shared_ptr<renderstack::graphics::vertex_format>   vertex_format();
};

} }

#endif
