#ifndef shader_monitor_hpp_renderstack_graphics
#define shader_monitor_hpp_renderstack_graphics

#include "renderstack_toolkit/platform.hpp"
#include "renderstack_toolkit/service.hpp"
#include <map>
#include <memory>
#include <string>
#include <sys/stat.h>

namespace renderstack
{
namespace graphics
{

class Program;

class Shader_monitor : public renderstack::toolkit::service
{
public:
    Shader_monitor();

    virtual ~Shader_monitor() = default;

    void initialize_service() override;

    void add(const std::string &path, Program *program);

    void remove(Program *program);

    void poll();

    void set_src_path(const std::string &src_path);

    void set_dst_path(const std::string &dst_path);

    std::string most_recent_version(const std::string &path);

private:
    struct File
    {
        time_t             src_last_time;
        time_t             dst_last_time;
        std::string        src_path;
        std::string        dst_path;
        std::set<Program*> programs;
    };

    std::map<std::string, File> m_files;
    std::string                 m_src_path;
    std::string                 m_dst_path;
};

} // namespace graphics
} // namespace renderstack

#endif
