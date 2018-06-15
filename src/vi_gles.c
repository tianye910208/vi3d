#include "vi_gles.h"
#include "vi_log.h"


#ifdef VI3D_GLES_EGL

#ifdef VI3D_SYS_WIN
#pragma comment(lib, "../../3rd/lua-5.3.3/lib_win64/lua.lib")
#pragma comment(lib, "../../3rd/GLES-Mali-v3.0.1-x64/libGLESv2.lib")
#pragma comment(lib, "../../3rd/GLES-Mali-v3.0.1-x64/libEGL.lib")
#endif

static EGLConfig config = NULL;
static EGLDisplay eglDisplay = EGL_NO_DISPLAY;
static EGLContext eglContext = EGL_NO_CONTEXT;
static EGLSurface eglSurface = EGL_NO_SURFACE;

int vi_gles_egl_init(EGLNativeDisplayType display, EGLNativeWindowType  window) {
	
	if (eglDisplay == EGL_NO_DISPLAY) {
		eglDisplay = eglGetDisplay(display);

		if (eglDisplay == EGL_NO_DISPLAY || eglGetError() != EGL_SUCCESS)
			return 101;

		EGLint majorVersion, minorVersion;
		if (!eglInitialize(eglDisplay, &majorVersion, &minorVersion) || eglGetError() != EGL_SUCCESS)
			return 102;
		
		EGLint cfgAttribList[] = {
			EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
			EGL_RED_SIZE, 8,
			EGL_GREEN_SIZE, 8,
			EGL_BLUE_SIZE, 8,
			EGL_ALPHA_SIZE, 8,
			EGL_DEPTH_SIZE, 24,
			EGL_STENCIL_SIZE, 8,
			EGL_SAMPLE_BUFFERS, 0,
			EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT,
			EGL_NONE
		};
		EGLint cfgNum = 0;
		if (!eglChooseConfig(eglDisplay, cfgAttribList, &config, 1, &cfgNum) || cfgNum < 1)
			return 103;
	}

	if (eglContext == EGL_NO_CONTEXT) {
		EGLint ctxAttribList[] = {
			EGL_CONTEXT_CLIENT_VERSION, 2,
			EGL_NONE
		};
		eglContext = eglCreateContext(eglDisplay, config, EGL_NO_CONTEXT, ctxAttribList);
		if (eglContext == EGL_NO_CONTEXT || eglGetError() != EGL_SUCCESS) {
			vi_log("[E]eglCreateContext %d", eglGetError());
			return 104;
		}
	}

	eglSurface = eglCreateWindowSurface(eglDisplay, config, window, NULL);
	if (eglSurface == EGL_NO_SURFACE || eglGetError() != EGL_SUCCESS) {
		vi_log("[E]eglCreateWindowSurface %d", eglGetError());
		return 105;
	}

	if (!eglMakeCurrent(eglDisplay, eglSurface, eglSurface, eglContext) || eglGetError() != EGL_SUCCESS) {
		vi_log("[E]eglMakeCurrent %d", eglGetError());
		return 106;
	}

	return 0;
}



int vi_gles_egl_exit() {
	eglMakeCurrent(eglDisplay, NULL, NULL, NULL);

	eglDestroyContext(eglDisplay, eglContext);
	eglContext = EGL_NO_CONTEXT;

	eglDestroySurface(eglDisplay, eglSurface);
	eglSurface = EGL_NO_SURFACE;

	eglTerminate(eglDisplay);
	eglDisplay = EGL_NO_DISPLAY;
	
	return 0;
}

int vi_gles_egl_swap() {
	eglSwapBuffers(eglDisplay, eglSurface);
	return 0;
}

#endif//VI3D_GLES_EGL


#ifdef VI3D_GLES_WGL
#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "../../3rd/lua-5.3.3/lib_win64/lua.lib")
#pragma comment(lib, "../../3rd/glew-2.1.0/lib/x64/glew32s.lib")


static HWND hWnd;
static HDC hDC;
static HGLRC hRC;

int vi_gles_wgl_init(void* _display, HWND hwnd) {
	hWnd = hwnd;
	hDC = GetDC(hwnd);

	PIXELFORMATDESCRIPTOR pfd = {
		sizeof(PIXELFORMATDESCRIPTOR),//size
		1,//version
		PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,
		PFD_TYPE_RGBA,//color format RGBA
		32, //color depth
		0, 0, 0, 0, 0, 0,  //Color Bits Ignored        
		0, //alpha buffer    
		0, // Shift Bit Ignored   
		0, //Accumulation buffer
		0, 0, 0, 0,// Accumulation Bits Ignored
		24,//depth buffer
		8, //stencil buffer
		0, //assistant buffer
		PFD_MAIN_PLANE,// Main Drawing Layer
		0, //reserved
		0, 0, 0 //Layer Masks Ignored   
	};

	int pixelformat = ChoosePixelFormat(hDC, &pfd);
	if (pixelformat == 0)
		return 101;

	if (!SetPixelFormat(hDC, pixelformat, &pfd))
		return 102;

	hRC = wglCreateContext(hDC);
	if (!hRC)
		return 103;
	if (!wglMakeCurrent(hDC, hRC))
		return 104;

	int err = glewInit();
	if (err != GLEW_OK) {
		vi_log("[E]glewInit %s", glewGetErrorString(err));
		return 105;
	}
	return 0;
}

int vi_gles_wgl_exit() {
	if (hRC)
		wglMakeCurrent(NULL, NULL);
	if (hRC)
		wglDeleteContext(hRC);
	if (hWnd && hDC)
		ReleaseDC(hWnd, hDC);
	return 0;
}

int vi_gles_wgl_swap() {
	return hDC && SwapBuffers(hDC);
}

#endif//VI3D_GLES_WGL


#ifdef VI3D_GLES_GLX


static Display* 	glxDisplay;
static Window   	glxWindow;
static GLXContext 	glxContext;

int vi_gles_glx_init(Display* display, Window window) {
	glxDisplay = display;
	glxWindow = window;

	GLint att[] = { 
	    GLX_DOUBLEBUFFER, 
        GLX_RGBA, 
        GLX_RED_SIZE, 8,
        GLX_GREEN_SIZE, 8,
        GLX_BLUE_SIZE, 8,
        GLX_ALPHA_SIZE, 8,
        GLX_DEPTH_SIZE, 24,
        GLX_STENCIL_SIZE, 8,
        None 
	};

	XVisualInfo* vi = glXChooseVisual(glxDisplay, 0, att);
	if (vi == NULL) 
		return 101;
	
    XSetWindowAttributes swa;
    swa.colormap = XCreateColormap(glxDisplay, RootWindow(glxDisplay, vi->screen), vi->visual, AllocNone);
    if (!XChangeWindowAttributes(glxDisplay, glxWindow, CWColormap, &swa))
        return 102;

	glxContext = glXCreateContext(glxDisplay, vi, NULL, GL_TRUE);
    if (!glxContext)
        return 103;

 	if (!glXMakeCurrent(glxDisplay, glxWindow, glxContext))
        return 104;
 
	int err = glewInit();
	if (err != GLEW_OK) {
		vi_log("[E]glewInit %s", glewGetErrorString(err));
		return 105;
	}

	return 0;
}

int vi_gles_glx_exit() {
	glXMakeCurrent(glxDisplay, None, NULL);
    glXDestroyContext(glxDisplay, glxContext);
	return 0;
}

int vi_gles_glx_swap() {
	glXSwapBuffers(glxDisplay, glxWindow);
	return 0;
}


#endif//VI3D_GLES_GLX















