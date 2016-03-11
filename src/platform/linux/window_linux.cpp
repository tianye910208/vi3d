#include "macro.h"
#include "event.h"
#include "window.h"

#ifdef VI3D_PLATFORM_LINUX
#include <X11/Xlib.h>
#include <X11/XKBlib.h>
#include <X11/Xutil.h>
#include <cstdlib>
#include <cstring>
#include <sys/time.h>
#include <unistd.h>

#include <GL/gl.h>
#include <GL/glx.h>

int vi_main(int argc, char* argv[]);
int main(int argc, char* argv[])
{
    return vi_main(argc, argv);
}

namespace vi3d
{

class WindowLinux:public Window
{
public:
    WindowLinux();
    ~WindowLinux();

    void show(const char* title, int w, int h);
    void swap();
    void setFullscreen(bool flag);
    void getSize(int &w, int &h);
    bool getEvent(Event &e);
private:
    ::Window        x_window;
    ::Display*      x_display;
    ::GLXContext    x_context;
    
    ::Atom     m_msg_char;
    ::Atom     m_msg_exit;
 
    bool       m_fullscreen;
    XEvent     m_fullscreenXEvent;
};

WindowLinux::WindowLinux()
:m_fullscreen(false)
{

}

WindowLinux::~WindowLinux()
{
    if (m_fullscreen)
        setFullscreen(false);

    glXDestroyContext(x_display, x_context);

    XDestroyWindow(x_display, x_window);
    XCloseDisplay(x_display);
}

void WindowLinux::show(const char* title, int width, int height)
{
    x_display = XOpenDisplay(NULL);
    x_window = XCreateWindow(x_display, DefaultRootWindow(x_display), 0, 0, width, height, 0, CopyFromParent, InputOutput, CopyFromParent, 0, 0);

    XStoreName(x_display, x_window, title);
    XChangeProperty(x_display, x_window, XInternAtom(x_display, "_NET_WM_NAME", False), XInternAtom(x_display, "UTF8_STRING", False), 8, PropModeReplace, (unsigned char *) title, strlen(title));

    XSelectInput(x_display, x_window, ExposureMask | StructureNotifyMask | FocusChangeMask | VisibilityChangeMask | KeyPressMask | KeyReleaseMask | KeymapStateMask | ButtonPressMask | ButtonReleaseMask | PointerMotionMask );
    XFlush(x_display);
    XMapWindow(x_display, x_window);

    static const GLint attr[] = { GLX_RGBA, GLX_RED_SIZE, 8, GLX_GREEN_SIZE, 8, GLX_BLUE_SIZE, 8, GLX_DEPTH_SIZE, 24, GLX_DOUBLEBUFFER, None};
    XVisualInfo* vi = glXChooseVisual(x_display, 0, (int*)&attr);
    x_context = glXCreateContext(x_display, vi, NULL, GL_TRUE);
    glXMakeCurrent(x_display, x_window, x_context);
    XFree(vi);


    // Prepare a fullscreen window message
    m_fullscreenXEvent.type = ClientMessage;
    m_fullscreenXEvent.xclient.window = x_window;
    m_fullscreenXEvent.xclient.message_type = XInternAtom(x_display, "_NET_WM_STATE", False);
    m_fullscreenXEvent.xclient.format = 32;
    m_fullscreenXEvent.xclient.data.l[1] = XInternAtom(x_display, "_NET_WM_STATE_FULLSCREEN", False);
    m_fullscreenXEvent.xclient.data.l[2] = 0;

    m_msg_exit = XInternAtom(x_display, "WM_DELETE_WINDOW", False);
    m_msg_char = XInternAtom(x_display, "CLIENT_CHARACTER_MESSAGE", False);
    XSetWMProtocols(x_display, x_window, &m_msg_exit, 1);
}

void WindowLinux::swap()
{
    glXSwapBuffers(x_display, x_window);
}

void WindowLinux::setFullscreen(bool flag) 
{ 
    if (m_fullscreen == flag) 
        return; 
    m_fullscreen = flag;

    m_fullscreenXEvent.xclient.data.l[0] = flag ?1:0; // Set the mWindow to fullscreen mode   1 fullscreen 0 windowed
    XSendEvent(x_display, DefaultRootWindow(x_display), False, SubstructureNotifyMask, &m_fullscreenXEvent);
}

void WindowLinux::getSize(int &width, int &height) 
{
    XWindowAttributes attr;
    XGetWindowAttributes(x_display, x_window, &attr);

    width = attr.width;
    height = attr.height;
}


bool WindowLinux::getEvent(Event &ev)
{

    XEvent e;
    while(XPending(x_display))
    {
        XNextEvent(x_display, &e);

        switch (e.type)
        {
        case FocusIn:
            ev.type = Event::WM_GOT_FOCUS;
            return true;
        case FocusOut:
            ev.type = Event::WM_LOST_FOCUS;
            return true;
        case Expose:
            //if (e.xexpose.count == 0)
            //    Event::REFRESH;
            break;
        case ConfigureNotify:
            if (e.xconfigure.width != w|| e.xconfigure.height != h)
            {
                ev.type = Event::WM_RESIZE;
                ev.data.size.w = e.xconfigure.width;
                ev.data.size.h = e.xconfigure.height;
            }
            return true;
        case ButtonPress:
            {
                ev.type = Event::IO_TOUCH_DOWN;
                ev.data.touch.x = e.xmotion.x;
                ev.data.touch.y = e.xmotion.y;
                ev.data.touch.idx = 0;
                if (e.xbutton.button == Button1)
                {
                    ev.data.touch.type = 1;//LEFT
                    return true;
                }
                else if (e.xbutton.button == Button2)
                {
                    ev.data.touch.type = 2;//RIGHT
                    return true;
                }
            }
            break;
        case ButtonRelease:
            {
                if (e.xbutton.button == Button1)
                {
                    ev.type = Event::IO_TOUCH_UP;
                    ev.data.touch.x = e.xmotion.x;
                    ev.data.touch.y = e.xmotion.y;
                    ev.data.touch.idx = 0;
                    ev.data.touch.type = 1;//LEFT
                    return true;
                }
                else if (e.xbutton.button == Button2)
                {
                    ev.type = Event::IO_TOUCH_UP;
                    ev.data.touch.x = e.xmotion.x;
                    ev.data.touch.y = e.xmotion.y;
                    ev.data.touch.idx = 0;
                    ev.data.touch.type = 2;//RIGHT
                    return true;
                }
                else if (e.xbutton.button == Button4)
                {
                    ev.type = Event::IO_WHEEL;
                    ev.data.wheel.x = e.xmotion.x;
                    ev.data.wheel.y = e.xmotion.x;
                    ev.data.wheel.dt = 100;
                    return true;
                }
                else if (e.xbutton.button == Button5)
                {
                    ev.type = Event::IO_WHEEL;
                    ev.data.wheel.x = e.xmotion.x;
                    ev.data.wheel.y = e.xmotion.x;
                    ev.data.wheel.dt = -100;
                    return true;
                }
            }
            break;
        case MotionNotify:
            {
                ev.type = Event::IO_TOUCH_MOVE;
                ev.data.touch.type = (e.xmotion.state & Button1Mask) ? 1 : 0 | (false) ? 3 : 0 | (e.xmotion.state & Button2Mask) ? 2 : 0;
                ev.data.touch.x = e.xmotion.x;
                ev.data.touch.y = e.xmotion.y;
                ev.data.touch.idx = 0;
            }
            return true;
	    case KeymapNotify:
	        XRefreshKeyboardMapping(&e.xmapping);
            break;
        case KeyPress: 
            {
                char string[20];
	            KeySym keysym;
	            int len = XLookupString(&e.xkey, string, 20, &keysym, NULL);
	            if (len > 0) {
                    XEvent ev = {0};
                    ev.type = ClientMessage;
                    ev.xclient.window = x_window;
                    ev.xclient.message_type = m_msg_char;
                    ev.xclient.format = 8;
                    memcpy(ev.xclient.data.b, string, 20);
	        	    XSendEvent(x_display, x_window, false, 0, &ev);
	            }
            }
            //go to next
        case KeyRelease:
            {
                ev.type = (e.type == KeyPress) ? Event::IO_KEY_DOWN : Event::IO_KEY_UP;
            
                unsigned int mods;
                KeySym keySym;
                if (XkbLookupKeySym(e.xkey.display, e.xkey.keycode, 0, &mods, &keySym))
                {
                    ev.data.key.code = keySym;
                    ev.data.key.ctrl = (e.xkey.state & ShiftMask) ? 1 : ((e.xkey.state & ControlMask) ? 2 : ((e.xkey.state & Mod1Mask) ? 3 : 0));
                    return true;
                }
            }
            break;
        case ClientMessage:
            if (e.xclient.data.l[0] == m_msg_exit)
            {
                ev.type = Event::WM_CLOSE;
                return true;
            }
            else if(e.xclient.message_type == m_msg_char)
            {
                static char string[20];
                memcpy(string, e.xclient.data.b, 20);
                ev.type = Event::IO_TEXT_INPUT;
                ev.data.ptr  = (void*)string;
                return true;
            }
            break;
        }
    }
    return false;
}


Window* Window::inst()
{
    if(gptr)
    {
        return gptr;
    }
    else
    {
        gptr = new WindowLinux();
        return gptr;
    }

}





}
#endif
