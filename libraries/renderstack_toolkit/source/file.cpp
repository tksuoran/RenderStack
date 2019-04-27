#include "renderstack_toolkit/file.hpp"
#include "renderstack_toolkit/log.hpp"
#include "renderstack_toolkit/platform.hpp"

#include <fstream>
#include <sstream>
#include <stdexcept>
#include <sys/stat.h>

#include <cstdio>
#ifdef WIN32
#    include <direct.h>
#    define get_cwd _getcwd
#else
#    include <unistd.h>
#    define get_cwd getcwd
#endif

namespace renderstack
{
namespace toolkit
{

#define LOG_CATEGORY &log_file

using namespace std;

bool exists(const std::string &fname)
{
    ifstream    file(fname, ios::binary);
    struct stat st;
    int         res = stat(fname.c_str(), &st);
    return res == 0;
}

string read(const std::string &fname)
{
    ifstream    file(fname, ios::binary);
    struct stat st;
    int         res = stat(fname.c_str(), &st);
    if (res != 0)
    {
        stringstream ss;
        ss << "file not found: " << fname;
        log_error(ss.str().c_str());
        throw runtime_error(ss.str().c_str());
    }

    string result(static_cast<size_t>(st.st_size), 0);
    file.read(&result[0], static_cast<streamsize>(st.st_size));

    return result;
}

string get_current_working_directory()
{
    char path[FILENAME_MAX];

    if (!get_cwd(path, sizeof(path)))
        return string("");

    path[sizeof(path) - 1] = '\0';

    return string(path);
}

} // namespace toolkit
} // namespace renderstack
