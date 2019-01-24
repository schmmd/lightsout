#ifndef _H_BOARDSIZE
#define _H_BOARDSIZE

#include <windows.h>

extern int g_iWidth;
extern int g_iHeight;

class BoardSizeDlg
{
public:
	bool Run(HINSTANCE hInstance, HWND hwnd, int width, int height);

	BYTE GetWidth() { return g_iWidth; }
	BYTE GetHeight() { return g_iWidth; }

private:
	static BOOL CALLBACK DialogProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
};

#endif