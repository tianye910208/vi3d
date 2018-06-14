#include "vi3d.h"
#include "resource.h"

#define APP_W 800
#define APP_H 480

HWND hwnd = NULL;

int runflag = 1;
int actived = 1;

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


int win_init(const char* name, int w, int h) {
	HINSTANCE hinst = GetModuleHandle(NULL);

    WNDCLASS winclass = {0};
	winclass.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	//winclass.hIcon = LoadIcon(NULL, IDI_WINLOGO);
	winclass.hIcon = LoadIcon(hinst, (LPCTSTR)IDI_ICON);
	winclass.hCursor = LoadCursor(NULL, IDC_ARROW);
    winclass.lpfnWndProc = (WNDPROC)msg_proc;
	winclass.hInstance = hinst;
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

	hwnd = CreateWindow(name, name, winstyle, (screenW - w) / 2, (screenH - h) / 2, w, h, NULL, NULL, hinst, NULL);
	if (hwnd == NULL)
        return 2;
    
	ShowWindow(hwnd, TRUE);
	SetForegroundWindow(hwnd);
	SetFocus(hwnd);

	return 0;
}


int main(int argc, char *argv[]) {
	//init------------------------------------------
	int err = 0;
	if(err = win_init("vi3d", APP_W, APP_H)) return err;
	if(err = vi_app_init(NULL, hwnd, argc>1 ? argv[1] : "../../", argc>2 ? argv[2] : "../../usr/")) return err;
	if(err = vi_app_main()) return err;

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
			
			if(actived)
				vi_app_loop(dt);
			
			Sleep(10);
		}
	}

	//exit------------------------------------------
	vi_app_exit();

    return 0;
}









