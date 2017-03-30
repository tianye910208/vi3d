#include "vi_log.h"

#define LOG_MAX_LEN 4096

static vi_log_func	__log_func = _vi_log_print;

void vi_log_setfunc(vi_log_func func)
{
	__log_func = func;
}

vi_log_func vi_log_getfunc()
{
	return __log_func;
}



void _vi_log_print(const char* file, int line, const char* msg)
{
	vi_log_print("%s  @%s:%d\n", msg, file, line);
}

void _vi_log(const char* file, int line, const char* fmt, ...)
{
	if (__log_func)
	{
		char str[LOG_MAX_LEN];
		va_list ap;
		va_start(ap, fmt);
		int n = vsnprintf(str, LOG_MAX_LEN - 1, fmt, ap);
		if (n < 0 || n >= LOG_MAX_LEN - 1)
			str[LOG_MAX_LEN - 1] = '\0';
		va_end(ap);

		__log_func(file, line, str);
	}
}










