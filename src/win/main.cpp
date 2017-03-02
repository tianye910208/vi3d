#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <stdio.h>

#include <GLES2/gl2.h>
#include <EGL/egl.h>
#include <EGL/eglext.h>


#pragma comment(lib, "libEGL.lib")
#pragma comment(lib, "libGLESv2.lib")


EGLNativeDisplayType nativedpy = NULL;
EGLNativeWindowType nativewin = NULL;

BOOL egl_init()
{

	EGLConfig config;
	EGLint majorVersion;
	EGLint minorVersion;
	EGLint contextAttribs[] = { EGL_CONTEXT_CLIENT_VERSION, 2, EGL_NONE };



	EGLDisplay display = eglGetDisplay(nativedpy);

	if (display == EGL_NO_DISPLAY)
		return FALSE;
	
	if (!eglInitialize(display, &majorVersion, &minorVersion))
		return FALSE;
	

   
	EGLint numConfigs = 0;
	EGLint attribList[] =
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

	// Choose config
	if (!eglChooseConfig(display, attribList, &config, 1, &numConfigs))
		return FALSE;
	  
	if (numConfigs < 1)
		return FALSE;
	   

    // Create a surface
	EGLSurface surface = eglCreateWindowSurface(display, config, nativewin, NULL);
    if (surface == EGL_NO_SURFACE)
		return FALSE;
   
    // Create a GL context
	EGLContext context = eglCreateContext(display, config, EGL_NO_CONTEXT, contextAttribs);
    if (context == EGL_NO_CONTEXT)
		return FALSE;
   
   // Make the context current
   if (!eglMakeCurrent(display, surface, surface, context))
	   return FALSE;

   return TRUE;
}



LRESULT WINAPI win_proc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	LRESULT  lRet = 1;

	switch (uMsg)
	{
	case WM_CREATE:
		break;
	case WM_PAINT:
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	case WM_CHAR:
		break;
	default:
		lRet = DefWindowProc(hWnd, uMsg, wParam, lParam);
		break;
	}

	return lRet;
}


BOOL win_init()
{
	HINSTANCE hInstance = GetModuleHandle(NULL);

	WNDCLASS winclass = {0};
	winclass.style = CS_OWNDC;
	winclass.lpfnWndProc = (WNDPROC)win_proc;
	winclass.hInstance = hInstance;
	winclass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	winclass.lpszClassName = "vi3d";

	if (!RegisterClass(&winclass))
		return FALSE;


	DWORD winstyle = WS_VISIBLE | WS_POPUP | WS_BORDER | WS_SYSMENU | WS_CAPTION;
	RECT winrect;
	winrect.left = 0;
	winrect.top = 0;
	winrect.right = 800;
	winrect.bottom = 480;
	AdjustWindowRect(&winrect, winstyle, FALSE);


	HWND hwnd = CreateWindow("vi3d","vi3d",winstyle,0,0,winrect.right - winrect.left,winrect.bottom - winrect.top,NULL,NULL,hInstance,NULL);
	if (hwnd == NULL)
		return FALSE;
	

	ShowWindow(hwnd, TRUE);


	nativewin = hwnd;

	return TRUE;
}


void win_loop()
{
	MSG msg = { 0 };
	DWORD lastTime = GetTickCount();

	while (true)
	{
		DWORD curTime = GetTickCount();
		float deltaTime = (float)(curTime - lastTime) / 1000.0f;
		lastTime = curTime;

		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
				return;
			
			TranslateMessage(&msg);
			DispatchMessage(&msg);
			
		}
		
		//update
		//render
	}
}


int main(int argc, char *argv[])
{
	if (win_init() == FALSE)
		return 1;

	if (egl_init() == FALSE)
		return 1;

	printf((const char*)glGetString(GL_EXTENSIONS));

	win_loop();
	
	return 0;
}
