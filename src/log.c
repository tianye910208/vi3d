#include "log.h"

#define LOG_MAX_LEN 4096

const char* __log_mark_file = NULL;
int		    __log_mark_line = 0;
char		__log_work_data[LOG_MAX_LEN];
log_func	__log_func = _log_print;


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
	sprintf(tmp, "%s  @%s:%d", msg, file, line);
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
		va_list ap;
		va_start(ap, fmt);
		int n = vsnprintf(__log_work_data, LOG_MAX_LEN - 1, fmt, ap);
		if (n < 0 || n >= LOG_MAX_LEN - 1)
			__log_work_data[LOG_MAX_LEN - 1] = '\0';
		va_end(ap);

		__log_func(__log_work_data, __log_mark_file, __log_mark_line);
	}
	return 0;
}










