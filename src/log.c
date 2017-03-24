#include "log.h"

#ifdef VI3D_SYS_ANDROID
#include<android/log.h>
#define _log_d_print(...) ((void)__android_log_print(ANDROID_LOG_DEBUG, "VI3D", __VA_ARGS__))
#define _log_i_print(...) ((void)__android_log_print(ANDROID_LOG_INFO, "VI3D", __VA_ARGS__))
#define _log_w_print(...) ((void)__android_log_print(ANDROID_LOG_WARN, "VI3D", __VA_ARGS__))
#define _log_e_print(...) ((void)__android_log_print(ANDROID_LOG_ERROR, "VI3D", __VA_ARGS__))
#else
#define _log_d_print(...)  printf(__VA_ARGS__)
#define _log_i_print(...)  printf(__VA_ARGS__)
#define _log_w_print(...)  printf(__VA_ARGS__)
#define _log_e_print(...)  printf(__VA_ARGS__)
#endif


void _log_d(const char* file, int line, const char* str)
{
	_log_d_print("[VI3D][D] %s\n@%s:%d", str, file, line);
}
void _log_i(const char* file, int line, const char* str)
{
	_log_i_print("[VI3D][I] %s\n@%s:%d", str, file, line);
}
void _log_w(const char* file, int line, const char* str)
{
	_log_w_print("[VI3D][W] %s\n@%s:%d", str, file, line);
}
void _log_e(const char* file, int line, const char* str)
{
	_log_e_print("[VI3D][E] %s\n@%s:%d", str, file, line);
}





