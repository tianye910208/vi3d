#include "system.h"
#include "macro.h"
#include "event.h"

#ifdef VI3D_PLATFORM_WIN
#include <windows.h>
#else
#include <sys/time.h>
#include <unistd.h>
#endif

namespace vi3d
{


System* System::gptr = NULL;
System* System::inst()
{
    if(gptr)
    {
        return gptr;
    }
    else
    {
        gptr = new System();
        return gptr;
    }
}

System::System()
{
    setFps(60);
}

System::~System()
{

}


void System::exit()
{
    if(gptr)
    {
        delete gptr;
        gptr = NULL;
    }
}


float System::time()
{
#ifdef VI3D_PLATFORM_WIN
    return float(timeGetTime()/1000.0f);
#else
    timeval t;
    gettimeofday(&t, NULL);
    return float(t.tv_sec + t.tv_usec/1000000.0f);
#endif
}

void System::wait(float dt)
{
#ifdef VI3D_PLATFORM_WIN
    Sleep(DWORD(dt * 1000));
#else
    usleep((useconds_t)(dt * 1000000));
#endif
}

const char* System::info()
{
    return VI3D_PLATFORM;
}


void System::quit(int reason)
{
    Event e(Event::SYS_QUIT);
    putEvent(e);
}

void System::setFps(int fps)
{
    _fps = fps;
    _fpsDt = 1.0f/_fps;
}

int System::getFps()
{
    return _fps;
}

float System::getFpsDt()
{
    return _fpsDt;
}

void System::putEvent(Event &e)
{
    //TODO mutex
    _eventQueue.put(e);
}

bool System::getEvent(Event &e)
{
    //TODO mutex
    return _eventQueue.get(e);
}


}




