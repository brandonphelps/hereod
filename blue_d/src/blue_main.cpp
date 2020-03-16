#include <iostream>
#include <stdint.h>

#include "video.h"
#include "game_state.h"
#include "game_controller.h"

#ifdef _WIN32
#include "console_another.h"
#endif

const uint8_t TileWidth = 30;
const uint8_t TileHeight = 30;


void DrawMap(uint8_t* buffer, uint32_t buf_width, uint32_t buf_height, uint8_t* tiles)
{
	for(int i = 0; i < 10; i++)
	{
		for(int j = 0; j < 10; j++)
		{
			if(tiles[i + j * 10] == 0)
			{
				DrawRectangle(buffer, buf_width,
				              i * TileWidth,
				              j * TileHeight,
				              TileWidth, TileHeight, 0xFF, 0xFF, 0xFF);
			}
			else
			{
				DrawRectangle(buffer, buf_width,
				              i * TileWidth,
				              j * TileHeight,
				              TileWidth, TileHeight,
				              0x00, 0x00, 0x00);
			}
		}
	}
}

void blueDraw(uint8_t* buffer, uint32_t buf_width, uint32_t buf_height, uint32_t pitch)
{

	uint8_t* row = (uint8_t*)buffer;
	for(int y = 0; y < buf_height; ++y)
	{
		uint8_t* pixel = (uint8_t*)row;
		for(int x = 0; x < buf_width; ++x)
		{
			#ifdef _WIN32

			// blue
			*pixel = 0;
			++pixel;

			// green
			*pixel = static_cast<uint8_t>(x);
			++pixel;

			// red
			*pixel = static_cast<uint8_t>(y);;
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

class Point
{
public:
	uint8_t x_pos;
	uint8_t y_pos;
};

class Map
{
public:
	// 10 by 10 grid
	uint8_t tile_info[100];
	uint8_t width;
	uint8_t height;



};


// works fine on windows, but something about console doesn't allow writing to.
extern "C" int GameInit(GameState* game_state)
{
	game_state->module_data = new uint8_t[sizeof(Map) + sizeof(Point)];

	Map* p = reinterpret_cast<Map*>(game_state->module_data);
	if(p != NULL)
	{
		p->width = 10;
		p->height = 10;

		for(int row = 0; row < p->height; row++)
		{
			for(int i = 0; i < p->width; i++)
			{
				p->tile_info[i] = 0;
			}
		}
		std::memset(p->tile_info, 0, 100); 
		p->tile_info[0] = 1;
		p->tile_info[1] = 1;

	}

	Point* toon = reinterpret_cast<Point*>((game_state->module_data)+sizeof(Map));

	toon->x_pos = 0;
	toon->y_pos = 0;
	
	return 0;
}

// some sort of buffer for video data is passed back and forth here.
extern "C" int GameUpdate(int dt, ScreenData* screenData, GameState* game_state, GameInputController* controller)
{

	Map* p = reinterpret_cast<Map*>(game_state->module_data);
	Point* toon = reinterpret_cast<Point*>((game_state->module_data)+sizeof(Map));

	// update the video buffer data as provided.
	// the width and height, etc will be updated for you. 
	blueDraw(screenData->buffer, screenData->width,
	         screenData->height, screenData->pitch);


	DrawMap(screenData->buffer, screenData->width, screenData->height, p->tile_info);


	return 0;
}

extern "C" int GameShutdown()
{
	return 0;
}
