#include "vi3d.h"

bool g_loop = true;

bool handle(Event ev)
{
    if(ev.type == Event::SYS_EXIT || ev.type == Event::WM_CLOSE)
    {
        g_loop = false;
        return false;
    }

    return true;
}


int main(int argc, char *argv[])
{

    vi_system_init();

    vi_window_show("vi3d", 800, 600);

    //vi_script_exec("main.lua");

    float pt = vi_system_time();
    float dt = 0;
    while(g_loop)
    {
        float vt = vi_system_get_fps_dt();

        Event ev;
        while (vi_window_get_event(ev))
        {
            if(handle(ev))
                vi_system_run_event(ev);
        }

        while (vi_system_get_event(ev))
        {
            if(handle(ev))
                vi_system_run_event(ev);
        }

        while(dt > vt)
        {
            vi_object_loop(dt);
            dt = dt - vt;
        }
        vi_render_loop(dt);

        float tt = vi_system_time();
        dt = dt + tt - pt;
        pt = tt;
        if(dt < vt)
            vi_system_wait(vt - dt);
    }

    vi_system_exit();

    return 0;
}

