#ifndef _VI3D_SYS_
#define _VI3D_SYS_

//PLATFORM
#if defined(_WIN32) || defined(_WINDOWS) || defined(WIN32)
	#define VI3D_SYS_WIN 1
	#define VI3D_SYS "WIN"
#elif defined(__APPLE__) || defined(__APPLE_CC__)
	#if (TARGET_OS_IPHONE || TARGET_IPHONE_SIMULATOR)
		#define VI3D_SYS_IOS 2
		#define VI3D_SYS "IOS"
	#else
		#define VI3D_SYS_OSX 3
		#define VI3D_SYS "OSX"
	#endif
#elif defined(__ANDROID__)
	#define VI3D_SYS_ANDROID 4
	#define VI3D_SYS "ANDROID"
#elif defined(__linux__)
	#define VI3D_SYS_LINUX 5
	#define VI3D_SYS "LINUX"
#else
	#error VI3D_ERROR_PLATFORM_UNKNOWN
#endif


#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>

//ASSERT
#if defined(_DEBUG) || defined(DEBUG)
	#define VI3D_DEBUG
	#include <assert.h>
	#define ASSERT(exp) assert(exp)
#else
	#define ASSERT(exp)
#endif

#ifndef NULL
	#define NULL 0
#endif

//TYPES
typedef signed char         int8;
typedef unsigned char       uint8;
typedef signed short        int16;
typedef unsigned short      uint16;
typedef signed int          int32;
typedef unsigned int        uint32;
typedef signed long long    int64;
typedef unsigned long long  uint64;



#ifdef VI3D_SYS_WIN

#define WIN32_LEAN_AND_MEAN
#include <GLES2/gl2.h>
#include <EGL/egl.h>
#include <EGL/eglext.h>

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
#include <android/native_activity.h>

void vi_sys_set_activity(ANativeActivity* activity);
ANativeActivity* vi_sys_get_activity();

#endif


#endif 


