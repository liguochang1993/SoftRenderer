#pragma once
#include <Windows.h>

#ifdef _MSC_VER
#pragma comment(lib, "gdi32.lib")
#pragma comment(lib, "user32.lib")
#endif

class Screen {

	static HWND handle;		// 主窗口 HWND
	static HDC dc;			// 配套的 HDC
	static HBITMAP hb;		// DIB
	static HBITMAP ob;		// 老的 BITMAP

	static int exit;
	static int keys[512];	// 当前键盘按下状态
private:
	int width = 0;
	int height = 0;
	LPCTSTR title;
	
	long pitch = 0;
	static LRESULT events(HWND, UINT, WPARAM, LPARAM);
public:
	Screen(int w, int h, LPCTSTR title);
	~Screen();

	unsigned char *fb;		// frame buffer

	int init();			// 屏幕初始化
	int close();		// 关闭屏幕
	void dispatch();	// 处理消息
	void update();		// 显示 FrameBuffer

	int getExit();
	int* getKeys();
};