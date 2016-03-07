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
        WM_CLOSE, 
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
        USR_DATA
    };

public:
    int    type;
    union {
        struct {float w; float h;} size;
        struct {int ctrl; int code;} key;
        struct {int type; int idx; float x; float y;} touch;
        struct {float x; float y; float dt;} wheel;
        void* ptr;
        int   n;
        float f;
    } data;
    Event* next;

public:
    Event()
    {
        next = NULL;
    }

    Event(int t)
    {
        type = t;
        next = NULL;
    }
    
    Event(SysEvent e)
    {
        type = e.type;
        data = e.data;
        next = e.next;
    }
};

class EventQueue
{
public:
    Event* temp;
    Event* head;
    Event* tail;
public:
    EventQueue();
    ~EventQueue();

    void put(Event &e);
    bool get(Event &e);
};


}
#endif







