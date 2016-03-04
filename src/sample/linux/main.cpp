#if defined(__linux__) && !defined(ANDROID)
#include <X11/Xlib.h>
#include <X11/XKBlib.h>
#include <X11/Xutil.h>
#include <cstdlib>
#include <cstring>
#include <sys/time.h>
#include <unistd.h>

#include <iostream>
using namespace std;

Window      x_window;
Display*    x_display;

int         m_fullscreen = 0;
XEvent      m_fullscreenXEvent;

Atom        m_msg_char;
Atom        m_msg_exit;

bool        m_loop = true;


void window_init(const char* title, unsigned int width, unsigned int height)
{
    x_display = XOpenDisplay(NULL);
    x_window = XCreateWindow(x_display, DefaultRootWindow(x_display), 0, 0, width, height, 0, CopyFromParent, InputOutput, CopyFromParent, 0, 0);

    XStoreName(x_display, x_window, title);
    XChangeProperty(x_display, x_window, XInternAtom(x_display, "_NET_WM_NAME", False), XInternAtom(x_display, "UTF8_STRING", False), 8, PropModeReplace, (unsigned char *) title, strlen(title));

    XSelectInput(x_display, x_window, ExposureMask | StructureNotifyMask | FocusChangeMask | VisibilityChangeMask | KeyPressMask | KeyReleaseMask | KeymapStateMask | ButtonPressMask | ButtonReleaseMask | PointerMotionMask );
    XFlush(x_display);
    XMapWindow(x_display, x_window);

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


void window_fullscreen(int flag) 
{ 
    if (m_fullscreen == flag) 
        return; 
    m_fullscreen = flag;

    m_fullscreenXEvent.xclient.data.l[0] = flag; // Set the mWindow to fullscreen mode   1 fullscreen 0 windowed
    XSendEvent(x_display, DefaultRootWindow(x_display), False, SubstructureNotifyMask, &m_fullscreenXEvent);
}

void window_getsize(unsigned int &width, unsigned int &height) 
{
    XWindowAttributes attr;
    XGetWindowAttributes(x_display, x_window, &attr);

    width = attr.width;
    height = attr.height;
}



bool window_getevent(XEvent &e)
{
    //while(XCheckWindowEvent(x_display, x_window, ~0, &e))
    //    return true;

    if (XPending(x_display))
    {
        XNextEvent(x_display, &e);
        return true;
    }

    return false;
}

void window_exit()
{
    if (m_fullscreen)
        window_fullscreen(0);
    
    XDestroyWindow(x_display, x_window);
    XCloseDisplay(x_display);
}


bool window_handle(XEvent &e)
{
    switch (e.type)
    {
        case FocusIn:
            //Event::GOT_FOCUS;
            return true;
        case FocusOut:
            //Event::LOST_FOCUS;
            return true;
        case Expose:
            if (e.xexpose.count == 0)
            {
                //Event::REFRESH;
            }
            return true;
        case ClientMessage:
            if (e.xclient.data.l[0] == m_msg_exit)
            {
                ///Event::CLOSE;
                m_loop = false;
                return true;
            }
            else if(e.xclient.message_type == m_msg_char)
            {
                //Event::TEXT_INPUT;
                //char = e.xclient.data.b[0];
                return false;
            }
        case ConfigureNotify:
            //if (e.xconfigure.width != width || e.xconfigure.height != height || e.xconfigure.x != x || e.xconfigure.y != y)
            {
                //Event::RESIZE;
                //size = { e.xconfigure.width, e.xconfigure.height };
            }
            return true;
        case ButtonPress:
            //Event::MOUSE_DOWN;
            if (e.xbutton.button == Button1)
            {
                //{ Event::LEFT, e.xmotion.x, e.xmotion.y };
                return true;
            }
            if (e.xbutton.button == Button2)
            {
                //{ Event::RIGHT, e.xmotion.x, e.xmotion.y };
                return true;
            }
        case ButtonRelease:
            //Event::MOUSE_UP;
            if (e.xbutton.button == Button1)
            {
                //{ Event::LEFT, e.xmotion.x, e.xmotion.y };
                return true;
            }
            else if (e.xbutton.button == Button2)
            {
                //{ Event::RIGHT, e.xmotion.x, e.xmotion.y };
                return true;
            }
            else if (e.xbutton.button == Button4)
            {
                //Event::MOUSE_WHEEL;
                //{ 100, e.xmotion.x, e.xmotion.y };
                return true;
            }
            else if (e.xbutton.button == Button5)
            {
                //Event::MOUSE_WHEEL;
                //{ -100, e.xmotion.x, e.xmotion.y };
                return true;
            }
            break;
        case MotionNotify:
            //Event::MOUSE_MOVE;
            //{ 
            //    (e.xmotion.state & Button1Mask) ? Event::LEFT : Event::NONE | (false) ? Event::MIDDLE : Event::NONE | (e.xmotion.state & Button2Mask) ? Event::RIGHT : Event::NONE,
            //    e.xmotion.x, 
            //    e.xmotion.y 
            //};
            return true;
	    case KeymapNotify:
		    XRefreshKeyboardMapping(&e.xmapping);
		    return false;
        case KeyPress: {
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
            // TODO: Don't do the stuff below on a key-repeate...
        } // Intentional fall-through!
        case KeyRelease:{
            //(e.type == KeyPress) ? Event::KEY_DOWN : Event::KEY_UP;
        
            unsigned int mods;
            KeySym keySym;
            if (XkbLookupKeySym(e.xkey.display, e.xkey.keycode, 0, &mods, &keySym))
            {
                //Event::Key key = {
                //    (e.xkey.state & ShiftMask) ? Event::MODIFIER_SHIFT : Event::MODIFIER_NONE &
                //    (e.xkey.state & ControlMask) ? Event::MODIFIER_CTRL : Event::MODIFIER_NONE &
                //    (e.xkey.state & Mod1Mask) ? Event::MODIFIER_ALT : Event::MODIFIER_NONE,
                //    keySym};
                return true;
            }
        }break;
    }
    return false;
}




////////// APPLICATION ENTRY POINT ////////////////////////////////////////////

int main(int argc, char *argv[])
{

    window_init("vi3d", 800, 600);

    XEvent ev;
    while(m_loop)
    {
        while (window_getevent(ev))
            window_handle(ev);



    
    }

    window_exit();
}

#endif
