#include <jni.h>
#include <errno.h>
#include <unistd.h>
#include <pthread.h>

#include <android/log.h>
#include <android/input.h>
#include <android/sensor.h>
#include <android/looper.h>
#include <android/configuration.h>
#include <android/native_window.h>
#include <android/native_activity.h>

#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, "VI3D", __VA_ARGS__))
#define LOGW(...) ((void)__android_log_print(ANDROID_LOG_WARN, "VI3D", __VA_ARGS__))

#include "../../sys.h"
#include "../../test.h"


ANativeActivity* nativeActivity = NULL;
ANativeWindow*	 nativeShowWindow = NULL;
AInputQueue*     nativeInputQueue = NULL;

EGLNativeDisplayType nativeDisplay = EGL_DEFAULT_DISPLAY;
EGLNativeWindowType  nativeWindow = NULL;

EGLConfig  eglConfig;
EGLDisplay eglDisplay;
EGLContext eglContext;
EGLSurface eglSurface;

bool egl_init()
{
	EGLint configNum = 0;
	EGLint majorVersion;
	EGLint minorVersion;
	EGLint ctxAttribList[] =
	{
		EGL_CONTEXT_CLIENT_VERSION, 2,
		EGL_NONE
	};
	EGLint cfgAttribList[] =
	{
		EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
		EGL_RED_SIZE, 5,
		EGL_GREEN_SIZE, 6,
		EGL_BLUE_SIZE, 5,
		EGL_ALPHA_SIZE, EGL_DONT_CARE,
		EGL_DEPTH_SIZE, EGL_DONT_CARE,
		EGL_STENCIL_SIZE, EGL_DONT_CARE,
		EGL_SAMPLE_BUFFERS, 0,
		EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT,
		EGL_NONE
	};


	eglDisplay = eglGetDisplay(nativeDisplay);

	if (eglDisplay == EGL_NO_DISPLAY || eglGetError() != EGL_SUCCESS)
		return false;

	if (!eglInitialize(eglDisplay, &majorVersion, &minorVersion) || eglGetError() != EGL_SUCCESS)
		return false;

	if (!eglChooseConfig(eglDisplay, cfgAttribList, &eglConfig, 1, &configNum) || configNum < 1)
		return false;

	eglContext = eglCreateContext(eglDisplay, eglConfig, EGL_NO_CONTEXT, ctxAttribList);
	if (eglContext == EGL_NO_CONTEXT || eglGetError() != EGL_SUCCESS)
		return false;

	eglSurface = eglCreateWindowSurface(eglDisplay, eglConfig, nativeWindow, NULL);
	if (eglSurface == EGL_NO_SURFACE || eglGetError() != EGL_SUCCESS)
		return false;

	if (!eglMakeCurrent(eglDisplay, eglSurface, eglSurface, eglContext) || eglGetError() != EGL_SUCCESS)
		return false;

	return true;
}

void egl_exit()
{
	eglMakeCurrent(eglDisplay, NULL, NULL, NULL);
	eglDestroySurface(eglDisplay, eglSurface);
	eglDestroyContext(eglDisplay, eglContext);
	eglTerminate(eglDisplay);
}





bool win_init()
{
	while (nativeShowWindow == NULL)
		usleep(10000);
	nativeWindow = nativeShowWindow;
	return true;
}

void win_exit()
{
	//when exit
}

