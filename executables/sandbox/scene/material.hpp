#ifndef material_hpp_scene
#define material_hpp_scene

#include "renderstack_graphics/program.hpp"
#include "renderstack_graphics/renderer.hpp"
#include "renderstack_graphics/uniform_block.hpp"
#include "renderstack_graphics/uniform_buffer_range.hpp"
#include "renderstack_toolkit/platform.hpp"

#include <map>
#include <memory>
#include <string>

struct Material
{
    Material(
        size_t        index,
        const std::string &name,
        glm::vec4 const &  color,
        float              roughness,
        float              isotropy)
    : index(index), name(name), color(color), roughness(roughness), isotropy(isotropy)
    {
    }

    size_t index;
    std::string name;
    glm::vec4   color;
    float       roughness;
    float       isotropy;
};

#endif
