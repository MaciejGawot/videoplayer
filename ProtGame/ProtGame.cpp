#include "stdafx.h"

#include "src/gameplay/CGameEngine.h"

#include <stdlib.h>
#include <assert.h>

int main(int argc, char *argv[]) 
{
	CGameEngine engine;
	engine.mainLoop();

	return 0;
}