void win_loop()
{
	float dt;
	struct timeval t1, t2;
	struct timezone tz;
	gettimeofday(&t1, &tz);

	while (true)
	{
		if (nativeInputQueue && AInputQueue_hasEvents(nativeInputQueue) > 0)
		{
			AInputEvent* ev;
			if (AInputQueue_getEvent(nativeInputQueue, &ev) >= 0)
			{
				//handle event
			}
			AInputQueue_finishEvent(nativeInputQueue, ev, true);
		}
		else if (nativeShowWindow == NULL)
		{
			while (nativeShowWindow == NULL)
				usleep(1000);
		
			if (nativeWindow != nativeShowWindow)
			{ 
				nativeWindow = nativeShowWindow;
				eglSurface = eglCreateWindowSurface(eglDisplay, eglConfig, nativeWindow, NULL);
				if (eglSurface == EGL_NO_SURFACE || eglGetError() != EGL_SUCCESS)
					LOGI("[VI3D]eglCreateWindowSurface %d", eglGetError());
				if (!eglMakeCurrent(eglDisplay, eglSurface, eglSurface, eglContext) || eglGetError() != EGL_SUCCESS)
					LOGI("[VI3D]eglMakeCurrent %d", eglGetError());
			}
		}
		else
		{
			gettimeofday(&t2, &tz);
			dt = (float)(t2.tv_sec - t1.tv_sec + (t2.tv_usec - t1.tv_usec) * 1e-6);
			t1 = t2;

			//update
			//render
			test_draw();

			eglSwapBuffers(eglDisplay, eglSurface);
			usleep(10000);
		}
		
	}
}



void* android_main(void* args)
{
	if (win_init() == false)
		return NULL;
	if (egl_init() == false)
		return NULL;


	//LOGI((const char*)glGetString(GL_EXTENSIONS));
	test_init();


	win_loop();


	egl_exit();
	win_exit();
	return NULL;
}





//NativeActivity
static void onStart(ANativeActivity* activity)
{

}

static void onResume(ANativeActivity* activity)
{

}

static void onPause(ANativeActivity* activity)
{

}

static void onStop(ANativeActivity* activity)
{

}

static void onDestroy(ANativeActivity* activity)
{

}

static void onLowMemory(ANativeActivity* activity)
{

}

static void* onSaveInstanceState(ANativeActivity* activity, size_t* outLen)
{
	outLen = 0;
	return 0;
}

static void onWindowFocusChanged(ANativeActivity* activity, int focused)
{
	LOGI("[VI3D]onWindowFocusChanged %d", focused);
}

static void onConfigurationChanged(ANativeActivity* activity)
{
	LOGI("[VI3D]onConfigurationChanged");
}

static void onNativeWindowCreated(ANativeActivity* activity, ANativeWindow* window)
{
	LOGI("[VI3D]onNativeWindowCreated  %p, %p", nativeShowWindow, window);
	nativeShowWindow = window;
}

static void onNativeWindowDestroyed(ANativeActivity* activity, ANativeWindow* window)
{
	nativeShowWindow = NULL;
	LOGI("[VI3D]onNativeWindowDestroyed %p", window);
}

static void onInputQueueCreated(ANativeActivity* activity, AInputQueue* queue)
{
	nativeInputQueue = queue;
}

static void onInputQueueDestroyed(ANativeActivity* activity, AInputQueue* queue)
{
	nativeInputQueue = NULL;
}




void ANativeActivity_onCreate(ANativeActivity* activity, void* savedState, size_t savedStateSize)
{
	LOGI("[VI3D]Creating: %p", activity);

	if (nativeActivity != activity)
	{
		activity->callbacks->onStart = onStart;
		activity->callbacks->onResume = onResume;
		activity->callbacks->onPause = onPause;
		activity->callbacks->onStop = onStop;
		activity->callbacks->onDestroy = onDestroy;
		activity->callbacks->onLowMemory = onLowMemory;
		activity->callbacks->onSaveInstanceState = onSaveInstanceState;
		activity->callbacks->onWindowFocusChanged = onWindowFocusChanged;
		activity->callbacks->onConfigurationChanged = onConfigurationChanged;
		activity->callbacks->onNativeWindowCreated = onNativeWindowCreated;
		activity->callbacks->onNativeWindowDestroyed = onNativeWindowDestroyed;
		activity->callbacks->onInputQueueCreated = onInputQueueCreated;
		activity->callbacks->onInputQueueDestroyed = onInputQueueDestroyed;
	}
	
	if (nativeActivity == NULL)
	{
		pthread_t tid;
		pthread_create(&tid, 0, &android_main, 0);
	}

	nativeActivity = activity;
}



















