#ifndef _VI3D_LOG_
#define _VI3D_LOG_

#include "sys.h"

#define log_d(...)  do{char str[4096]; sprintf(str, __VA_ARGS__);_log_d(__FILE__, __LINE__, (const char*)str);}while(0)
#define log_i(...)  do{char str[4096]; sprintf(str, __VA_ARGS__);_log_i(__FILE__, __LINE__, (const char*)str);}while(0)
#define log_w(...)  do{char str[4096]; sprintf(str, __VA_ARGS__);_log_w(__FILE__, __LINE__, (const char*)str);}while(0)
#define log_e(...)  do{char str[4096]; sprintf(str, __VA_ARGS__);_log_e(__FILE__, __LINE__, (const char*)str);}while(0)



void _log_d(const char* file, int line, const char* str);
void _log_i(const char* file, int line, const char* str);
void _log_w(const char* file, int line, const char* str);
void _log_e(const char* file, int line, const char* str);


#endif 


