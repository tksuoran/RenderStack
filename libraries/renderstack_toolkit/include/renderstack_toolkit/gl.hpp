#ifndef gl_hpp_renderstack_toolkit
#define gl_hpp_renderstack_toolkit

#if defined(_WIN32)
#   define _CRT_SECURE_NO_WARNINGS
#   define WIN32_LEAN_AND_MEAN
#   define VC_EXTRALEAN
#   define STRICT
#   define NOGDICAPMASKS     // CC_*, LC_*, PC_*, CP_*, TC_*, RC_
#   define NOVIRTUALKEYCODES // VK_*
#   define NOWINMESSAGES     // WM_*, EM_*, LB_*, CB_*
#   define NOWINSTYLES       // WS_*, CS_*, ES_*, LBS_*, SBS_*, CBS_*
#   define NOSYSMETRICS      // SM_*
#   define NOMENUS           // MF_*
#   define NOICONS           // IDI_*
#   define NOKEYSTATES       // MK_*
#   define NOSYSCOMMANDS     // SC_*
#   define NORASTEROPS       // Binary and Tertiary raster ops
#   define NOSHOWWINDOW      // SW_*
#   define OEMRESOURCE       // OEM Resource values
#   define NOATOM            // Atom Manager routines
#   define NOCLIPBOARD       // Clipboard routines
#   define NOCOLOR           // Screen colors
#   define NOCTLMGR          // Control and Dialog routines
#   define NODRAWTEXT        // DrawText() and DT_*
#   define NOGDI             // All GDI defines and routines
#   define NOKERNEL          // All KERNEL defines and routines
#   define NOUSER            // All USER defines and routines
#   define NONLS             // All NLS defines and routines
#   define NOMB              // MB_* and MessageBox()
#   define NOMEMMGR          // GMEM_*, LMEM_*, GHND, LHND, associated routines
#   define NOMETAFILE        // typedef METAFILEPICT
#   define NOMINMAX          // Macros min(a,b) and max(a,b)
#   define NOMSG             // typedef MSG and associated routines
#   define NOOPENFILE        // OpenFile(), OemToAnsi, AnsiToOem, and OF_*
#   define NOSCROLL          // SB_* and scrolling routines
#   define NOSERVICE         // All Service Controller routines, SERVICE_ equates, etc.
#   define NOSOUND           // Sound driver routines
#   define NOTEXTMETRIC      // typedef TEXTMETRIC and associated routines
#   define NOWH              // SetWindowsHook and WH_*
#   define NOWINOFFSETS      // GWL_*, GCL_*, associated routines
#   define NOCOMM            // COMM driver routines
#   define NOKANJI           // Kanji support stuff.
#   define NOHELP            // Help engine interface.
#   define NOPROFILER        // Profiler interface.
#   define NODEFERWINDOWPOS  // DeferWindowPos routines
#   define NOMCX             // Modem Configuration Extensions
#   define NOMINMAX          // Macros min(a,b) and max(a,b)
#   include <windows.h>
#endif

#if defined(RENDERSTACK_GL_API_OPENGL_ES_3)
//# include "renderstack_toolkit/dynamic_opengl_es3.hpp"
# include "GLES3/gl3.h"
#endif

#if defined(RENDERSTACK_GL_API_OPENGL_ES_2)
//# include "renderstack_toolkit/dynamic_opengl_es2.hpp"
# include "GLES2/gl2.h"
# include "GLES2/gl2ext.h"
#endif

#if defined(RENDERSTACK_GL_API_OPENGL)
# define GL_GLEXT_PROTOTYPES 1
# include <GL/glcorearb.h>
# include "renderstack_toolkit/glext.hpp"
#endif

#include "gl_core.hpp"

//#include <GL/glfw3.h>
//#include "gles2.hpp"
//#include "GL/dynamic_opengl_es_2.h"

#endif
