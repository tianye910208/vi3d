#ifndef _VI_MACRO_
#define _VI_MACRO_

//PLATFORM
#if defined(_WIN32) || defined(_WINDOWS) || defined(WIN32)
#   define WIN32_LEAN_AND_MEAN
#	define VI_OS_WIN
#elif defined(__APPLE__) || defined(__APPLE_CC__)
#   if TARGET_OS_IPHONE || TARGET_IPHONE_SIMULATOR
#       define VI_OS_IOS
#   else
#       define VI_OS_OSX
#   endif
#elif defined(__ANDROID__)
#   define VI_OS_ANDROID
#elif defined(__linux__)
#   define VI_OS_LINUX
#else
#   error VI_ERROR_PLATFORM_UNKNOWN
#endif

//DLLEXPORT
#ifndef DLLEXP
#	ifdef VI_OS_WIN
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
#   define VI_DEBUG
#   include <assert.h>
#	define ASSERT( exp ) assert( exp );
#else
#	define ASSERT( exp )
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


