#include <iostream>
#include <stdint.h>

#include "video.h"
#include "grid_helpers.h"
#include "game_state.h"
#include "game_controller.h"

#ifdef _WIN32
#include "console_another.h"
#endif

const uint8_t TileWidth = 30;
const uint8_t TileHeight = 30;
uint8_t pos = 3;
const uint16_t MapXOffset = 10;
const uint16_t MapYOffset = 10;

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

const uint8_t currentmap[10 * 10] = {
                                     1, 1, 1, 1, 1,  1, 1, 1, 1, 1, 
                                     1, 0, 0, 0, 0,  0, 0, 1, 1, 1, 
                                     1, 0, 1, 0, 0,  0, 0, 0, 0, 1, 
                                     1, 0, 1, 0, 0,  0, 0, 1, 1, 1, 
                                     1, 0, 1, 0, 0,  1, 0, 1, 1, 1, 
                                     
                                     1, 0, 1, 0, 0,  0, 0, 1, 1, 1, 
                                     1, 0, 1, 1, 1,  0, 0, 0, 0, 1, 
                                     1, 0, 1, 0, 0,  0, 0, 1, 1, 1, 
                                     1, 0, 0, 0, 0,  0, 0, 0, 0, 1, 
                                     1, 0, 0, 0, 0,  0, 0, 1, 1, 1, };


void DrawMap(ScreenData* screenData, uint8_t* tiles)
{
	// 
	//drawBuf(buffer, buf_width, buf_height, 1);
	for(int row = 0; row < 10; row++)
	{
		for(int col = 0; col < 10; col++)
		{
			if(tiles[col + (row * 10)] == 1)
			{
				DrawRectangle(screenData,
				              MapXOffset + (col * TileWidth), MapXOffset + (row * TileHeight),
				              TileWidth, TileHeight, 0xFF00FFFF);
			}
			else
			{
				DrawRectangle(screenData,
				              MapXOffset + (col * TileWidth),
				              MapYOffset + (row * TileHeight),
				              TileWidth, TileHeight, 0xFFFF00FF);
			}
		}
	}
}

void draw_circle_helper(ScreenData* screenData,
                        uint32_t center_x, uint32_t center_y,
                        uint32_t x, uint32_t y)
{
	screenData->set_pixel_color(center_x + x, center_y + y, 0xFF0000FF);
	screenData->set_pixel_color(center_x - x, center_y + y, 0xFF0000FF);
	screenData->set_pixel_color(center_x + x, center_y - y, 0xFF0000FF);
	screenData->set_pixel_color(center_x - x, center_y - y, 0xFF0000FF);

	screenData->set_pixel_color(center_x + y, center_y + x, 0xFF0000FF);
	screenData->set_pixel_color(center_x - y, center_y + x, 0xFF0000FF);
	screenData->set_pixel_color(center_x + y, center_y - x, 0xFF0000FF);
	screenData->set_pixel_color(center_x - y, center_y - x, 0xFF00FFFF);
}

void DrawCircle(ScreenData* screenData,
                uint32_t center_x, uint32_t center_y,
                uint32_t r)
{
	uint32_t x = 0;
	uint32_t y = r;
	uint32_t d = 3 - y * r;
	draw_circle_helper(screenData, center_x, center_y, x, y);
	while(y >= x)
	{
		x++;
		if(d > 0)
		{
			y--;
			d = d + 4 * (x - y) + 10;
		}
		else
		{
			d = d + 4 * x + 6;
		}
		draw_circle_helper(screenData, center_x, center_y, x, y);
	}
}

void DrawToon(ScreenData* screenData, Point* toon)
{
	DrawRectangle(screenData, toon->x_pos, toon->y_pos, 30, 30, 0xAAAA00FF);
}

// works fine on windows, but something about console doesn't allow writing to.
extern "C" int GameInit(GameState* game_state)
{
	game_state->module_data = new uint8_t[sizeof(Map) + sizeof(Point)];

	Map* p = reinterpret_cast<Map*>(game_state->module_data);
	if(p != NULL)
	{
		p->width = 10;
		p->height = 10;

		std::memset(p->tile_info, 0, 100);
		std::memcpy(p->tile_info, currentmap, 100);
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
	if(controller != NULL)
	{
		if(controller->MoveUp.EndedDown)
		{
			toon->y_pos--;
		}

		if(controller->MoveDown.EndedDown)
		{
			toon->y_pos++;
		}

		if(controller->MoveRight.EndedDown)
		{
			toon->x_pos++;
		}
		if(controller->MoveLeft.EndedDown)
		{
			toon->x_pos--;
		}
	}
	
	DrawMap(screenData, p->tile_info);
	DrawToon(screenData, toon);

	return 0;
}

extern "C" int GameShutdown()
{
	return 0;
}
