#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include "resource.h"
#include "vi3d.h"


#define APP_W 800
#define APP_H 480

int runflag = 1;
int actived = 1;

EGLNativeDisplayType nativeDisplay = EGL_DEFAULT_DISPLAY;
EGLNativeWindowType  nativeWindow = NULL;

EGLConfig  eglConfig;
EGLDisplay eglDisplay;
EGLContext eglContext;
EGLSurface eglSurface;

vi_msg* msg;
int     msgTouchDown = 0;
int     msgKeyDown[256] = {0};

LRESULT WINAPI msg_proc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    LRESULT  lRet = 1;
    switch (uMsg)
    {
    case WM_DESTROY:
		runflag = 0;
        PostQuitMessage(0);
        break;
	case WM_SIZE:
		vi_app_set_screen_size(LOWORD(lParam), HIWORD(lParam));
		break;
    case WM_KEYDOWN:
		if (msgKeyDown[wParam & 0xFF] == 0) {
			if (msg = vi_msg_push(VI_MSG_KEY_DOWN, 1)) {
				msg->data[0] = (int)wParam;
				msgKeyDown[wParam & 0xFF] = 1;
			}
		}
        break;
	case WM_KEYUP:
		if (msg = vi_msg_push(VI_MSG_KEY_UP, 1)) {
			msg->data[0] = (int)wParam;
			msgKeyDown[wParam & 0xFF] = 0;
		}
		break;
	case WM_LBUTTONDOWN:
		if (msg = vi_msg_push(VI_MSG_TOUCH_DOWN, 3)) {
			msgTouchDown = 1;
			msg->data[0] = LOWORD(lParam);
			msg->data[1] = HIWORD(lParam);
			msg->data[2] = 0;
		}
		break;
	case WM_LBUTTONUP:
		msgTouchDown = 0;
		if (msg = vi_msg_push(VI_MSG_TOUCH_UP, 3)) {
			msg->data[0] = LOWORD(lParam);
			msg->data[1] = HIWORD(lParam);
			msg->data[2] = 0;
		}
		break;
	case WM_MOUSEMOVE:
		if (msgTouchDown) {
			if (wParam & MK_LBUTTON) {
				msg = vi_msg_push(VI_MSG_TOUCH_MOVE, 3);
			}
			else {
				msgTouchDown = 0;
				msg = vi_msg_push(VI_MSG_TOUCH_UP, 3);
			}
			if (msg) {
				msg->data[0] = LOWORD(lParam);
				msg->data[1] = HIWORD(lParam);
				msg->data[2] = 0;
			}
		}
		break;
    default:
        lRet = DefWindowProc(hWnd, uMsg, wParam, lParam);
        break;
    }

    return lRet;
}


int egl_init(const char* name, int w, int h) {
    HINSTANCE hInstance = GetModuleHandle(NULL);

    WNDCLASS winclass = {0};
	winclass.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	//winclass.hIcon = LoadIcon(NULL, IDI_WINLOGO);
	winclass.hIcon = LoadIcon(hInstance, (LPCTSTR)IDI_ICON);
	winclass.hCursor = LoadCursor(NULL, IDC_ARROW);
    winclass.lpfnWndProc = (WNDPROC)msg_proc;
    winclass.hInstance = hInstance;
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

	w = winrect.right - winrect.left;
	h = winrect.bottom - winrect.top;

	int screenW = GetSystemMetrics(SM_CXSCREEN);
	int screenH = GetSystemMetrics(SM_CYSCREEN);

	nativeWindow = CreateWindow(name, name, winstyle, (screenW - w)/2, (screenH - h)/2, w, h, NULL, NULL, hInstance, NULL);
    if (nativeWindow == NULL)
        return 2;
    
    ShowWindow(nativeWindow, TRUE);
	SetForegroundWindow(nativeWindow);
	SetFocus(nativeWindow);

	EGLint configNum = 0;
	EGLint majorVersion;
	EGLint minorVersion;
	EGLint ctxAttribList[] = {
		EGL_CONTEXT_CLIENT_VERSION, 2,
		EGL_NONE
	};
	EGLint cfgAttribList[] = {
		EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
		EGL_RED_SIZE, 8,
		EGL_GREEN_SIZE, 8,
		EGL_BLUE_SIZE, 8,
		EGL_ALPHA_SIZE, 8,
		EGL_DEPTH_SIZE, 24,
		EGL_STENCIL_SIZE, 8,
		EGL_SAMPLE_BUFFERS, 0,
		EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT,
		EGL_NONE
	};


	eglDisplay = eglGetDisplay(nativeDisplay);

	if (eglDisplay == EGL_NO_DISPLAY || eglGetError() != EGL_SUCCESS)
		return 11;

	if (!eglInitialize(eglDisplay, &majorVersion, &minorVersion) || eglGetError() != EGL_SUCCESS)
		return 12;

	if (!eglChooseConfig(eglDisplay, cfgAttribList, &eglConfig, 1, &configNum) || configNum < 1)
		return 13;

	eglContext = eglCreateContext(eglDisplay, eglConfig, EGL_NO_CONTEXT, ctxAttribList);
	if (eglContext == EGL_NO_CONTEXT || eglGetError() != EGL_SUCCESS)
		return 14;

	eglSurface = eglCreateWindowSurface(eglDisplay, eglConfig, nativeWindow, NULL);
	if (eglSurface == EGL_NO_SURFACE || eglGetError() != EGL_SUCCESS)
		return 15;

	if (!eglMakeCurrent(eglDisplay, eglSurface, eglSurface, eglContext) || eglGetError() != EGL_SUCCESS)
		return 16;

	return 0;
}

void egl_exit() {
    eglMakeCurrent(eglDisplay, NULL, NULL, NULL);
    eglDestroyContext(eglDisplay, eglContext);
    eglDestroySurface(eglDisplay, eglSurface);
    eglTerminate(eglDisplay);
}

int main(int argc, char *argv[]) {
	//init------------------------------------------
	if (egl_init("vi3d", APP_W, APP_H) != 0)
        return 1;
	
	vi_app_init(argc>1?argv[1]:"../../", argc>2?argv[2]:"../../usr/");
	vi_app_main();

	//loop------------------------------------------
	float dt;
	DWORD t1, t2;
	t1 = GetTickCount();

	MSG msg = {0};
	while (runflag) {
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else {
			t2 = GetTickCount();
			dt = (float)(t2 - t1) / 1000.0f;
			t1 = t2;
			
			if(actived) {
				vi_app_loop(dt);
				eglSwapBuffers(eglDisplay, eglSurface);
			}
			Sleep(10);
		}
	}

	//exit------------------------------------------
	vi_app_exit();
    egl_exit();

    return 0;
}









