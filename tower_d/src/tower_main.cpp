#include <iostream>
#include <stdint.h>

#include "video.h"
#include "game_state.h"

#ifdef _WIN32
#include "console_another.h"
#endif

// works fine on windows, but something about console doesn't allow writing to.
extern "C" int GameInit(GameState* game_state)
{
	return 0;
}

// some sort of buffer for video data is passed back and forth here.
extern "C" int GameUpdate(int dt, ScreenData* screenData, GameState* game_state)
{
	// update the video buffer data as provided.
	// the width and height, etc will be updated for you. 

	drawBuf(screenData->buffer, screenData->width,
	        screenData->height, screenData->pitch);

	return 0;
}

extern "C" int GameShutdown()
{
	return 0;
}
