#include "vi_sys.h"




#ifdef VI3D_SYS_WIN

#endif
#ifdef VI3D_SYS_LINUX

#endif
#ifdef VI3D_SYS_ANDROID

static ANativeActivity* __sys_activity = NULL;
void vi_sys_set_activity(ANativeActivity* activity)
{
	__sys_activity = activity;
}
ANativeActivity* vi_sys_get_activity()
{
	return __sys_activity;
}

#endif
#ifdef VI3D_SYS_IOS

#endif
#ifdef VI3D_SYS_OSX

#endif









