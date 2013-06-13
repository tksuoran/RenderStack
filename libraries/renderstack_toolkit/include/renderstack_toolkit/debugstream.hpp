#ifndef debugstream_hpp_renderstack_toolkit
#define debugstream_hpp_renderstack_toolkit

#include "platform.hpp"
#include <ostream>
#include <sstream>
#include <string>

#if defined(_WIN32)
# define WIN32_LEAN_AND_MEAN
# define STRICT
# include <windows.h>
#endif


//	Custom streambuf class for the debugstream --------------------------------
template<typename E, typename T = std::char_traits<E> >
class basic_debugbuf : public std::basic_stringbuf<E, T>
{
public:
    basic_debugbuf() {	}
    ~basic_debugbuf() {
        sync();
    }

protected:
    int sync() {
        output_debug_string(str().c_str());
        str(std::basic_string<E>());	//	Clear the string buffer.
        return 0;
    }
    void output_debug_string(const E* text) {
    }
};


//	Specialisations for char and wchar_t to call the correct Win32 APIs -------
template<>
void basic_debugbuf<char>::output_debug_string(const char* text) {
#if defined(_WIN32)
    ::OutputDebugStringA(text);
#endif
}

template<>
void basic_debugbuf<wchar_t>::output_debug_string(const wchar_t* text) {
#if defined(_WIN32)
    ::OutputDebugStringW(text);
#endif
}


//	The debugstream -----------------------------------------------------------
template<typename E, typename T = std::char_traits<E> >
class basic_debugstream : public std::basic_ostream<E, T>
{
    typedef basic_debugbuf<E, T>	debugbuf;

public:
    //	Connect the base ostream with the custom streambuf.
    basic_debugstream() : std::basic_ostream<E, T>(new debugbuf())
    {	}
    ~basic_debugstream() {
        delete rdbuf();
    }
};


//	Typedefs for the obvious template instantions -----------------------------
typedef basic_debugstream<char>		debugstream;
typedef basic_debugstream<wchar_t>	debugstreamw;


//	Helper functor for debugging. Useful in conjunction with std::for_each ----
template<typename T>
struct debug_outputter
{
    void operator()(const T& t) {
        debugstream() << t << std::endl;
    }
};

#endif
