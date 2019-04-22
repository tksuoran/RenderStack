#ifndef platform_hpp_renderstack_toolkit
#    define platform_hpp_renderstack_toolkit

#    if defined(_WIN32)

// Uncomment this to speed up iterator debugging
// But you will often lose capability to show std::string in debugger :/
// http://connect.microsoft.com/VisualStudio/feedback/details/721456
//# define _HAS_ITERATOR_DEBUGGING 0

#        define _CRT_SECURE_NO_WARNINGS
#        define WIN32_LEAN_AND_MEAN
#        define VC_EXTRALEAN
#        define STRICT
#        define NOGDICAPMASKS     // CC_*, LC_*, PC_*, CP_*, TC_*, RC_
#        define NOVIRTUALKEYCODES // VK_*
//# define NOWINMESSAGES     // WM_*, EM_*, LB_*, CB_*
#        define NOWINSTYLES  // WS_*, CS_*, ES_*, LBS_*, SBS_*, CBS_*
#        define NOSYSMETRICS // SM_*
#        define NOMENUS      // MF_*
//# define NOICONS           // IDI_*
#        define NOKEYSTATES   // MK_*
#        define NOSYSCOMMANDS // SC_*
#        define NORASTEROPS   // Binary and Tertiary raster ops
#        define NOSHOWWINDOW  // SW_*
#        define OEMRESOURCE   // OEM Resource values
#        define NOATOM        // Atom Manager routines
#        define NOCLIPBOARD   // Clipboard routines
#        define NOCOLOR       // Screen colors
#        define NOCTLMGR      // Control and Dialog routines
#        define NODRAWTEXT    // DrawText() and DT_*
//# define NOGDI             // All GDI defines and routines
#        define NOKERNEL // All KERNEL defines and routines
//# define NOUSER            // All USER defines and routines
#        define NONLS            // All NLS defines and routines
#        define NOMB             // MB_* and MessageBox()
#        define NOMEMMGR         // GMEM_*, LMEM_*, GHND, LHND, associated routines
#        define NOMETAFILE       // typedef METAFILEPICT
#        define NOMINMAX         // Macros min(a,b) and max(a,b)
#        define NOMSG            // typedef MSG and associated routines
#        define NOOPENFILE       // OpenFile(), OemToAnsi, AnsiToOem, and OF_*
#        define NOSCROLL         // SB_* and scrolling routines
#        define NOSERVICE        // All Service Controller routines, SERVICE_ equates, etc.
#        define NOSOUND          // Sound driver routines
#        define NOTEXTMETRIC     // typedef TEXTMETRIC and associated routines
#        define NOWH             // SetWindowsHook and WH_*
#        define NOWINOFFSETS     // GWL_*, GCL_*, associated routines
#        define NOCOMM           // COMM driver routines
#        define NOKANJI          // Kanji support stuff.
#        define NOHELP           // Help engine interface.
#        define NOPROFILER       // Profiler interface.
#        define NODEFERWINDOWPOS // DeferWindowPos routines
#        define NOMCX            // Modem Configuration Extensions
#        define NOMINMAX         // Macros min(a,b) and max(a,b)
//# include <windows.h>
#        define _CRTDBG_MAP_ALLOC
#        include <crtdbg.h>
#        include <stdlib.h>

#        if _MSC_VER < 1900
#            define vsnprintf _vsnprintf
#        endif

#        ifdef _DEBUG
#            ifndef DBG_NEW
#                define DBG_NEW new (_NORMAL_BLOCK, __FILE__, __LINE__)
#                define new DBG_NEW
#            endif
#        endif // _DEBUG

#    endif

extern void init_memory_system();
extern void check_memory_system();
extern void begin_memory_compare();
extern void end_memory_compare();

#endif
