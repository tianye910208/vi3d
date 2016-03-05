
#include "event.h"

namespace vi3d
{


EventQueue::EventQueue()
{
    head = NULL;
    tail = NULL;
}

EventQueue::~EventQueue()
{
    Event* e = head;
    while(e)
    {
        Event* next = e->next;
        delete e;
        e = next;
    }

}

void EventQueue::push(Event *e)
{
    if(tail)
    {
        tail->next = e;
        tail = e;
    }
    else
    {
        head = tail = e;
    }


}

Event* EventQueue::pull()
{
    if(head)
    {
        Event* e = head;
        if(e->next)
            head = e->next;
        else
            head = tail = NULL;
        return e;
    }
    else
    {
        return NULL;
    }
}









}








