#ifndef _VI3D_LOG_
#define _VI3D_LOG_

#include "vi_sys.h"


#ifdef VI3D_SYS_ANDROID
#include<android/log.h>
#define vi_log_print(x) (__android_log_print(ANDROID_LOG_INFO, "VI3D", "%s", x))
#else
#define vi_log_print(x) (printf("%s", x))
#endif

#define vi_log		     _vi_log_mark(__FILE__, __LINE__)&&_vi_log_work


typedef int(*vi_log_func)(const char* msg, const char* file, int line);

int			vi_log_setfunc(vi_log_func func);
vi_log_func vi_log_getfunc();

int _vi_log_print(const char* msg, const char* file, int line);
int _vi_log_mark(const char* file, int line);
int _vi_log_work(const char* fmt, ...);



#endif 


