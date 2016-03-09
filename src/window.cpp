#include "window.h"
#include "macro.h"

namespace vi3d
{


Window* Window::gptr = NULL;
void Window::exit()
{
    if(gptr)
    {
        delete gptr;
        gptr = NULL;
    }

}

Window::Window()
{

}

Window::~Window()
{

}

void Window::show(const char* title, int w, int h)
{

}

void Window::swap()
{

}

bool Window::getEvent(Event &e)
{
    return false;
}

void Window::setFullscreen(bool flag)
{

}

void Window::setSize(int tw, int th)
{
    w = tw;
    h = th;
}

void Window::getSize(int &tw, int &th)
{
    tw = w;
    th = h;
}






}





