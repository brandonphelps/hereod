#include <iostream>
#include <stdint.h>

#include "video.h"
#include "game_state.h"
#include "controller.h"

#ifdef _WIN32
#include "console_another.h"
#endif

// works fine on windows, but something about console doesn't allow writing to.
extern "C" int GameInit(GameState* game_state)
{
	char coolio[10];
	for(int i = 0; i < 10; i++)
	{
		coolio[i] = game_state->platformData[i];
	}

	game_state->module_data = new uint8_t[100];

	if(game_state->module_data == NULL)
	{
		return 1;
	}

	// uint16 offset X
	game_state->module_data[0] = 0;
	game_state->module_data[1] = 0;

	// uint16 offset Y
	game_state->module_data[2] = 0;
	game_state->module_data[3] = 0;

	return 0;
}

// some sort of buffer for video data is passed back and forth here.
extern "C" int GameUpdate(int dt, ScreenData* screenData, GameState* game_state, GameInputController* game_controller)
{
	if(game_controller != NULL && game_state != NULL)
	{
		if(game_controller->MoveRight.EndedDown)
		{
			if(game_state->module_data != NULL)
			{
				game_state->module_data[1]++;
			}
		}
		if(game_controller->MoveLeft.EndedDown)
		{
			if(game_state->module_data != NULL)
			{
				game_state->module_data[1]--;
			}
		}
	}

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
