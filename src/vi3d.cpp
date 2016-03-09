#include "vi3d.h"
#include "system.h"
#include "window.h"

namespace vi3d
{

void vi_system_init()
{
    System::inst();
    Window::inst();
}

float vi_system_time()
{
    return System::inst()->time();
}

void vi_system_wait(float dt)
{
    System::inst()->wait(dt);
}

void vi_system_quit(int reason)
{
    System::inst()->quit(reason);
}

void vi_system_exit()
{
    Window::inst()->exit();
}


void vi_system_put_event(Event &e)
{
    System::inst()->putEvent(e);
}

bool vi_system_get_event(Event &e)
{
    return System::inst()->getEvent(e);
}

void vi_system_set_fps(int fps)
{
    System::inst()->setFps(fps);
}

int vi_system_get_fps()
{
    return System::inst()->getFps();
}

float vi_system_get_fps_dt()
{
    return System::inst()->getFpsDt();
}

const char* vi_system_info()
{
    return System::inst()->info();
}


void vi_window_show(const char* title, int w, int h)
{
    Window::inst()->show(title, w, h);
}

void vi_window_swap()
{
    Window::inst()->swap();
}

bool vi_window_get_event(Event &e)
{
    return Window::inst()->getEvent(e);
}

void vi_window_set_fullscreen(bool flag)
{
    Window::inst()->setFullscreen(flag);
}

void vi_window_set_size(int w, int h)
{
    Window::inst()->setSize(w, h);
}

void vi_window_get_size(int &w, int &h)
{
    Window::inst()->getSize(w, h);
}





}


