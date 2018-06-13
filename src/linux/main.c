#include <unistd.h>
#include <sys/time.h>
#include <X11/Xlib.h>
#include <X11/Xatom.h>
#include <X11/Xutil.h>

#include "vi3d.h"


#define APP_W 800
#define APP_H 480


Display nativeDisplay = NULL;
Window  nativeWindow = NULL;

Atom wmDeleteWindow;

int runflag = 1;
int actived = 1;

vi_msg* msg;
int     msgTouchDown = 0;

void msg_proc(XEvent* ev)
{
	switch (ev->type)
	{
    case ClientMessage:
        if(ev->xclient.data.l[0] == wmDeleteWindow)
            runflag = 0;
		break;
	case ConfigureNotify:
		vi_app_set_screen_size(ev->xconfigure.width, ev->xconfigure.height);
		break;
    case MotionNotify:  
        if (msgTouchDown) {
            if(msg = vi_msg_push(VI_MSG_TOUCH_MOVE, 3)) {
                msg->data[0] = ev->xmotion.x;
                msg->data[1] = ev->xmotion.y;
                msg->data[2] = 0;
            }
        }
        break;  
    case ButtonPress:  
        if (ev->xbutton.button == 1) {
            msgTouchDown = 1;
            if(msg = vi_msg_push(VI_MSG_TOUCH_DOWN, 3)) {
                msg->data[0] = ev->xmotion.x;
                msg->data[1] = ev->xmotion.y;
                msg->data[2] = 0;
            }
        }
        break;  
    case ButtonRelease:  
        if (ev->xbutton.button == 1) {
            msgTouchDown = 0;
            if(msg = vi_msg_push(VI_MSG_TOUCH_UP, 3)) {
                msg->data[0] = ev->xmotion.x;
                msg->data[1] = ev->xmotion.y;
                msg->data[2] = 0;
            }
        }
        break;  
    case KeyPress:
        if (msg = vi_msg_push(VI_MSG_KEY_DOWN, 1)) {
            KeySym sym = XLookupKeysym(&ev->xkey, 0);
            KeySym tmp;
            XConvertCase(sym, &tmp, &sym);
            msg->data[0] = sym & 0xFF;
        }
        break;
    case KeyRelease:
        if (XPending(nativeDisplay)) {
            XEvent nev;
            XPeekEvent(nativeDisplay, &nev);
            if (nev.type == KeyPress && nev.xkey.time == ev->xkey.time && nev.xkey.keycode == ev->xkey.keycode) {
                XNextEvent(nativeDisplay, &nev);
                break;
            }
        }
        if (msg = vi_msg_push(VI_MSG_KEY_UP, 1)) {
            KeySym sym = XLookupKeysym(&ev->xkey, 0);
            KeySym tmp;
            XConvertCase(sym, &tmp, &sym);
            msg->data[0] = sym & 0xFF;
        }
        break;
	default:
		break;
	}
}

int win_init(const char *title, int w, int h)
{
	//init win------------------------------------------
	nativeDisplay = XOpenDisplay(NULL);
	if (nativeDisplay == NULL)
		return 1;

	nativeWindow = XCreateWindow(nativeDisplay, DefaultRootWindow(nativeDisplay), 0, 0, w, h, 0, CopyFromParent, InputOutput, CopyFromParent, 0, 0);

	XStoreName(nativeDisplay, nativeWindow, title);
	XSelectInput(nativeDisplay, nativeWindow, ExposureMask | StructureNotifyMask | FocusChangeMask | VisibilityChangeMask | KeyPressMask | KeyReleaseMask | KeymapStateMask | ButtonPressMask | ButtonReleaseMask | PointerMotionMask);

	wmDeleteWindow = XInternAtom(nativeDisplay, "WM_DELETE_WINDOW", False);
	XSetWMProtocols(nativeDisplay, nativeWindow, &wmDeleteWindow, 1);

    //int screen = DefaultScreen(nativeDisplay);
    //int screenW = DisplayWidth(nativeDisplay, screen);
    //int screenH = DisplayHeight(nativeDisplay, screen);

	//XSizeHints* hints = XAllocSizeHints();
	//hints->flags |= (PMinSize | PMaxSize);
	//hints->min_width  = hints->max_width  = w;
	//hints->min_height = hints->max_height = h;
	//XSetWMNormalHints(nativeDisplay, nativeWindow, hints);
	//XFree(hints);

	XFlush(nativeDisplay);
	XMapWindow(nativeDisplay, nativeWindow);
    return 0;
}

void win_exit()
{
	XDestroyWindow(nativeDisplay, nativeWindow);
	XCloseDisplay(nativeDisplay);
}


int main(int argc, char *argv[])
{
	//init------------------------------------------
	if (win_init("vi3d", APP_W, APP_H) != 0)
		return 1;

	if (vi_gles_egl_init(nativeDisplay, nativeWindow) != 0)
		return 2;

	vi_app_init(argc>1 ? argv[1] : "../../", argc>2 ? argv[2] : "../../dat");
    vi_app_set_screen_size(APP_W, APP_H);
	vi_app_main();

	//loop------------------------------------------
    float dt;
	struct timeval t1, t2;
	gettimeofday(&t1, NULL);

	XEvent xev;
	while (runflag)
	{
		if (XPending(nativeDisplay))
		{
			XNextEvent(nativeDisplay, &xev);
			msg_proc(&xev);
		}
		else
		{
			gettimeofday(&t2, NULL);
			dt = (float)(t2.tv_sec - t1.tv_sec + (t2.tv_usec - t1.tv_usec) * 1e-6);
			t1 = t2;

            if (actived) {
			    vi_app_loop(dt);
				vi_gles_egl_swap();
            }
			usleep(10000);
		}

	}


	//eixt------------------------------------------
    vi_app_exit();
    win_exit();

    return 0;
}







