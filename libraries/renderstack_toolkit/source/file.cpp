#include "renderstack_toolkit/platform.hpp"
#include "renderstack_toolkit/file.hpp"
#include "renderstack_toolkit/logstream.hpp"

#include <fstream>
#include <sstream>
#include <sys/stat.h>
#include <stdexcept>

namespace renderstack { namespace toolkit {

using namespace std;

string read(string const &fname)
{
    ifstream file(fname, ios::binary);
    struct stat st;
    int res = stat(fname.c_str(), &st);
    if (res != 0)
        throw runtime_error("file not found");

    string result(static_cast<size_t>(st.st_size), 0);
    file.read(&result[0], static_cast<streamsize>(st.st_size));

    return result;
}

} }
