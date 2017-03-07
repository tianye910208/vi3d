#include <stdio.h>
#include <unistd.h>
#include <sys/time.h>

#include  <X11/Xlib.h>
#include  <X11/Xatom.h>
#include  <X11/Xutil.h>

#include <GLES2/gl2.h>
#include <EGL/egl.h>
#include <EGL/eglext.h>

#include "test.h"


EGLNativeDisplayType nativeDisplay = EGL_DEFAULT_DISPLAY;
EGLNativeWindowType  nativeWindow;

EGLDisplay eglDisplay;
EGLContext eglContext;
EGLSurface eglSurface;

Atom wmDeleteWindow;

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
 
    nativeDisplay = XOpenDisplay(NULL);
    if (nativeDisplay == NULL)
        return false;

    nativeWindow = XCreateWindow(
        nativeDisplay, 
        DefaultRootWindow(nativeDisplay),
        0, 0, w, h, 0,
        CopyFromParent, InputOutput,
        CopyFromParent, 0, 0);

    XStoreName(nativeDisplay, nativeWindow, title);

    XSelectInput(nativeDisplay, nativeWindow, ExposureMask | StructureNotifyMask | FocusChangeMask | VisibilityChangeMask | KeyPressMask | KeyReleaseMask | KeymapStateMask | ButtonPressMask | ButtonReleaseMask | PointerMotionMask );

    XFlush(nativeDisplay);

    XMapWindow(nativeDisplay, nativeWindow);

    wmDeleteWindow = XInternAtom(nativeDisplay, "WM_DELETE_WINDOW", False);
    XSetWMProtocols(nativeDisplay, nativeWindow, &wmDeleteWindow, 1);

    return true;
}

void win_exit()
{
    XDestroyWindow(nativeDisplay, nativeWindow);
    XCloseDisplay(nativeDisplay);
}

void win_loop()
{
    float dt;
    struct timeval t1, t2;
    struct timezone tz;

    gettimeofday(&t1, &tz);

    XEvent xev;
    while (true)
    {
    
        if(XPending(nativeDisplay))
        {
            XNextEvent(nativeDisplay, &xev);
            if (xev.type == ClientMessage && xev.xclient.data.l[0] == wmDeleteWindow)
                return;
        }
        else
        {
            gettimeofday(&t2, &tz);
            dt = (float)(t2.tv_sec - t1.tv_sec + (t2.tv_usec - t1.tv_usec) * 1e-6);
            t1 = t2;

            //update
            //render
            test_draw();

            eglSwapBuffers(eglDisplay, eglSurface);
            usleep(10000);
        }

    }
}


int main(int argc, char *argv[])
{

    if (win_init("vi3d", 800, 480) == false)
        return 1;

    if (egl_init() == false)
        return 1;

    //printf((const char*)glGetString(GL_EXTENSIONS));
    test_init();

    win_loop();

    egl_exit();

    win_exit();

    return 0;
}







