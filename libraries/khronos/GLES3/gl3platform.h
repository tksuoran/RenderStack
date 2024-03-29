#ifndef __gl3platform_h_
#define __gl3platform_h_

/* $Revision: 18437 $ on $Date:: 2012-07-08 23:31:39 -0700 #$ */

/*
 * This document is licensed under the SGI Free Software B License Version
 * 2.0. For details, see http://oss.sgi.com/projects/FreeB/ .
 */

/* Platform-specific types and definitions for OpenGL ES 3.X  gl3.h
 *
 * Adopters may modify khrplatform.h and this file to suit their platform.
 * You are encouraged to submit all modifications to the Khronos group so that
 * they can be included in future versions of this file.  Please submit changes
 * by sending them to the public Khronos Bugzilla (http://khronos.org/bugzilla)
 * by filing a bug against product "OpenGL-ES" component "Registry".
 */

#include <KHR/khrplatform.h>

#ifndef GL_APICALL
//#define GL_APICALL  KHRONOS_APICALL
#define GL_APICALL  
#endif

#ifndef GL_APIENTRY
//#define GL_APIENTRY KHRONOS_APIENTRY
#define GL_APIENTRY  
#endif

#endif /* __gl3platform_h_ */
