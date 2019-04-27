#ifndef bounding_box_hpp_util
#define bounding_box_hpp_util

#include "renderstack_toolkit/platform.hpp"
#include <algorithm>
#include <glm/glm.hpp>
#include <limits>

struct bounding_box
{
    glm::vec3 min;
    glm::vec3 max;

    inline glm::vec3 size()
    {
        return max - min;
    }

    inline glm::vec3 half_size()
    {
        return size() / 2.0f;
    }

    inline glm::vec3 center()
    {
        return min + half_size();
    }

    void clear()
    {
        min = glm::vec3(std::numeric_limits<float>::max(), std::numeric_limits<float>::max(), std::numeric_limits<float>::max());
        max = glm::vec3(std::numeric_limits<float>::min(), std::numeric_limits<float>::min(), std::numeric_limits<float>::min());
    }

    inline void extend_by(glm::vec3 const &p)
    {
        min.x = (std::min)(p.x, min.x);
        max.x = (std::max)(p.x, max.x);
        min.y = (std::min)(p.y, min.y);
        max.y = (std::max)(p.y, max.y);
        min.z = (std::min)(p.z, min.z);
        max.z = (std::max)(p.z, max.z);
    }

    void extend_by(float x, float y, float z)
    {
        min.x = (std::min)(x, min.x);
        max.x = (std::max)(x, max.x);
        min.y = (std::min)(y, min.y);
        max.y = (std::max)(y, max.y);
        min.z = (std::min)(z, min.z);
        max.z = (std::max)(z, max.z);
    }
};

#endif
