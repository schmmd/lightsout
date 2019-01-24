#include "procedure.h"
#include "dlgboardsize.h"
#include "dlgtilesize.h"
#include "lib\other\canvas.h"
#include "resource.h"
#include "main.h"

BOOL CALLBACK SimpleDlgProc(HWND hwnd, UINT message, UINT wParam, LPARAM lParam);

void CProcedure::Create(CREATESTRUCT* create)
{
	_world = new CWorld;
	_view.Create(create->hInstance, _world);

	_displayscore.Set(create->hMenu, ID_GAME_DISPLAY_SCORE);
	_displaybar.Set(create->hMenu, ID_GAME_DISPLAY_SCOREBAR);
	_displaymoves.Set(create->hMenu, ID_GAME_DISPLAY_MOVES);
	_displaymovesstart.Set(create->hMenu, ID_GAME_DISPLAY_MOVESSTART);
	_displaymoveshere.Set(create->hMenu, ID_GAME_DISPLAY_MOVESHERE);

	// _hcurWait   = LoadCursor(NULL, IDC_WAIT );
	// _hcurNormal = LoadCursor(NULL, IDC_ARROW);

	_timerhint.Create(_hwnd, 1);

	UpdateBottomDisplay();
	AdjustWindowSize();
}

bool CProcedure::Paint(HDC hdc)
{
	PaintCanvas canvas(_hwnd);
	_view.Paint(canvas);

	return true;
}

bool CProcedure::Size(int width, int height)
{
	_view.Size(width, height);
	UpdateAndPaint();

	return true;
}

bool CProcedure::LeftClick(int x, int y)
{
	if (_world->IsPlaying())
	{
		if (x > _view._game.left && y > _view._game.top && x < _view._game.right && y < _view._game.bottom)
		{
			int xBoard = (x - _view._game.left - 1) / (TILE_WIDTH  + 1);
			int yBoard = (y - _view._game.top  - 1) / (TILE_HEIGHT + 1);
			_world->Click(xBoard, yBoard);

			_view.HideHint();
			UpdateAndPaint();
		}
	}

	return true;
}

bool CProcedure::Timer (int id, TIMERPROC * proc)
{
	if (id == 1)
	{
		// Turn off hint
		_view.HideHint();
		UpdateAndPaint();
		_timerhint.Kill();

		return true;
	}

	return false;
}

bool CProcedure::MenuCommand (int cmdId, bool isAccel)
{
	bool bCheck;
	BoardSizeDlg boardsizedlg;
	TileSizeDlg tilesizedlg;
	int x,y;

	switch(cmdId)
	{
	case IDM_FILE_NEW:
		_world->New();
		UpdateAndPaint();
		return true;

	case ID_FILE_EXIT:
		Exit();
		return true;

	case ID_GAME_HINT:
		if (_world->IsPlaying())
		{
			_world->_game.Hint(x, y);
			_view.ShowHint(x, y);
			UpdateAndPaint();
			_timerhint.Set(1000);
		}
		return true;

	case ID_GAME_BOARDDIM:
		bCheck = boardsizedlg.Run(GetModuleHandle(NULL), _hwnd, BOARD_COLS, BOARD_ROWS);
		if (bCheck)
		{
			BOARD_COLS = boardsizedlg.GetWidth();
			BOARD_ROWS = boardsizedlg.GetHeight();
			AdjustWindowSize();
			_world->New();
		}
		UpdateAndPaint();
		return true;

	case ID_GAME_TILEDIM:
		tilesizedlg.Run(GetModuleHandle(NULL), _hwnd, _iTileSizeRatio1, _iTileSizeRatio2);
		_iTileSizeRatio1 = tilesizedlg.GetRatio1();
		_iTileSizeRatio2 = tilesizedlg.GetRatio2();
		SetTileSize((float)_iTileSizeRatio1 / (float)_iTileSizeRatio2);
		return true;

	case ID_GAME_DISPLAY_SCORE:
		bCheck = !(_displayscore.GetCheck());
		_displayscore.Check(bCheck);
		UpdateBottomDisplay();
		return true;

	case ID_GAME_DISPLAY_SCOREBAR:
		bCheck = !(_displaybar.GetCheck());
		_displaybar.Check(bCheck);
		UpdateBottomDisplay();
		return true;

	case ID_GAME_DISPLAY_MOVES:
		bCheck = !(_displaymoves.GetCheck());
		_displaymoves.Check(bCheck);
		UpdateBottomDisplay();
		return true;

	case ID_GAME_DISPLAY_MOVESSTART:
		bCheck = !(_displaymovesstart.GetCheck());
		_displaymovesstart.Check(bCheck);
		UpdateBottomDisplay();
		return true;

	case ID_GAME_DISPLAY_MOVESHERE:
		bCheck = !(_displaymoveshere.GetCheck());
		_displaymoveshere.Check(bCheck);
		UpdateBottomDisplay();
		return true;

	case ID_HELP_INSTRUCTIONS:
		DialogBox(GetModuleHandle(NULL), (char*)IDD_INSTRUCTIONS, _hwnd, DLGPROC(SimpleDlgProc));
		return true;

	case ID_HELP_SCHMITZTECH:
		ShellExecute(_hwnd, "Open", "http://www.schmitztech.com/", "", "", SW_SHOWNORMAL);
		return true;

	case ID_HELP_ABOUT:
		DialogBox(GetModuleHandle(NULL), (char*)IDD_ABOUT, _hwnd, DLGPROC(SimpleDlgProc));
		return true;

	default:
		return false;
	}
}

