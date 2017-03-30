#ifndef _VI3D_LOG_
#define _VI3D_LOG_

#include "vi_sys.h"


#ifdef VI3D_SYS_ANDROID
#include<android/log.h>
#define vi_log_print(fmt, arg, ...) (__android_log_print(ANDROID_LOG_INFO, "VI3D", fmt, arg, ##__VA_ARGS__))
#else
#define vi_log_print(fmt, arg, ...) (printf(fmt, arg, ##__VA_ARGS__))
#endif

#define vi_log(fmt, ...) (_vi_log(__FILE__, __LINE__, fmt, ##__VA_ARGS__))

typedef int(*vi_log_func)(const char* file, int line, const char* msg);

int			vi_log_setfunc(vi_log_func func);
vi_log_func vi_log_getfunc();

int _vi_log_print(const char* file, int line, const char* msg);
int _vi_log(const char* file, int line, const char* fmt, ...);



#endif 


