#ifndef _VI3D_WINDOW_
#define _VI3D_WINDOW_

namespace vi3d
{

class Event;

class Window
{
public:
    static Window* gptr;
    static Window* inst();
    static void    exit();

public:
    Window();
    virtual ~Window();

    virtual void show(const char* title, int w, int h);

    virtual bool getEvent(Event &e);

    virtual void setFullscreen(bool flag);

    virtual void setSize(int w, int h);
    virtual void getSize(int &w, int &h);
public:
    int w;
    int h;
};





}
#endif







