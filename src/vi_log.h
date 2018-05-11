#ifndef _VI3D_LOG_
#define _VI3D_LOG_

#include "vi_sys.h"


#define vi_log(fmt, ...)       (_vi_log(__FILE__, __LINE__, fmt, ##__VA_ARGS__))
#define vi_log_print(fmt, ...) (_vi_log(NULL, 0, fmt, ##__VA_ARGS__))

typedef void (*vi_log_func)(const char* file, int line, const char* msg);

void		vi_log_setfunc(vi_log_func func);
vi_log_func vi_log_getfunc();


void _vi_log(const char* file, int line, const char* fmt, ...);



#endif 


