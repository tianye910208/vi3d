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


#include "vi3d.h"

int runflag = 1;
int actived = 1;

int screenWidth = 0;
int screenHeight = 0;

ANativeActivity* nativeActivity = NULL;
ANativeWindow*	 nativeShowWindow = NULL;
AInputQueue*     nativeInputQueue = NULL;

EGLNativeWindowType  nativeWindow = NULL;


vi_msg* msg;
int     msgid;
void msg_proc(AInputEvent* ev) {
	switch (AInputEvent_getType(ev)) {
	case AINPUT_EVENT_TYPE_MOTION:
		switch (AInputEvent_getSource(ev)) {
		case AINPUT_SOURCE_TOUCHSCREEN:
			msg = NULL;
			switch(AMotionEvent_getAction(ev)) {
			case AMOTION_EVENT_ACTION_MOVE:
				msg = vi_msg_push(VI_MSG_TOUCH_MOVE, 3);
				break;
			case AMOTION_EVENT_ACTION_DOWN:
				msg = vi_msg_push(VI_MSG_TOUCH_DOWN, 3);
				break;
			case AMOTION_EVENT_ACTION_UP:
				msg = vi_msg_push(VI_MSG_TOUCH_UP, 3);
				break;
			}
			if(msg) {
				msg->data[0] = AMotionEvent_getX(ev, 0);
				msg->data[1] = AMotionEvent_getY(ev, 0);
				msg->data[2] = 0;
			}
			break;
		case AINPUT_SOURCE_TRACKBALL:
			break;
		}
		break;
		
	case AINPUT_EVENT_TYPE_KEY:
		break;
	}
}

void* _main(void* args) {
	//init------------------------------------------
	if(vi_app_init(NULL, nativeWindow, "", nativeActivity->internalDataPath)) return NULL;
	if(vi_app_main()) return NULL;
	

	//loop------------------------------------------
	float dt;
	struct timeval t1, t2;
	gettimeofday(&t1, NULL);
	
	runflag = 1;
	while (runflag) {
		if (nativeInputQueue && AInputQueue_hasEvents(nativeInputQueue) > 0) {
			AInputEvent* ev;
			if (AInputQueue_getEvent(nativeInputQueue, &ev) >= 0)
				msg_proc(ev);
			AInputQueue_finishEvent(nativeInputQueue, ev, 1);
		}
		else if (nativeShowWindow == NULL) {
			usleep(10000);
		}
		else if (nativeShowWindow != nativeWindow) {
			nativeWindow = nativeShowWindow;
			vi_gles_egl_init(NULL, nativeWindow);
			//vi_app_set_screen_size(screenWidth, screenHeight);
		}
		else {
			gettimeofday(&t2, NULL);
			dt = (float)(t2.tv_sec - t1.tv_sec + (t2.tv_usec - t1.tv_usec) * 1e-6);
			t1 = t2;
			if (actived)
				vi_app_loop(dt);
			
			usleep(10000);
		}

	}

	//init------------------------------------------
	vi_app_exit();

	return NULL;
}





//NativeActivity-----------------------------------
static void onStart(ANativeActivity* activity) {
	actived = 1;
}

static void onResume(ANativeActivity* activity) {
	actived = 1;
}

static void onPause(ANativeActivity* activity) {
	actived = 0;
}

static void onStop(ANativeActivity* activity) {
	actived = 0;
}

static void onDestroy(ANativeActivity* activity) {
	runflag = 0;
}

static void onLowMemory(ANativeActivity* activity) {

}

static void* onSaveInstanceState(ANativeActivity* activity, size_t* outLen) {
	outLen = 0;
	return 0;
}

static void onWindowFocusChanged(ANativeActivity* activity, int focused) {
	vi_log("onWindowFocusChanged %d", focused);

}

static void onConfigurationChanged(ANativeActivity* activity) {
	vi_log("onConfigurationChanged");
}

static void onNativeWindowCreated(ANativeActivity* activity, ANativeWindow* window) {
	screenWidth = ANativeWindow_getWidth(window);
	screenHeight = ANativeWindow_getHeight(window);
	vi_app_set_screen_size(screenWidth, screenHeight);

	vi_log("onNativeWindowCreated  %p, %p, w:%d h:%d", nativeShowWindow, window, screenWidth, screenHeight);
	nativeShowWindow = window;

	if (nativeWindow == NULL) {
		nativeWindow = nativeShowWindow;	
		pthread_t tid;
		pthread_create(&tid, 0, &_main, 0);
	}
}

static void onNativeWindowDestroyed(ANativeActivity* activity, ANativeWindow* window) {
	nativeShowWindow = NULL;
	vi_log("onNativeWindowDestroyed %p", window);
}

static void onInputQueueCreated(ANativeActivity* activity, AInputQueue* queue) {
	nativeInputQueue = queue;
}

static void onInputQueueDestroyed(ANativeActivity* activity, AInputQueue* queue) {
	nativeInputQueue = NULL;
}


void ANativeActivity_onCreate(ANativeActivity* activity, void* savedState, size_t savedStateSize) {
	vi_log("onCreate: %p", activity);

	if (nativeActivity != activity) {
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
	
	nativeActivity = activity;
	vi_sys_set_activity(nativeActivity);

}



















