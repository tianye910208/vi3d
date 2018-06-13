#include "vi_gles.h"
#include "vi_log.h"


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


