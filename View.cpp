#include "view.h"
#include "lib\other\canvas.h"
#include "main.h"
#include "resource.h"

#include <stdio.h>

CView::CView() : _bHint(0)
{
	_bottomdisplays[0] = DISP_GAMESTATE;
	_bottomdisplays[1] = DISP_SCOREBAR;
	_bottomdisplays[2] = DISP_END;
}

CView::~CView()
{

}

void CView::Create	(HINSTANCE hInstance, CWorld * world)
{
	_world = world;

	_bmpOn.Load (hInstance, IDB_ON);
	_bmpOff.Load(hInstance, IDB_OFF);
	_bmpHint.Load(hInstance, IDB_HINT);

	// Create fonts
	_hfontTitle  = CreateFont(16, 0, 0, 0, 1000, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_CHARACTER_PRECIS, CLIP_DEFAULT_PRECIS, PROOF_QUALITY, FF_DONTCARE, "Ariel");

	// Create colors
	_colorrefAlive = RGB(000, 000, 000);
	_colorrefDead  = RGB(150, 150, 150);

	// Create brushes
	_hbrOnBar  = CreateSolidBrush(RGB(000, 175, 190));
	_hbrOffBar = CreateSolidBrush(RGB(000, 065, 071));
}

void CView::Size	(int width, int height)
{
	_width  = width;
	_height = height;

	_client.left   = 0;
	_client.right  = width;
	_client.top    = 0;
	_client.bottom = height;

	_game.left   = _client.left;
	_game.top    = _client.top + TITLE_HEIGHT;
	_game.right  = _client.right;
	_game.bottom = _game.top + BOARD_HEIGHT + BOARD_ROWS;

	int difference = _client.right - ((TILE_WIDTH + 1) * BOARD_COLS + 1);
	if (difference > 0)
	{
		difference  /= 2;
		_game.left  += difference;
		_game.right -= difference;
	}

	_title.left   = _client.left;
	_title.top    = _client.top;
	_title.right  = _client.right;
	_title.bottom = _title.top + TITLE_HEIGHT;
}

void CView::Update	(Canvas & canvas)
{
	if (_width == 0 || _height == 0)
		return;

    Bitmap bmp (canvas, _width, _height);
    BitmapCanvas bmpCanvas (canvas, bmp);

	bmpCanvas.WhiteWash(_client);

	int i;
	int x, y;
	int cx, cy;
	int width;
	char szBuffer[64];

	bmpCanvas.GetTextSize(cx, cy);

	// Draw title
	bmpCanvas.SelectObject((void*)_hfontTitle);
	bmpCanvas.SelectObject((void*)_colorrefAlive);

	sprintf(szBuffer, "Lights Out");
	width = canvas.GetTextWidth(szBuffer);
	bmpCanvas.Text((_client.right - width) / 2, 5, szBuffer, strlen(szBuffer));

	// Draw Grid
	for (x = _game.left; x <= _game.right; x += TILE_WIDTH + 1)
	{
		bmpCanvas.Line(x, _game.top, x, _game.bottom);
	}
	for (y = _game.top; y <= _game.bottom; y += TILE_HEIGHT + 1)
	{
		bmpCanvas.Line(_game.left, y, _game.right, y);
	}

	// Draw Board
	Blitter blitOn (_bmpOn);
	Blitter blitOff(_bmpOff);
	blitOn.SetDstArea(TILE_WIDTH, TILE_HEIGHT);
	blitOff.SetDstArea(TILE_WIDTH, TILE_HEIGHT);

	for (x = 0; x < BOARD_COLS; x++)
	{
		for (y = 0; y < BOARD_ROWS; y++)
		{
			bool tile = _world->_game.Get(x, y);

			int xBlt = x * (TILE_WIDTH  + 1) + 1 + _game.left;
			int yBlt = y * (TILE_HEIGHT + 1) + 1 + _game.top;
			if (tile == TILE_ON)
			{
				blitOn.SetDest(xBlt, yBlt);
				blitOn.BlitTo(bmpCanvas);
			}
			else if (tile == TILE_OFF)
			{
				blitOff.SetDest(xBlt, yBlt);
				blitOff.BlitTo(bmpCanvas);
			}	
		}
	}

	// Draw hint
	if (_bHint)
	{
		Blitter blitHint(_bmpHint);
		blitHint.SetDstArea(TILE_WIDTH, TILE_HEIGHT);

		int xBlt = _xHint * (TILE_WIDTH  + 1) + 1 + _game.left;
		int yBlt = _yHint * (TILE_HEIGHT + 1) + 1 + _game.top;
		blitHint.SetDest(xBlt, yBlt);
		blitHint.BlitTo(bmpCanvas);
	}

	// Draw bottom displays
	i = 0;
	y = _game.bottom + 3;

	if (_bottomdisplays[i] == DISP_GAMESTATE)
	{
		x = 0;

		sprintf(szBuffer, "Blue: %.2i           Red: %.2i", _world->_game.NumberOn(), _world->_game.NumberOff());
		width = bmpCanvas.GetTextWidth(szBuffer);
		bmpCanvas.Text((_client.right - width) / 2, y, szBuffer, strlen(szBuffer));

		i++;
		y += 20;
	}
	if (_bottomdisplays[i] == DISP_MOVES)
	{
		x = 0;

		sprintf(szBuffer, "Moves: %i", _world->_game.GetMoves());
		width = bmpCanvas.GetTextWidth(szBuffer);
		bmpCanvas.Text((_client.right - width) / 2, y, szBuffer, strlen(szBuffer));

		i++;
		y += 20;
	}
	if (_bottomdisplays[i] == DISP_MOVESSTART)
	{
		x = 0;

		if (!_world->IsPlaying() && _world->_game.GetMoves() == _world->_game.GetPerfect())
			sprintf(szBuffer, "Perfect Game!", _world->_game.GetPerfect());
		else
			sprintf(szBuffer, "Perfect game moves: %i", _world->_game.GetPerfect());
		width = bmpCanvas.GetTextWidth(szBuffer);
		bmpCanvas.Text((_client.right - width) / 2, y, szBuffer, strlen(szBuffer));

		i++;
		y += 20;
	}
	if (_bottomdisplays[i] == DISP_MOVESHERE)
	{
		x = 0;

		sprintf(szBuffer, "Moves to win: %i", _world->_game.GetMovesLeft());
		width = bmpCanvas.GetTextWidth(szBuffer);
		bmpCanvas.Text((_client.right - width) / 2, y, szBuffer, strlen(szBuffer));

		i++;
		y += 20;
	}
	if (_bottomdisplays[i] == DISP_SCOREBAR)
	{
		y -= 2;

		int iBlue = _world->_game.NumberOn();
		int iTotal = _world->_game.Width() * _world->_game.Height();

		// bmpCanvas.SelectObject((void*)_hpenBlueBar);
		RECT rect;

		// Set dimentions
		rect.left   = 0;
		rect.top    = y;
		rect.right  = (int)(_client.right * ((float)iBlue / (float)iTotal));
		rect.bottom = rect.top + SCOREBAR_HEIGHT;

		// Paint
		bmpCanvas.FillRect(&rect, _hbrOnBar);

		// Adjust dimentions
		rect.left  = rect.right;
		rect.right = _client.right;

		// Paint
		bmpCanvas.FillRect(&rect, _hbrOffBar);
	}

	_bmpBack = bmp;
}

void CView::Paint	(Canvas & canvas)
{
	if (_width == 0 || _height == 0)
		return;

	Blitter bltBack(_bmpBack);
	bltBack.BlitTo (canvas);
}