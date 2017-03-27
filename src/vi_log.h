#ifndef _VI3D_LOG_
#define _VI3D_LOG_

#include "vi_sys.h"


#ifdef VI3D_SYS_ANDROID
#include<android/log.h>
#define log_print(x) (__android_log_print(ANDROID_LOG_INFO, "VI3D", "%s", x))
#else
#define log_print(x) (printf("%s", x))
#endif

#define log		     _log_mark(__FILE__, __LINE__)&&_log_work


typedef int (*log_func)(const char* msg, const char* file, int line);

int		 log_set_func(log_func func);
log_func log_get_func();

int _log_print(const char* msg, const char* file, int line);
int _log_mark(const char* file, int line);
int _log_work(const char* fmt, ...);



#endif 


