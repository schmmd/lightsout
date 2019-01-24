#ifndef _H_VIEW
#define _H_VIEW

#include <windows.h>
#include "world.h"
#include "lib\other\bitmap.h"

class CView
{
public:
	CView();
	~CView();

	void Create	(HINSTANCE hInstance, CWorld * world);
	void Size	(int width, int height);
	void Update	(Canvas & canvas);
	void Paint	(Canvas & canvas);

	RECT _client, _title, _game;

	BYTE _bottomdisplays[6];

	void ShowHint(int x, int y)
	{
		_bHint = true;
		_xHint = x;
		_yHint = y;
	}
	void HideHint() { _bHint = false; }

private:
	Bitmap _bmpBack;

	Bitmap _bmpOn, _bmpOff, _bmpHint;

	HFONT _hfontTitle;
	HBRUSH  _hbrOnBar, _hbrOffBar;
	COLORREF _colorrefAlive, _colorrefDead;

	CWorld* _world;

	bool _bHint;
	int _xHint, _yHint;

	int _width, _height;
};

const int DISP_END			= 0;
const int DISP_GAMESTATE	= 1;
const int DISP_MOVES		= 2;
const int DISP_MOVESSTART	= 3;
const int DISP_MOVESHERE	= 4;
const int DISP_SCOREBAR		= 5;

const int SCOREBAR_HEIGHT	= 6;

#endif