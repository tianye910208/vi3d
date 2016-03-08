#include "event.h"

namespace vi3d
{


EventQueue::EventQueue()
{
    temp = NULL;
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

    e = temp;
    while(e)
    {
        Event* next = e->next;
        delete e;
        e = next;
    }



}

void EventQueue::put(Event &ev)
{
    Event* e;
    if(temp)
    {
        e = temp;
        temp = e->next;

        *e = ev;
    }
    else
    {
        e = new Event(ev);
    }

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

bool EventQueue::get(Event &ev)
{
    if(head)
    {
        Event* e = head;
        if(e->next)
            head = e->next;
        else
            head = tail = NULL;

        ev = *e;

        e->next = temp;
        temp = e;
        return true;
    }
    else
    {
        return false;
    }
}






}








