#include <windows.h>

#include "lib/app.h"
#include "procedure.h"
#include "resource.h"

#include "main.h"

// LRESULT CALLBACK CClass::Procedure (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	char * szClassName = "CWindow";
	char * szCaption   = "Lights Out";

	CClass windowclass (szClassName, hInstance);
	windowclass.SetMenu(IDR_MENU);
	windowclass.SetIcon(IDI_ICON);
	windowclass.Register();

	CProcedure proc;

	CWindow window(szCaption, szClassName, hInstance);
	window.SetStyle(WINDOW_STYLE);
	window.SetDimentions((TILE_WIDTH + 1) * BOARD_COLS + 1, TITLE_HEIGHT + (TILE_HEIGHT + 1) * BOARD_ROWS + 1, true);
	window.Create(&proc);
	window.AttachAccelerator(IDR_ACCELERATOR);
	window.Show();

	while(window.ProcessMessage())
	{

	}

	return 0;
}

int TILE_WIDTH  = IMAGE_WIDTH;
int TILE_HEIGHT = IMAGE_HEIGHT;

int BOARD_TILES = BOARD_COLS * BOARD_ROWS;

int BOARD_WIDTH  = BOARD_COLS * TILE_WIDTH;
int BOARD_HEIGHT = BOARD_ROWS * TILE_HEIGHT;

int BOARD_ROWS = 5;
int BOARD_COLS = 5;