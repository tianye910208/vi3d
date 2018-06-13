#include "vi_gles.h"
#include "vi_log.h"


#ifdef VI3D_GLES_EGL

static EGLDisplay eglDisplay = EGL_NO_DISPLAY;
static EGLContext eglContext = EGL_NO_CONTEXT;
static EGLSurface eglSurface = EGL_NO_SURFACE;

int vi_gles_egl_init(EGLNativeDisplayType display, EGLNativeWindowType  window) {
	EGLConfig config = NULL;
	
	if (eglDisplay == EGL_NO_DISPLAY) {
		eglDisplay = eglGetDisplay(display);

		if (eglDisplay == EGL_NO_DISPLAY || eglGetError() != EGL_SUCCESS)
			return 1;

		EGLint majorVersion, minorVersion;
		if (!eglInitialize(eglDisplay, &majorVersion, &minorVersion) || eglGetError() != EGL_SUCCESS)
			return 2;
		
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
			return 3;
	}

	if (eglContext == EGL_NO_CONTEXT) {
		EGLint ctxAttribList[] = {
			EGL_CONTEXT_CLIENT_VERSION, 2,
			EGL_NONE
		};
		eglContext = eglCreateContext(eglDisplay, config, EGL_NO_CONTEXT, ctxAttribList);
		if (eglContext == EGL_NO_CONTEXT || eglGetError() != EGL_SUCCESS) {
			vi_log("[E]eglCreateContext %d", eglGetError());
			return 4;
		}
	}

	eglSurface = eglCreateWindowSurface(eglDisplay, config, window, NULL);
	if (eglSurface == EGL_NO_SURFACE || eglGetError() != EGL_SUCCESS) {
		vi_log("[E]eglCreateWindowSurface %d", eglGetError());
		return 5;
	}

	if (!eglMakeCurrent(eglDisplay, eglSurface, eglSurface, eglContext) || eglGetError() != EGL_SUCCESS) {
		vi_log("[E]eglMakeCurrent %d", eglGetError());
		return 6;
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

#endif





#ifdef VI3D_GLES_WGL

HWND hWnd;
HDC hDC;
HGLRC hRC;
int vi_gles_wgl_init(void* _display, HWND hwnd) {
	int err = wglewInit();
	if (GLEW_OK != err) {
		vi_log("[E]wglewInit %s", glewGetErrorString(err));
	}
	if (!WGLEW_ARB_create_context || !WGLEW_ARB_pixel_format) {
		vi_log("[E]wglewInit %p %p", WGLEW_ARB_create_context, WGLEW_ARB_pixel_format);
		return 1;
	}

	hWnd = hwnd;
	hDC = GetDC(hwnd);
	
	const int iPixelFormatAttributeList[] = {
		WGL_DRAW_TO_WINDOW_ARB, GL_TRUE,                        // 绘制到窗口
		WGL_SUPPORT_OPENGL_ARB, GL_TRUE,                        // 支持OpenGL
		WGL_ACCELERATION_ARB, WGL_FULL_ACCELERATION_ARB,        // 硬件加速
		WGL_DOUBLE_BUFFER_ARB, GL_TRUE,                         // 双缓冲
		WGL_PIXEL_TYPE_ARB, WGL_TYPE_RGBA_ARB,                  // RGBA
		WGL_COLOR_BITS_ARB, 32,                                 // 颜色位数32
		WGL_DEPTH_BITS_ARB, 24,                                 // 深度位数24
		WGL_STENCIL_BITS_ARB, 8,                                // 模板位数8
		WGL_SWAP_METHOD_ARB, WGL_SWAP_EXCHANGE_ARB,             // 双缓冲swap方式直接交换
		WGL_SAMPLES_ARB, 4,										// 4倍抗锯齿
		0
	};

	const int iContextAttributeList[] = {
		WGL_CONTEXT_MAJOR_VERSION_ARB, 3,                        // 主版本号
		WGL_CONTEXT_MINOR_VERSION_ARB, 3,                        // 次版本号
		WGL_CONTEXT_FLAGS_ARB, WGL_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB,
		0
	};

	int iPixelFormat, iNumFormat;
	wglChoosePixelFormatARB(hDC, iPixelFormatAttributeList, NULL, 1, &iPixelFormat, (UINT *)&iNumFormat);

	PIXELFORMATDESCRIPTOR pfd;
	if (!SetPixelFormat(hDC, iPixelFormat, &pfd)) {
		vi_log("[E]SetPixelFormat %d", GetLastError());
		return 2;
	}

	hRC = wglCreateContextAttribsARB(hDC, NULL, iContextAttributeList);
	if (!hRC) {
		vi_log("[E]wglCreateContextAttribsARB %d", GetLastError());
		return 3;
	}
	
	wglMakeCurrent(hDC, hRC);
	return 0;
}

int vi_gles_wgl_exit() {
	wglMakeCurrent(hDC, NULL);
	wglDeleteContext(hRC);
	ReleaseDC(hWnd, hDC);
	return 0;
}

int vi_gles_wgl_swap() {
	return wglSwapLayerBuffers(hDC, WGL_SWAP_MAIN_PLANE);
}


#endif


