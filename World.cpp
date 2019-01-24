#include "world.h"
#include "main.h"

#include <assert.h>
#include <stdio.h>

CWorld::CWorld()
{
	New();
}

void CWorld::New()
{
	// Create boards
	_game.Create(BOARD_COLS, BOARD_ROWS);
	_game.New();
}

void CWorld::Click(int x, int y)
{
	_game.Move(x, y);
}
