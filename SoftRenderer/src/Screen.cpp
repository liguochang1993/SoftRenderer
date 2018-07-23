#include "Screen.h"

Screen::Screen(int w, int h, LPCTSTR title) {
	width = w;
	height = h;
	title = title;
}

Screen::~Screen() {}

HWND Screen::handle;
HDC Screen::dc;
HBITMAP Screen::hb;
HBITMAP Screen::ob;

int Screen::exit = 0;
int Screen::keys[512];

int Screen::init() {
	WNDCLASS wc = { CS_BYTEALIGNCLIENT, (WNDPROC)events, 0, 0, 0,
		NULL, NULL, NULL, NULL, LPCTSTR("SCREEN3.1415926") };
	BITMAPINFO bi = { { sizeof(BITMAPINFOHEADER), width, -height, 1, 32, BI_RGB,
		width * height * 4, 0, 0, 0, 0 } };
	RECT rect = { 0, 0, width, height };
	int wx, wy, sx, sy;
	LPVOID ptr;
	HDC hDC;

	close();

	wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wc.hInstance = GetModuleHandle(NULL);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	if (!RegisterClass(&wc)) return -1;

	handle = CreateWindow(LPCTSTR("SCREEN3.1415926"), title,
		WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
		0, 0, 0, 0, NULL, NULL, wc.hInstance, NULL);
	if (handle == NULL) return -2;

	exit = 0;
	hDC = GetDC(handle);
	dc = CreateCompatibleDC(hDC);
	ReleaseDC(handle, hDC);

	hb = CreateDIBSection(dc, &bi, DIB_RGB_COLORS, &ptr, 0, 0);
	if (hb == NULL) return -3;

	ob = (HBITMAP)SelectObject(dc, hb);
	fb = (unsigned char*)ptr;
	
	pitch = width * 4;

	AdjustWindowRect(&rect, GetWindowLong(handle, GWL_STYLE), 0);
	wx = rect.right - rect.left;
	wy = rect.bottom - rect.top;
	sx = (GetSystemMetrics(SM_CXSCREEN) - wx) / 2;
	sy = (GetSystemMetrics(SM_CYSCREEN) - wy) / 2;
	if (sy < 0) sy = 0;
	SetWindowPos(handle, NULL, sx, sy, wx, wy, (SWP_NOCOPYBITS | SWP_NOZORDER | SWP_SHOWWINDOW));
	SetForegroundWindow(handle);

	ShowWindow(handle, SW_NORMAL);
	dispatch();

	memset(keys, 0, sizeof(int) * 512);
	memset(fb, 0, width * height * 4);

	return 0;
}

int Screen::close() {
	if (dc) {
		if (ob) {
			SelectObject(dc, ob);
			ob = NULL;
		}
		DeleteDC(dc);
		dc = NULL;
	}
	if (hb) {
		DeleteObject(hb);
		hb = NULL;
	}
	if (handle) {
		CloseWindow(handle);
		handle = NULL;
	}
	return 0;
}

void Screen::dispatch() {
	MSG msg;
	while (1) {
		if (!PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE)) break;
		if (!GetMessage(&msg, NULL, 0, 0)) break;
		DispatchMessage(&msg);
	}
}

void Screen::update() {
	HDC hDC = GetDC(handle);
	BitBlt(hDC, 0, 0, width, height, dc, 0, 0, SRCCOPY);
	ReleaseDC(handle, hDC);
	dispatch();
}

LRESULT Screen::events(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	switch (msg) {
		case WM_CLOSE:
			exit = 1;
			break;
		case WM_KEYDOWN:
			keys[wParam & 511] = 1; 
			break;
		case WM_KEYUP: 
			keys[wParam & 511] = 0;
			break;
		default: 
			return DefWindowProc(hWnd, msg, wParam, lParam);
	}
	return 0;
}

int Screen::getExit() {
	return exit;
}

int* Screen::getKeys() {
	return keys;
}