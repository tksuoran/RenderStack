#include "renderstack_graphics/shader_monitor.hpp"
#include "renderstack_graphics/program.hpp"
#include "renderstack_toolkit/file.hpp"
#include "renderstack_toolkit/platform.hpp"
#include <cassert>
#include <fstream>
#include <stdexcept>
#include <vector>

namespace renderstack
{
namespace graphics
{

using namespace std;
using namespace renderstack::toolkit;

Shader_monitor::Shader_monitor()
    : service("Shader_monitor")
{
}

void Shader_monitor::initialize_service()
{
    try
    {
        string src_path;
        string dst_path;

        if (exists("res/src_path.txt"))
        {
            src_path = read("res/src_path.txt");
            dst_path = read("res/dst_path.txt");
        }
        else
        {
            src_path = ""; //C:/git/RenderStack/executables/sandbox";
            dst_path = "";
        }

        set_dst_path(dst_path);
        set_src_path(src_path);
    }
    catch (...)
    {
        //log_warn("Programs::initialize_service() setting up shader monitor failed");
    }
}

void Shader_monitor::set_src_path(const std::string &src_path)
{
    m_src_path = src_path;
}

void Shader_monitor::set_dst_path(const std::string &dst_path)
{
    m_dst_path = dst_path;
}

void Shader_monitor::add(const std::string &path, Program *program)
{
    auto i = m_files.find(path);
    if (i == m_files.end())
    {
        File f;
        m_files[path] = f;
    }

    auto &f = m_files[path];

    int         ok = 0;
    struct stat st;
    int         res = ::stat(path.c_str(), &st);
    if (res != 0)
    {
        throw runtime_error("file not found");
    }

    f.src_path = m_src_path + "/" + path;
    f.dst_path = m_dst_path + "/" + path;

    res = stat(f.src_path.c_str(), &st);
    if (res != 0)
    {
        f.src_path.clear();
    }
    else
    {
        f.src_last_time = st.st_mtime;
        ++ok;
    }

    res = ::stat(f.dst_path.c_str(), &st);
    if (res != 0)
    {
        f.dst_path.clear();
    }
    else
    {
        f.dst_last_time = st.st_mtime;
        ++ok;
    }

    assert(ok > 0); // TODO
    if (ok)
    {
        f.programs.insert(program);
    }
}

string Shader_monitor::most_recent_version(const std::string &path)
{
    auto i = m_files.find(path);
    if (i != m_files.end())
    {
        auto        &f = i->second;
        struct stat src_st;
        struct stat dst_st;
        int         src_res = stat(f.src_path.c_str(), &src_st);
        int         dst_res = stat(f.dst_path.c_str(), &dst_st);
        if (src_res == 0 && dst_res == 0)
        {
            if (src_st.st_mtime >= dst_st.st_mtime)
            {
                return f.src_path;
            }
            else
            {
                return f.dst_path;
            }
        }
        else if (src_res != 0 && dst_res == 0)
        {
            return f.dst_path;
        }
        else if (src_res == 0 && dst_res != 0)
        {
            return f.src_path;
        }
    }
    return "";
}

void Shader_monitor::poll()
{
    for (auto &i : m_files)
    {
        auto &f = i.second;
        struct stat st;
        int         res = stat(f.src_path.c_str(), &st);
        if (res == 0)
        {
            if (f.src_last_time != st.st_mtime)
            {
                for (auto program : f.programs)
                {
                    program->reload(*this);
                }

                f.src_last_time = st.st_mtime;
                continue;
            }
        }
        res = stat(f.dst_path.c_str(), &st);
        if (res == 0)
        {
            if (f.dst_last_time != st.st_mtime)
            {
                for (auto program : f.programs)
                {
                    program->reload(*this);
                }

                f.dst_last_time = st.st_mtime;
                continue;
            }
        }
    }
}

} // namespace graphics
} // namespace renderstack
