#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include "resource.h"
#include "vi3d.h"


#define APP_W 800
#define APP_H 480


EGLNativeDisplayType nativeDisplay = EGL_DEFAULT_DISPLAY;
EGLNativeWindowType  nativeWindow = NULL;

EGLConfig  eglConfig;
EGLDisplay eglDisplay;
EGLContext eglContext;
EGLSurface eglSurface;

int egl_init()
{
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
		EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
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
        return 1;
    
    if (!eglInitialize(eglDisplay, &majorVersion, &minorVersion) || eglGetError() != EGL_SUCCESS)
        return 2;

	if (!eglChooseConfig(eglDisplay, cfgAttribList, &eglConfig, 1, &configNum) || configNum < 1)
        return 3;
      
	eglContext = eglCreateContext(eglDisplay, eglConfig, EGL_NO_CONTEXT, ctxAttribList);
    if (eglContext == EGL_NO_CONTEXT || eglGetError() != EGL_SUCCESS)
        return 4;

	eglSurface = eglCreateWindowSurface(eglDisplay, eglConfig, nativeWindow, NULL);
    if (eglSurface == EGL_NO_SURFACE || eglGetError() != EGL_SUCCESS)
        return 5;

    if (!eglMakeCurrent(eglDisplay, eglSurface, eglSurface, eglContext) || eglGetError() != EGL_SUCCESS)
        return 6;

    return 0;
}

void egl_exit()
{
    eglMakeCurrent(eglDisplay, NULL, NULL, NULL);
    eglDestroyContext(eglDisplay, eglContext);
    eglDestroySurface(eglDisplay, eglSurface);
    eglTerminate(eglDisplay);
}


LRESULT WINAPI win_proc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    LRESULT  lRet = 1;

    switch (uMsg)
    {
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
	case WM_SIZE:
		vi_app_set_screen_size(LOWORD(lParam), HIWORD(lParam));
		break;
    case WM_CHAR:
        break;
    default:
        lRet = DefWindowProc(hWnd, uMsg, wParam, lParam);
        break;
    }

    return lRet;
}


int win_init(const char* name, int w, int h)
{
    HINSTANCE hInstance = GetModuleHandle(NULL);

    WNDCLASS winclass = {0};
    winclass.style = CS_OWNDC;
	winclass.hIcon = LoadIcon(hInstance, (LPCTSTR)IDI_ICON);
    winclass.lpfnWndProc = (WNDPROC)win_proc;
    winclass.hInstance = hInstance;
    winclass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
    winclass.lpszClassName = name;

    if (!RegisterClass(&winclass))
        return 1;


	DWORD winstyle = WS_VISIBLE | WS_POPUP | WS_BORDER | WS_SYSMENU | WS_CAPTION | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_THICKFRAME;
    RECT winrect;
    winrect.left = 0;
    winrect.top = 0;
    winrect.right = w;
    winrect.bottom = h;
    AdjustWindowRect(&winrect, winstyle, FALSE);


    nativeWindow = CreateWindow(name, name, winstyle, 0, 0, winrect.right - winrect.left, winrect.bottom - winrect.top, NULL, NULL, hInstance, NULL);
    if (nativeWindow == NULL)
        return 2;
    
    ShowWindow(nativeWindow, TRUE);

    return 0;
}

void win_exit()
{
    //when exit
}


void win_loop()
{
    float dt;
    DWORD t1, t2;
    t1 = GetTickCount();

    MSG msg = { 0 };
    while (1)
    {
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            if (msg.message == WM_QUIT)
                return;
            
            TranslateMessage(&msg);
            DispatchMessage(&msg);
            
        }
        else
        {
            t2 = GetTickCount();
            dt = (float)(t2 - t1) / 1000.0f;
            t1 = t2;

			vi_app_loop(dt);
			eglSwapBuffers(eglDisplay, eglSurface);
            Sleep(10);
        }
    }
}


int main(int argc, char *argv[])
{
	if (win_init("vi3d", APP_W, APP_H) != 0)
        return 1;
    if (egl_init() != 0)
        return 2;

	for (int i = 0; i < argc; i++)
		vi_log("[arg]%d %s", i, argv[i]);

	vi_app_init(argc>1?argv[1]:"../../", argc>2?argv[2]:"../../dat");
	vi_app_set_screen_size(APP_W, APP_H);
	
    win_loop();

	vi_app_exit();

    egl_exit();
    win_exit();
    return 0;
}









