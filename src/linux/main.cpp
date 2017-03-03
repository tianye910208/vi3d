#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <sys/time.h>

#include  <X11/Xlib.h>
#include  <X11/Xatom.h>
#include  <X11/Xutil.h>

#include <GLES2/gl2.h>
#include <EGL/egl.h>
#include <EGL/eglext.h>


// X11 related local variables
static Display *x_display = NULL;

EGLNativeDisplayType nativeDisplay = EGL_DEFAULT_DISPLAY;
EGLNativeWindowType  nativeWindow;

EGLDisplay eglDisplay;
EGLContext eglContext;
EGLSurface eglSurface;

bool egl_init()
{

    EGLConfig config;
    EGLint configNum = 0;
    EGLint majorVersion;
    EGLint minorVersion;
    EGLint ctxAttribList[] =
    {
        EGL_CONTEXT_CLIENT_VERSION, 2,
        EGL_NONE
    };
    EGLint cfgAttribList[] =
    {
        EGL_RED_SIZE, 5,
        EGL_GREEN_SIZE, 6,
        EGL_BLUE_SIZE, 5,
        EGL_ALPHA_SIZE, EGL_DONT_CARE,
        EGL_DEPTH_SIZE, EGL_DONT_CARE,
        EGL_STENCIL_SIZE, EGL_DONT_CARE,
        EGL_SAMPLE_BUFFERS, 0,
        EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT,
        EGL_NONE
    };


    eglDisplay = eglGetDisplay(nativeDisplay);

    if (eglDisplay == EGL_NO_DISPLAY || eglGetError() != EGL_SUCCESS)
        return false;

    if (!eglInitialize(eglDisplay, &majorVersion, &minorVersion) || eglGetError() != EGL_SUCCESS)
        return false;

    if (!eglChooseConfig(eglDisplay, cfgAttribList, &config, 1, &configNum) || configNum < 1)
        return false;

    eglContext = eglCreateContext(eglDisplay, config, EGL_NO_CONTEXT, ctxAttribList);
    if (eglContext == EGL_NO_CONTEXT || eglGetError() != EGL_SUCCESS)
        return false;

    eglSurface = eglCreateWindowSurface(eglDisplay, config, nativeWindow, NULL);
    if (eglSurface == EGL_NO_SURFACE || eglGetError() != EGL_SUCCESS)
        return false;

    // Make the context current
    if (!eglMakeCurrent(eglDisplay, eglSurface, eglSurface, eglContext) || eglGetError() != EGL_SUCCESS)
        return false;

    return true;
}

void egl_exit()
{
    eglMakeCurrent(eglDisplay, NULL, NULL, NULL);
    eglDestroyContext(eglDisplay, eglContext);
    eglDestroySurface(eglDisplay, eglSurface);
    eglTerminate(eglDisplay);
}


bool win_init(const char *title, int w, int h)
{
 
    Window root;
    XSetWindowAttributes swa;
    XSetWindowAttributes  xattr;
    Atom wm_state;
    XWMHints hints;
    XEvent xev;
    EGLConfig ecfg;
    EGLint num_config;
    Window win;

    /*
    * X11 native display initialization
    */

    x_display = XOpenDisplay(NULL);
    if (x_display == NULL)
    {
        return false;
    }

    root = DefaultRootWindow(x_display);

    swa.event_mask = ExposureMask | PointerMotionMask | KeyPressMask;
    win = XCreateWindow(
        x_display, root,
        0, 0, w, h, 0,
        CopyFromParent, InputOutput,
        CopyFromParent, CWEventMask,
        &swa);

    xattr.override_redirect = false;
    XChangeWindowAttributes(x_display, win, CWOverrideRedirect, &xattr);

    hints.input = true;
    hints.flags = InputHint;
    XSetWMHints(x_display, win, &hints);

    // make the window visible on the screen
        XMapWindow(x_display, win);
    XStoreName(x_display, win, title);

    // get identifiers for the provided atom name strings
    wm_state = XInternAtom(x_display, "_NET_WM_STATE", false);

    memset(&xev, 0, sizeof(xev));
    xev.type = ClientMessage;
    xev.xclient.window = win;
    xev.xclient.message_type = wm_state;
    xev.xclient.format = 32;
    xev.xclient.data.l[0] = 1;
    xev.xclient.data.l[1] = false;
    XSendEvent(
        x_display,
        DefaultRootWindow(x_display),
        false,
        SubstructureNotifyMask,
        &xev);

    nativeWindow = (EGLNativeWindowType)win;
    nativeDisplay = (EGLNativeDisplayType)x_display;
    return true;
}

///
//  userInterrupt()
//
//      Reads from X11 event loop and interrupt program if there is a keypress, or
//      window close action.
//
bool userInterrupt()
{
    XEvent xev;
    KeySym key;

    bool userinterrupt = false;
    char text;

    // Pump all messages from X server. Keypresses are directed to keyfunc (if defined)
    while (XPending(x_display))
    {
        XNextEvent(x_display, &xev);
        if (xev.type == KeyPress)
        {
            if (XLookupString(&xev.xkey, &text, 1, &key, 0) == 1)
            {
                //prinrf(text);
            }
        }
        if (xev.type == DestroyNotify)
            userinterrupt = true;
    }
    return userinterrupt;
}

///
//  WinLoop()
//
//      Start main windows loop
//
void win_loop()
{
    struct timeval t1, t2;
    struct timezone tz;
    float deltatime;

    gettimeofday(&t1, &tz);

    while (userInterrupt() == false)
    {
        gettimeofday(&t2, &tz);
        deltatime = (float)(t2.tv_sec - t1.tv_sec + (t2.tv_usec - t1.tv_usec) * 1e-6);
        t1 = t2;

        //update
        //render

        eglSwapBuffers(eglDisplay, eglSurface);
    }
}


int main(int argc, char *argv[])
{

    if (win_init("vi3d", 800, 480) == false)
        return 1;

    if (egl_init() == false)
        return 1;

    printf((const char*)glGetString(GL_EXTENSIONS));

    win_loop();

    egl_exit();

    return 0;
}
