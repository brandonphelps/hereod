#include <iostream>
#include <stdint.h>

#include "video.h"
#include "game_state.h"
#include "controller.h"

#ifdef _WIN32
#include "console_another.h"
#endif


void towerDraw(uint8_t* buffer, uint32_t buf_width, uint32_t buf_height, uint32_t pitch,
               uint16_t xOffset, uint16_t yOffset)
{

	uint8_t greenOffset = xOffset / 500;

	uint8_t* row = (uint8_t*)buffer;
	for(int y = 0; y < buf_height; ++y)
	{
		uint8_t* pixel = (uint8_t*)row;
		for(int x = 0; x < buf_width; ++x)
		{
			#ifdef _WIN32

			// blue
			*pixel = static_cast<uint8_t>(y);
			++pixel;

			// green
			*pixel = static_cast<uint8_t>(x) + greenOffset;
			++pixel;

			// red
			*pixel = 0;
			++pixel;

			// alpha 
			*pixel = 0xFF;
			++pixel;

			#else

			// red
			*pixel = (uint8_t)y;
			++pixel;

			// blue;
			*pixel = 0;
			++pixel;

			// green;
			*pixel = (uint8_t)x;
			++pixel;

			// alpha
			*pixel = 0xFF;
			++pixel;

			#endif

		}
		row += pitch;
	}

}

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
	uint16_t xOffset;
	if(game_controller != NULL && game_state != NULL)
	{
		uint16_t* tmp = reinterpret_cast<uint16_t*>(game_state->module_data);
		xOffset = *tmp;
		if(game_controller->MoveRight.EndedDown)
		{
			tmp[0]++;
		}
		if(game_controller->MoveLeft.EndedDown)
		{
			tmp[0]--;
		}
	}

	// update the video buffer data as provided.
	// the width and height, etc will be updated for you. 
	towerDraw(screenData->buffer, screenData->width,
	          screenData->height, screenData->pitch, xOffset, 0);

	return 0;
}

extern "C" int GameShutdown()
{
	return 0;
}
