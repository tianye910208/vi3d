#ifndef _VI3D_H_
#define _VI3D_H_

#include "macro.h"
class Event;


DLLEXP void    vi_system_init();
DLLEXP float   vi_system_time();
DLLEXP void    vi_system_wait(float dt);
DLLEXP char*   vi_system_info();
DLLEXP void    vi_system_quit(int reason);
DLLEXP void    vi_system_exit();
DLLEXP void    vi_system_put_event(Event &e);
DLLEXP bool    vi_system_get_event(Event &e);
DLLEXP void    vi_system_run_event(Event &e);
DLLEXP void    vi_system_set_fps(int fps);
DLLEXP int     vi_system_get_fps();
DLLEXP float   vi_system_get_fps_dt();



DLLEXP void    vi_window_show(const char* title, int w, int h);
DLLEXP bool    vi_window_get_event(Event &e);
DLLEXP void    vi_window_set_fullscreen(bool flag);
DLLEXP void    vi_window_set_size(int w, int h);
DLLEXP void    vi_window_get_size(int &w, int &h);




#endif 


