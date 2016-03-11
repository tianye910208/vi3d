#include "macro.h"

#ifdef VI3D_PLATFORM_ANDROID

#include <EGL/egl.h>

#include <jni.h>
#include <errno.h>

#include <poll.h>
#include <sched.h>
#include <unistd.h>
#include <pthread.h>

#include <android/sensor.h>
#include <android/log.h>
#include <android/configuration.h>
#include <android/looper.h>
#include <android/input.h>
#include <android/native_activity.h>
#include <android/native_window.h>

#include <EGL/egl.h>


#include "event.h"
#include "window.h"
#include "system.h"
using namespace vi3d;

#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, "native-activity", __VA_ARGS__))
#define LOGW(...) ((void)__android_log_print(ANDROID_LOG_WARN, "native-activity", __VA_ARGS__))



int vi_main(int argc, char* argv[]);
void* vi_main_thread(void* param) 
{ 
    vi_main(0, NULL); 
    return NULL; 
}


pthread_mutex_t mutex;
struct Lock 
{ 
    Lock(pthread_mutex_t* m) 
    { 
        x=m; 
        pthread_mutex_lock(x); 
    } 
    ~Lock() 
    { 
        pthread_mutex_unlock(x); 
    } 
private: 
    pthread_mutex_t* x; 
};




bool             windowInitflag = false;
ANativeWindow*   windowInstance = NULL;
ANativeActivity* nativeActivity = NULL;
AInputQueue*     inputQueue     = NULL;


static void onStart(ANativeActivity* activity) 		{ }
static void onStop(ANativeActivity* activity) 		{ }
static void onResume(ANativeActivity* activity) 	{ }
static void onPause(ANativeActivity* activity)  	{ }
static void onLowMemory(ANativeActivity* activity) 	{ }

static void* onSaveInstanceState(ANativeActivity* activity, size_t* outLen) 			
{ 
    outLen = 0; 
    return 0;
}

static void onWindowFocusChanged(ANativeActivity* activity, int focused)
{ 
    Event ev; 
    ev.type = (focused) ? Event::WM_GOT_FOCUS : Event::WM_LOST_FOCUS; 
    System::inst()->putEvent(ev); 
}

static void onInputQueueCreated(ANativeActivity* activity, AInputQueue* queue)
{ 
    Lock guard(&mutex); 
    inputQueue = queue;
}

static void onInputQueueDestroyed(ANativeActivity* activity, AInputQueue* queue)
{ 
    Lock guard(&mutex); 
    inputQueue = 0; 
}

static void onNativeWindowCreated(ANativeActivity* activity, ANativeWindow* window)
{ 
    Lock guard(&mutex); 
    windowInstance = window; 
    windowInitflag = true;
}

static void onNativeWindowDestroyed(ANativeActivity* activity, ANativeWindow* window)
{ 
    Lock guard(&mutex); 
    windowInstance = NULL; 
    windowInitflag = true;
}

static void onDestroy(ANativeActivity* activity)
{
    pthread_mutex_destroy(&mutex);
}

static void onConfigurationChanged(ANativeActivity* activity)
{
    AConfiguration *config = AConfiguration_new();
    AConfiguration_fromAssetManager(config, activity->assetManager);

    Event ev(Event::WM_ORIENTATION);
    if (AConfiguration_getOrientation(config) == ACONFIGURATION_ORIENTATION_LAND)
        ev.data.n = 1;
    else
        ev.data.n = 2;
    AConfiguration_delete(config);

    System::inst()->putEvent(ev);
}


void ANativeActivity_onCreate(ANativeActivity* activity, void* savedState, size_t savedStateSize)
{
	nativeActivity = activity;

	LOGI("[VI3D]Creating: %p\n==================================", activity);

    activity->callbacks->onDestroy = onDestroy;
    activity->callbacks->onStart = onStart;
    activity->callbacks->onResume = onResume;
    activity->callbacks->onSaveInstanceState = onSaveInstanceState;
    activity->callbacks->onPause = onPause;
    activity->callbacks->onStop = onStop;
    activity->callbacks->onConfigurationChanged = onConfigurationChanged;
    activity->callbacks->onLowMemory = onLowMemory;
    activity->callbacks->onWindowFocusChanged = onWindowFocusChanged;
    activity->callbacks->onNativeWindowCreated = onNativeWindowCreated;
    activity->callbacks->onNativeWindowDestroyed = onNativeWindowDestroyed;
    activity->callbacks->onInputQueueCreated = onInputQueueCreated;
    activity->callbacks->onInputQueueDestroyed = onInputQueueDestroyed;

    // Start the main thread
    pthread_mutex_init(&mutex, 0);
    pthread_t tid;
    pthread_create(&tid, 0, &vi_main_thread, 0);
}


