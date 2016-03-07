#ifndef _VI3D_SYSTEM_
#define _VI3D_SYSTEM_

class Event;
class EventQueue;

class System
{
public:
    static System*  gptr;
    static System*  inst();
    static void     exit();

public:
    float time();
    void  wait(float dt);
    char* info();

    void  quit(int reason);

    void  setFps(int fps);
    int   getFps();
    float getFpsDt();

    void  putEvent(Event &e);
    bool  getEvent(Event &e);
    void  runEvent(Event &e);

private:
    int         _fps;
    float       _fpsDt;
    EventQueue  _eventQueue;
}


#endif







