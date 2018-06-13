#ifndef _VI3D_GLES_
#define _VI3D_GLES_

#include "vi_sys.h"


#ifdef VI3D_SYS_WIN
#include <GLES2/gl2.h>
#include <EGL/egl.h>
#include <EGL/eglext.h>
#endif

#ifdef VI3D_SYS_IOS
#include <OpenGLES/ES2/gl.h>
#include <OpenGLES/ES2/glext.h>
#endif

#ifdef VI3D_SYS_LINUX
#include <GLES2/gl2.h>
#include <EGL/egl.h>
#include <EGL/eglext.h>
#endif

#ifdef VI3D_SYS_ANDROID
#include <GLES2/gl2.h>
#include <EGL/egl.h>
#include <EGL/eglext.h>
#endif


int vi_gles_egl_init(EGLNativeDisplayType display, EGLNativeWindowType window);
int vi_gles_egl_exit();
int vi_gles_egl_swap();

#endif 


