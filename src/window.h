#ifndef _VI3D_WINDOW_
#define _VI3D_WINDOW_





class Window
{
public:
    static Window* g_inst;
    int w;
    int h;
public:
    static Window* inst();
    virtual void init(char* title, int w, int h);
    virtual void exit();

    virtual void pushEvent(void* e);
    virtual void pullEvent(void* e);
    virtual void execEvent(void* e);

    virtual void setSize(int w, int h);
    virtual void setFullscreen(bool flag);
};














#endif







