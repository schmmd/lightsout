#ifndef _H_WORLD
#define _H_WORLD

#include <windows.h>
#include "game.h"

class CWorld
{
public:
	CWorld();

	void New();

	void Click(int x, int y);

	CLightsGame _game;

	bool IsPlaying() { return !(_game.IsGameover()); }
};

#endif