#include "log.h"

#define LOG_MAX_LEN 4096

static const char*	__log_mark_file = NULL;
static int			__log_mark_line = 0;
static log_func		__log_func = _log_print;


int log_set_func(log_func func)
{
	__log_func = func;
	return 0;
}

log_func log_get_func()
{
	return __log_func;
}



int _log_print(const char* msg, const char* file, int line)
{
	char tmp[LOG_MAX_LEN];
	sprintf(tmp, "%s  @%s:%d\n", msg, file, line);
	log_print(tmp);
	return 0;
}

int _log_mark(const char* file, int line)
{
	__log_mark_file = (char*)file;
	__log_mark_line = line;
	return 1;
}

int _log_work(const char* fmt, ...)
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

		__log_func(str, __log_mark_file, __log_mark_line);
	}
	return 0;
}










