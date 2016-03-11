#include "vi3d.h"
using namespace vi3d;

#ifdef VI3D_PLATFORM_LINUX
#include <GL/gl.h>
#include <GL/glx.h>
#else
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#include <EGL/egl.h>
#endif


GLfloat alpha = 180.f, beta = 0.f;
float cursorX, cursorY;
bool lockedL = false;
bool lockedR = false;


bool g_flag = true;


void game_init()
{
    int w, h;
    vi_window_get_size(w, h);
    //vi_window_set_fullscreen(true);


    glClearColor(0,0,255,255);
}

bool game_event(Event e)
{
    switch(e.type)
    {
    case Event::SYS_QUIT:
    case Event::WM_CLOSE:
        g_flag = false;
        return false;
    case Event::IO_KEY_DOWN:
        vi_system_quit(0);
        return false;
    case Event::IO_TOUCH_MOVE:
        {
            float x = e.data.touch.x;
            float y = e.data.touch.y;
            if (lockedL)
            {   
                alpha += (GLfloat) (x - cursorX) / 10.f;
                beta += (GLfloat) (y - cursorY) / 10.f;
            }
            if(lockedR)
            {

            }
            cursorX = (int) x;
            cursorY = (int) y;
        }
        break;
    case Event::IO_TOUCH_DOWN:
        {
            float x = e.data.touch.x;
            float y = e.data.touch.y;
         
            cursorX = (int) x;
            cursorY = (int) y;

            lockedL = (e.data.touch.type == 1);
            lockedR = (e.data.touch.type == 2);
        }
        break;
    case Event::IO_TOUCH_UP:
        {
            lockedL = (e.data.touch.type == 1)?false:lockedL;
            lockedR = (e.data.touch.type == 2)?false:lockedR;
        }
        break;
    default:
        break;
    }

    //vi_script_run_event(ev);
    return true;
}

void game_update(float dt)
{

}

void game_render(float dt)
{

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


    vi_window_swap();
}

int vi_main(int argc, char *argv[])
{

    vi_system_init();

    vi_window_show("vi3d", 800, 600);

    game_init();
    //vi_script_exec("main.lua");

    float pt = vi_system_time();
    float gt = 0;
    while(g_flag)
    {
        float tt = vi_system_time();
        float vt = vi_system_get_fps_dt();
        float dt = tt - pt;
        gt = gt + dt;
        pt = tt;

        Event ev;
        while (vi_window_get_event(ev))
            game_event(ev);

        while (vi_system_get_event(ev))
            game_event(ev);

        while(gt > vt)
        {
            game_update(vt);
            //vi_object_loop(vt);
            gt = gt - vt;
        }
        game_render(dt);
        //vi_render_loop(dt);
        
        float rt = vi_system_time() - tt;
        if(rt < vt)
            vi_system_wait(vt - rt);
    }

    vi_system_exit();

    return 0;
}