void CProcedure::UpdateAndPaint()
{
	UpdateCanvas canvas(_hwnd);
	_view.Update(canvas);
	InvalidateRect(_hwnd, NULL, 0);
}

void CProcedure::UpdateBottomDisplay()
{
	int i = 0;
	if (_displayscore.GetCheck() == true)
	{
		_view._bottomdisplays[i] = DISP_GAMESTATE;
		i++;
	}
	if (_displaymoves.GetCheck() == true)
	{
		_view._bottomdisplays[i] = DISP_MOVES;
		i++;
	}
	if (_displaymovesstart.GetCheck() == true)
	{
		_view._bottomdisplays[i] = DISP_MOVESSTART;
		i++;
	}
	if (_displaymoveshere.GetCheck() == true)
	{
		_view._bottomdisplays[i] = DISP_MOVESHERE;
		i++;
	}
	if (_displaybar.GetCheck() == true)
	{
		_view._bottomdisplays[i] = DISP_SCOREBAR;
		i++;
	}
	_view._bottomdisplays[i] = DISP_END;

	AdjustWindowSize();
}

void CProcedure::SetTileSize(float fl)
{
	TILE_WIDTH  = (int)(IMAGE_WIDTH  * fl);
	TILE_HEIGHT = (int)(IMAGE_HEIGHT * fl);
	BOARD_WIDTH  = BOARD_ROWS * TILE_WIDTH;
	BOARD_HEIGHT = BOARD_COLS * TILE_HEIGHT;

	BOARD_TILES = BOARD_COLS * BOARD_ROWS;

	AdjustWindowSize();
}

void CProcedure::AdjustWindowSize()
{
	RECT window = 
	{	
		0, 
		0, 
		(TILE_WIDTH + 1) * BOARD_COLS + 1, 
		TITLE_HEIGHT + (TILE_HEIGHT + 1) * BOARD_ROWS + 1
	};

	BOARD_WIDTH  = BOARD_COLS * TILE_WIDTH;
	BOARD_HEIGHT = BOARD_ROWS * TILE_HEIGHT;

	int i = 0, height = 0;
	while(_view._bottomdisplays[i] != DISP_END) 
	{ 
		i++; 
		if (_view._bottomdisplays[i] != DISP_SCOREBAR)
			height += 20;
		else
			height += SCOREBAR_HEIGHT;
	}
	window.bottom += height;

	if (window.right < WINDOW_MINWIDTH)
		window.right = WINDOW_MINWIDTH;
	AdjustWindowRect(&window, WINDOW_STYLE, true);
	SetWindowPos(_hwnd, NULL, 0, 0, window.right - window.left, window.bottom - window.top, SWP_NOZORDER | SWP_NOMOVE);
}

BOOL CALLBACK SimpleDlgProc
   (HWND hwnd, UINT message, UINT wParam, LPARAM lParam)
{
	RECT rect, rect2;

    switch (message)
    {
    case WM_INITDIALOG:
		// Center dialog
		GetWindowRect(GetParent(hwnd), &rect);
		GetWindowRect(hwnd , &rect2);
		SetWindowPos (hwnd, HWND_TOP, ((rect.left + rect.right) / 2) - ((rect2.right - rect2.left) / 2), ((rect.top + rect.bottom) / 2) - ((rect2.bottom - rect2.top) / 2), 0, 0, SWP_NOSIZE);
        return TRUE;
    case WM_COMMAND:
        switch (LOWORD(wParam))
        {
        case IDOK:
        case IDCANCEL:
            EndDialog(hwnd, 0);
            return TRUE;
        }
        break;
    }
    return FALSE;
}
