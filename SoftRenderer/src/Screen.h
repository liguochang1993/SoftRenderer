#pragma once
#include <Windows.h>

#ifdef _MSC_VER
#pragma comment(lib, "gdi32.lib")
#pragma comment(lib, "user32.lib")
#endif

class Screen {

	static HWND handle;		// ������ HWND
	static HDC dc;			// ���׵� HDC
	static HBITMAP hb;		// DIB
	static HBITMAP ob;		// �ϵ� BITMAP

	static int exit;
	static int keys[512];	// ��ǰ���̰���״̬
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

	int init();			// ��Ļ��ʼ��
	int close();		// �ر���Ļ
	void dispatch();	// ������Ϣ
	void update();		// ��ʾ FrameBuffer

	int getExit();
	int* getKeys();
};