namespace vi3d
{

class WindowAndroid:public Window
{
public:
    WindowAndroid();
    ~WindowAndroid();

    void show(const char* title, int w, int h);
    void swap();
    void getSize(int &w, int &h);
    bool getEvent(Event &e);
private:
    bool getEvent(AInputEvent* pEvent, Event &ev);

private:
    ANativeWindow*   m_window;
    EGLDisplay  m_display;
    EGLConfig   m_config;
    EGLint      m_format;
    EGLSurface  m_surface;
    EGLContext  m_context;
};

WindowAndroid::WindowAndroid()
:m_window(NULL)
,m_surface(EGL_NO_SURFACE)
{

}

WindowAndroid::~WindowAndroid()
{ 
    eglMakeCurrent(m_display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
    eglDestroyContext(m_display, m_context);
    eglTerminate(m_display);
}

void WindowAndroid::show(const char* title, int w, int h)
{
	while(windowInstance == NULL)
		usleep(10000);
    m_window = windowInstance;

    m_display = eglGetDisplay(EGL_DEFAULT_DISPLAY);
    eglInitialize(m_display, 0, 0);
    eglBindAPI(EGL_OPENGL_ES_API);
    eglMakeCurrent(m_display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
    
    EGLint numConfigs;
    static const EGLint attribs[] = { EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT, EGL_SURFACE_TYPE, EGL_WINDOW_BIT, EGL_BLUE_SIZE, 8, EGL_GREEN_SIZE, 8, EGL_RED_SIZE, 8, EGL_ALPHA_SIZE, 8, EGL_DEPTH_SIZE, 16, EGL_NONE };
    eglChooseConfig(m_display, attribs, &m_config, 1, &numConfigs);
    eglGetConfigAttrib(m_display, m_config, EGL_NATIVE_VISUAL_ID, &m_format);

    EGLint contextAttrs[] = { EGL_CONTEXT_CLIENT_VERSION, 2, EGL_NONE };
    m_context = eglCreateContext(m_display, m_config, NULL, contextAttrs);

    m_surface = eglCreateWindowSurface(m_display, m_config, m_window, NULL);
    eglMakeCurrent(m_display, m_surface, m_surface, m_context);// should == EGL_TRUE
}

void WindowAndroid::swap()
{
    eglSwapBuffers(m_display, m_surface);
}

void WindowAndroid::getSize(int &width, int &height)
{ 
    if(m_surface != EGL_NO_SURFACE)
    {
        EGLint tw, th; 
        eglQuerySurface(m_display, m_surface, EGL_WIDTH, &tw);
        eglQuerySurface(m_display, m_surface, EGL_HEIGHT, &th);

        width = tw;
        height = th;
    }
    else if(m_window)
    {
        width = ANativeWindow_getWidth(m_window); 
        height = ANativeWindow_getHeight(m_window); 
    }
    else
    {
        width = 0;
        height = 0;
    }
}


bool WindowAndroid::getEvent(Event &ev)
{
	Lock guard(&mutex);
    if(windowInitflag)
    {
        windowInitflag = false;

        if(windowInstance)
        {
            if(windowInstance != m_window)
            {
                m_window = windowInstance;
                m_surface = eglCreateWindowSurface(m_display, m_config, m_window, NULL);
                eglMakeCurrent(m_display, m_surface, m_surface, m_context);// should == EGL_TRUE
            }
        }
        else
        {
            m_window = NULL;
        }
    }

	bool handled = false;
	while(!handled && inputQueue && AInputQueue_hasEvents(inputQueue) > 0)
	{
		AInputEvent* outEvent;
		if (AInputQueue_getEvent(inputQueue, &outEvent) >= 0)
			handled = getEvent(outEvent, ev);
		AInputQueue_finishEvent(inputQueue, outEvent, true);
	}

	return handled;
}

bool WindowAndroid::getEvent(AInputEvent* pEvent, Event &ev)
{
	switch (AInputEvent_getType(pEvent))
	{
	case AINPUT_EVENT_TYPE_KEY: 
        {
		    switch(AKeyEvent_getAction(pEvent)) 
            {
		    case AKEY_EVENT_ACTION_DOWN: 
                ev.type = Event::IO_KEY_DOWN; 
                break;
		    case AKEY_EVENT_ACTION_UP: 
                ev.type = Event::IO_KEY_UP; 
                break;
		    default: 
                return false; 
		    }

		    uint32_t metaState = AKeyEvent_getMetaState(pEvent);
		    ev.data.key.ctrl = (metaState & AMETA_ALT_ON == AMETA_ALT_ON) ? 2: ((metaState & AMETA_SHIFT_ON == AMETA_SHIFT_ON) ? 1: 0);
		    ev.data.key.code = AKeyEvent_getKeyCode(pEvent);
	    }
		return true;
	case AINPUT_EVENT_TYPE_MOTION: 
        {
		    uint32_t type = AMotionEvent_getAction(pEvent) & AMOTION_EVENT_ACTION_MASK;
		    uint32_t index = (AMotionEvent_getAction(pEvent) & AMOTION_EVENT_ACTION_MASK) >> AMOTION_EVENT_ACTION_POINTER_INDEX_SHIFT;

		    switch(type)
		    {
		    case AMOTION_EVENT_ACTION_DOWN: 
		    case AMOTION_EVENT_ACTION_POINTER_DOWN:
                {
		    	    ev.type = Event::IO_TOUCH_DOWN;
		    	    ev.data.touch.type = 1;
		    	    ev.data.touch.idx = AMotionEvent_getPointerId(pEvent, 0);
                    ev.data.touch.x = AMotionEvent_getX(pEvent, 0);
                    ev.data.touch.y = AMotionEvent_getY(pEvent, 0);
		        }
		    	return true;
		    case AMOTION_EVENT_ACTION_MOVE: 
                {
		    	    ev.type = Event::IO_TOUCH_MOVE;
		    	    ev.data.touch.type = 1;
		    	    ev.data.touch.idx = AMotionEvent_getPointerId(pEvent, 0);
                    ev.data.touch.x = AMotionEvent_getX(pEvent, 0);
                    ev.data.touch.y = AMotionEvent_getY(pEvent, 0);

		    	    // Add the rest of the touch points to the window event queue.
		    	    uint32_t count = AMotionEvent_getPointerCount(pEvent);
		    	    for (uint32_t index = 1; index < count; ++index)
		    	    {
		    	    	Event ev(Event::IO_TOUCH_MOVE);
		    	    	ev.data.touch.idx = AMotionEvent_getPointerId(pEvent, index);
                        ev.data.touch.x = AMotionEvent_getX(pEvent, index);
                        ev.data.touch.y = AMotionEvent_getY(pEvent, index);
                        System::inst()->putEvent(ev);
		    	    }
                }
		    	return true;
		    case AMOTION_EVENT_ACTION_UP:
		    case AMOTION_EVENT_ACTION_CANCEL: 
		    case AMOTION_EVENT_ACTION_POINTER_UP:
                {
		        	ev.type = Event::IO_TOUCH_UP;
		    	    ev.data.touch.type = 1;
		    	    ev.data.touch.idx = AMotionEvent_getPointerId(pEvent, 0);
                    ev.data.touch.x = AMotionEvent_getX(pEvent, 0);
                    ev.data.touch.y = AMotionEvent_getY(pEvent, 0);
		        }
		    	return true;
            default:
                break;
		    }
	    }
        break;
    default:
        break;
	}
	return false;
}




Window* Window::inst()
{
    if(gptr)
    {
        return gptr;
    }
    else
    {
        gptr = new WindowAndroid();
        return gptr;
    }

}




}


#endif
