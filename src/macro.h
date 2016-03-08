#ifndef _VI3D_MACRO_
#define _VI3D_MACRO_

//PLATFORM
#if defined(_WIN32) || defined(_WINDOWS) || defined(WIN32)
#   define WIN32_LEAN_AND_MEAN
#	define VI3D_PLATFORM_WIN 1
#	define VI3D_PLATFORM "WIN"
#elif defined(__APPLE__) || defined(__APPLE_CC__)
#   if TARGET_OS_IPHONE || TARGET_IPHONE_SIMULATOR
#       define VI3D_PLATFORM_IOS 2
#       define VI3D_PLATFORM "IOS"
#   else
#       define VI3D_PLATFORM_OSX 3
#       define VI3D_PLATFORM "OSX"
#   endif
#elif defined(__ANDROID__)
#   define VI3D_PLATFORM_ANDROID 4
#   define VI3D_PLATFORM "ANDROID"
#elif defined(__linux__)
#   define VI3D_PLATFORM_LINUX 5
#   define VI3D_PLATFORM "LINUX"
#else
#   error VI3D_ERROR_PLATFORM_UNKNOWN
#endif

//DLLEXPORT
#ifndef DLLEXP
#	ifdef VI3D_PLATFORM_WIN
#		define DLLEXP extern "C" __declspec( dllexport )
#	else
#		if defined(__GNUC__) && __GNUC__ >= 4
#		  define DLLEXP extern "C" __attribute__ ((visibility("default")))
#   	else
#		  define DLLEXP extern "C"
#   	endif
#	endif
#endif



//ASSERT
#if defined(_DEBUG)
#   define VI3D_DEBUG
#   include <assert.h>
#	define ASSERT( exp ) assert( exp );
#else
#	define ASSERT( exp )
#endif

#ifndef NULL
#   define NULL 0
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


#endif 


