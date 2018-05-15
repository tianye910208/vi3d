#include "vi_log.h"
#include <malloc.h>

#define LOG_TMP_LEN 1024

static void _vi_log_print(const char* msg)
{
	vi_sys_print(msg);
}

vi_log_func __log_func = _vi_log_print;

void vi_log_setfunc(vi_log_func func)
{
	__log_func = func;
}

vi_log_func vi_log_getfunc()
{
	return __log_func;
}


void vi_log_1(const char* fmt)
{
	if (__log_func == NULL)
		return;
	__log_func(fmt);
}

void vi_log_2(const char* fmt, ...)
{
	if (__log_func == NULL)
		return;

	size_t fmtn = strlen(fmt);

#ifdef VI3D_SYS_WIN
	char *fmts = (char *)alloca(fmtn + 16);
#else
	char fmts[fmtn + 16];
#endif	
	strcpy(fmts, fmt);
	strcat(fmts, " @%s:%d\n");

	va_list ap;
	va_start(ap, fmt);

	char str[LOG_TMP_LEN + 1];
	int n = vsnprintf(str, LOG_TMP_LEN, fmts, ap);
	va_end(ap);

	if (n > -1 && n < LOG_TMP_LEN)
	{
		__log_func(str);
	}
	else
	{
        va_list ap;
	    va_start(ap, fmt);

#ifdef VI3D_SYS_WIN
		n = _vscprintf(fmts, ap);
		char *tmp = (char *)alloca(n + 1);
#else
		char tmp[n + 1];
#endif	

		vsnprintf(tmp, n, fmts, ap);
	    va_end(ap);

		__log_func(tmp);
	}

}




