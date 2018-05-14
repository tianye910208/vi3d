#ifndef _VI3D_LOG_
#define _VI3D_LOG_

#include "vi_sys.h"


#define vi_log(fmt, ...) vi_log_2(fmt, ##__VA_ARGS__,  __FILE__, __LINE__)


typedef void(*vi_log_func)(const char* msg);

void		vi_log_setfunc(vi_log_func func);
vi_log_func vi_log_getfunc();

void        vi_log_1(const char* fmt);
void        vi_log_2(const char* fmt, ...);


#endif 


