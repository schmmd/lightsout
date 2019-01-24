#ifndef _H_TILESIZE
#define _H_TILESIZE

#include <windows.h>

extern int g_iRatio1;
extern int g_iRatio2;

class TileSizeDlg
{
public:
	bool Run(HINSTANCE hInstance, HWND hwnd, int iRatio1, int iRatio2);

	int GetRatio1() { return g_iRatio1; }
	int GetRatio2() { return g_iRatio2; }

private:
	static BOOL CALLBACK DialogProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
};

#endif