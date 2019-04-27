#ifndef textures_hpp
#define textures_hpp

#include "renderstack_graphics/renderer.hpp"
#include "renderstack_graphics/texture.hpp"
#include "renderstack_toolkit/platform.hpp"
#include "renderstack_toolkit/service.hpp"
#include <memory>
#include <string>

class textures : public renderstack::toolkit::service
{
public:
    textures();

    virtual ~textures();

    void connect(std::shared_ptr<renderstack::graphics::Renderer> renderer);

    void initialize_service() override;

    std::shared_ptr<renderstack::graphics::Texture> load(unsigned int texture_unit, const std::string &path);

    std::shared_ptr<renderstack::graphics::Texture> background_texture;

private:
    std::shared_ptr<renderstack::graphics::Renderer> m_renderer;
};

#endif
