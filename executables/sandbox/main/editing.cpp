#include "renderstack_toolkit/platform.hpp"
#include "main/game.hpp"
#include "voxel/voxel_renderer.hpp"
#include "voxel/morton.hpp"
#include <unordered_set>

#define LOG_CATEGORY &log_game

using namespace glm;

void game::begin_patch()
{
    slog_trace("game::begin_patch()");

    assert(m_voxel_renderer);
    assert(m_map);

    //cout << "---------------------------------------------\n";
    m_voxel_renderer->begin_patch();
    m_patch_list.clear();
}

void game::end_patch()
{
    slog_trace("game::end_patch()");

    assert(m_voxel_renderer);
    assert(m_map);

    for (auto i = m_patch_list.begin(); i != m_patch_list.end(); ++i)
    {
        coord_t x, y, z;
        morton_t morton = *i;
        morton_decode(morton, x, y, z);
        m_voxel_renderer->patch(x, y, z, *m_map);
    }
    m_patch_list.clear();
    m_voxel_renderer->end_patch(*m_map);
    //cout << ".\n";
}

void game::patch(glm::ivec3 const &pos, blockid_t brush)
{
    coord_t x = static_cast<coord_t>(pos.x);
    coord_t y = static_cast<coord_t>(pos.y);
    coord_t z = static_cast<coord_t>(pos.z);

    slog_trace("game::patch(%d, %d, %d, %u)", x, y, z, brush);

    assert(m_voxel_renderer);
    assert(m_map);

    m_map->put(x, y, z, brush);
    m_map->put_light(x, y, z, 0xf0);
    m_patch_list.insert(morton_encode(x, y, z));

    if (x > 0                  ) m_patch_list.insert(morton_encode(x - 1, y, z));
    if (x < m_map->size_x() - 1) m_patch_list.insert(morton_encode(x + 1, y, z));
    if (y > 0                  ) m_patch_list.insert(morton_encode(x, y - 1, z));
    if (y < m_map->size_y() - 1) m_patch_list.insert(morton_encode(x, y + 1, z));
    if (z > 0                  ) m_patch_list.insert(morton_encode(x, y, z - 1));
    if (z < m_map->size_z() - 1) m_patch_list.insert(morton_encode(x, y, z + 1));
}

void game::patch_inner(coord_t x, coord_t y, coord_t z, blockid_t brush)
{
    slog_trace("game::patch_inner(%d, %d, %d, %u)", x, y, z, brush);

    assert(m_voxel_renderer);
    assert(m_map);

    m_map->put(x, y, z, brush);
    m_map->put_light(x, y, z, 0xf0);
    m_patch_list.insert(morton_encode(x, y, z));
}

void game::paint(glm::ivec3 const &pos, blockid_t brush)
{
    coord_t ox = static_cast<coord_t>(pos.x);
    coord_t oy = static_cast<coord_t>(pos.y);
    coord_t oz = static_cast<coord_t>(pos.z);
    slog_trace("game::paint(%d, %d, %d, %u)", ox, oy, oz, brush);

    assert(m_voxel_renderer);
    assert(m_map);

#if 0
    patch(ox, oy, oz, brush);
#else
    coord_t size = m_controls.brush_size;
    for (coord_t ix = -size; ix <= size; ++ix)
    {
        for (coord_t iy = -size; iy <= size; ++iy)
        {
            for (coord_t iz = -size; iz <= size; ++iz)
            {
                coord_t x = ox + ix;
                coord_t y = oy + iy;
                coord_t z = oz + iz;
                // TODO Fix the unsigned >= 0 comparison
                if (
                    (x >= 0) && (x < m_map->size_x()) &&
                    (y >= 0) && (y < m_map->size_y()) &&
                    (z >= 0) && (z < m_map->size_z())
                )
                {
                    vec3 p(
                        static_cast<float>(ix),
                        static_cast<float>(iy),
                        static_cast<float>(iz)
                    );
                    float l = length(p);
                    if (l > static_cast<float>(size))
                        continue;

                    if (l < static_cast<float>(size) - 1.0f)
                        patch_inner(x, y, z, brush);

                    patch(glm::ivec3(x, y, z), brush);
                }
            }
        }
    }
#endif
}
