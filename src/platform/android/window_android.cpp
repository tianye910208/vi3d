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

#include "event.h"
#include "window.h"
#include "system.h"
using namespace vi3d;

#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, "native-activity", __VA_ARGS__))
#define LOGW(...) ((void)__android_log_print(ANDROID_LOG_WARN, "native-activity", __VA_ARGS__))



int main(int argc, char *argv[]);
void* start_main(void* param) 
{ 
    main(0,0); 
    return 0; 
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




ANativeWindow*   windowInstance = 0;
ANativeActivity* nativeActivity = 0;
AInputQueue*     inputQueue;


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
}

static void onNativeWindowDestroyed(ANativeActivity* activity, ANativeWindow* window)
{ 
    Lock guard(&mutex); 
    windowInstance = 0; 
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
    pthread_create(&tid, 0, &start_main, 0);
}


namespace vi3d
{

class WindowAndroid:public Window
{
public:
    WindowAndroid();
    ~WindowAndroid();

    void show(const char* title, int w, int h);
    void getSize(int &w, int &h);
    bool getEvent(Event &e);
private:
    bool getEvent(AInputEvent* pEvent, Event &ev);
};

WindowAndroid::WindowAndroid()
{

}

WindowAndroid::~WindowAndroid()
{ 
    if(windowInstance)
        ANativeWindow_release(windowInstance); 
}

void WindowAndroid::show(const char* title, int w, int h)
{
	while(windowInstance == 0)
		usleep(10000);

	ANativeWindow_acquire(windowInstance);
}

void WindowAndroid::getSize(int &width, int &height)
{ 
    if(windowInstance)
    {
        width = ANativeWindow_getWidth(windowInstance); 
        height = ANativeWindow_getHeight(windowInstance); 
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
