#ifndef _VI3D_GLES_
#define _VI3D_GLES_

#include "vi_sys.h"

//#define VI3D_GLES_EGL

#ifdef VI3D_SYS_ANDROID
#define VI3D_GLES_EGL
#endif

#ifdef VI3D_SYS_IOS
#undef  VI3D_GLES_EGL
#define VI3D_GLES_AGL
#endif

#ifdef VI3D_SYS_WIN
#ifndef VI3D_GLES_EGL
#define VI3D_GLES_WGL
#endif
#endif

#ifdef VI3D_SYS_LINUX
#ifndef VI3D_GLES_EGL
#define VI3D_GLES_GLX
#endif
#endif





#ifdef VI3D_GLES_EGL
#include <GLES2/gl2.h>
#include <EGL/egl.h>
#include <EGL/eglext.h>

#define vi_gles_init(x, y) vi_gles_egl_init((EGLNativeDisplayType)x, (EGLNativeWindowType)y)
#define vi_gles_exit vi_gles_egl_exit
#define vi_gles_swap vi_gles_egl_swap

int vi_gles_egl_init(EGLNativeDisplayType display, EGLNativeWindowType window);
int vi_gles_egl_exit();
int vi_gles_egl_swap();

#endif


#ifdef VI3D_GLES_AGL
#include <OpenGLES/ES2/gl.h>
#include <OpenGLES/ES2/glext.h>

#define vi_gles_init(x, y)  0
#define vi_gles_exit()      0
#define vi_gles_swap()      0


#endif//VI3D_GLES_AGL

#ifdef VI3D_GLES_WGL
#define GLEW_STATIC
#include <GL/glew.h>
#include <GL/wglew.h>

#define vi_gles_init(x, y) vi_gles_wgl_init(NULL, (HWND)y)
#define vi_gles_exit vi_gles_wgl_exit
#define vi_gles_swap vi_gles_wgl_swap

int vi_gles_wgl_init(void* _display, HWND hwnd);
int vi_gles_wgl_exit();
int vi_gles_wgl_swap();

#endif//VI3D_GLES_WGL


#ifdef VI3D_GLES_GLX
#define GLEW_STATIC
#include <GL/glew.h>
#include <GL/glxew.h>

#define vi_gles_init(x, y) vi_gles_glx_init((Display*)x, (Window)y)
#define vi_gles_exit vi_gles_glx_exit
#define vi_gles_swap vi_gles_glx_swap

int vi_gles_glx_init(Display* display, Window window);
int vi_gles_glx_exit();
int vi_gles_glx_swap();

#endif//VI3D_GLES_GLX


#endif 


