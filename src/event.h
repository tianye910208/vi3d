#ifndef _VI3D_EVENT_
#define _VI3D_EVENT_

#include "macro.h"

namespace vi3d 
{ 

class Event
{
public:
    enum EventType
    {
        WM_EXIT, 
        WM_RESIZE, 
        WM_GOT_FOCUS, 
        WM_LOST_FOCUS,
        
        IO_TEXT_INPUT, 
        IO_KEY_DOWN, 
        IO_KEY_UP, 
        IO_TOUCH_DOWN, 
        IO_TOUCH_UP, 
        IO_TOUCH_MOVE, 
        IO_WHEEL,
    
        SYS_EXIT,
        USR_CUSTOM
    };

public:
    int    type;
    Event* next;

public:
    Event()
    {
        next = NULL;
    }
};

class SysEvent:public Event
{
public:
    union {
        struct {float w; float h;} size;
        struct {int ctrl; int code;} key;
        struct {int type; int idx; float x; float y;} touch;
        struct {float x; float y; float dt;} wheel;
    } data;
public:
    SysEvent()
    {

    }
    SysEvent(int t)
    {
        type = t;
    }
};

class UsrEvent:public Event
{
public:
    void* data;
public:
    UsrEvent()
    {
        type = USR_CUSTOM;
        data = NULL;
    }
    UsrEvent(void *p)
    {
        type = USR_CUSTOM;
        data = p;
    }
};

class EventQueue
{
public:
    Event* head;
    Event* tail;
public:
    EventQueue();
    ~EventQueue();

    void push(Event *e);
    Event* pull();
};









}
#endif







