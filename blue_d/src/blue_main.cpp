#include <iostream>
#include <stdint.h>

#include "video.h"
#include "blue_entity.h"

#include "grid_helpers.h"
#include "game_state.h"
#include "game_input.h"
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
	uint16_t x_pos;
	uint16_t y_pos;
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


class Tower
{
public:
	
private:
	
};

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

void towerDraw(ScreenData* screenData)
{
	for(int y = 0; y < screenData->height; ++y)
	{
		for(int x = 0; x < screenData->width; ++x)
		{
			uint32_t color = (0xFF << 24 | static_cast<uint8_t>(x) << 16 | static_cast<uint8_t>(y) << 8 | 0xFF);
			screenData->set_pixel_color(x, y, color);
		}
	}
}

extern PositionComponent p_entities[100];
extern HealthComponent   h_entities[100];


void DrawToon(ScreenData* screenData, PositionComponent* toon, uint32_t color_mask)
{
	DrawRectangle(screenData, static_cast<uint32_t>(toon->x_pos), static_cast<uint32_t>(toon->y_pos), 30, 30, color_mask);
}


EntityObj player;
EntityObj leaf1;

EntityObj ents[10]; // all the entities live here. 
uint32_t entityId = 0;

void add_leaf(uint32_t start_x, float start_y, float color_mask)
{
	EntityObj* current = ents + entityId;
	current->id = entityId;
	entityId++;
	p_entities[current->id].x_pos = start_x;
	p_entities[current->id].y_pos = start_y;
	p_entities[current->id].color_mask = color_mask;
	current->component_mask = 1;
}

// works fine on windows, but something about console doesn't allow writing to.
extern "C" int GameInit(GameState* game_state)
{
	game_state->module_data = new uint8_t[sizeof(Map) + sizeof(Point)];
	game_state->module_size = sizeof(Map) + sizeof(Point);

	Map* p = reinterpret_cast<Map*>(game_state->module_data);
	if(p != NULL)
	{
		p->width = 10;
		p->height = 10;

		std::memset(p->tile_info, 0, 100);
		std::memcpy(p->tile_info, currentmap, 100);
	}

	Point* toon = reinterpret_cast<Point*>((game_state->module_data)+sizeof(Map));

	// add_player
	player.id = 0;
	player.component_mask = 1;
	p_entities[player.id].x_pos = 40;
	p_entities[player.id].y_pos = 40;

	h_entities[player.id].amount = 10;
	entityId++;

	add_leaf(70, 0, 0x00BBFFFF);
	// leaf1.id = 2;

	
	// p_entities[leaf1.id].x_pos = 20;
	// p_entities[leaf1.id].y_pos = 20;



	return 0;
}

void wind_movement_update(float dt, EntityObj* start, uint32_t entity_size);
void player_move_update(float dt, GameInput* game_input, EntityObj* start, uint32_t entity_size);

bool enableWind = false;

// some sort of buffer for video data is passed back and forth here.
extern "C" int GameUpdate(ScreenData* screenData, GameState* game_state, GameInput* game_input)
{
	GameInputController* controller = &(game_input->keyboard);
	float dt = game_input->dtForFrame;

	player_move_update(dt, game_input, &player, 1);


	if(controller != NULL)
	{
		if(controller->MoveUp.EndedDown)
		{
			enableWind = true;
		}
	}

	if(enableWind)
	{
		EntityObj moved_objs[10];
		uint32_t move_count = 0;
		for(int i = 0; i < 10; i++)
		{
			if(ents[i].component_mask == 1)
			{
				// is this doing a copy by value?
				moved_objs[move_count] = ents[i];
				move_count++;
			}
		}
		// EntityObj moved_objs[2] = {player, ents[1]};
		// EntityObj* moved_objs[2];
		// moved_objs[0] = &player;
		// moved_objs[1] = &(ents[1]);
		wind_movement_update(dt, moved_objs, move_count);
	}

	Map* p = reinterpret_cast<Map*>(game_state->module_data);
	Point* toon = reinterpret_cast<Point*>((game_state->module_data)+sizeof(Map));

	towerDraw(screenData);
	DrawMap(screenData, p->tile_info);

	// for(int i = 1; i < 2; i++)
	// {
	// 	DrawToon(screenData, p_entities + ents[i].id, (p_entities + ents[i].id)->color_mask);
	// }
	DrawToon(screenData, p_entities + ents[1].id, (p_entities + ents[1].id)->color_mask);
	// DrawToon(screenData, p_entities + leaf1.id, 0xCCDD22FF);
	DrawToon(screenData, p_entities + player.id, 0xAAAA00FF);
	
	return 0;
}

extern "C" int GameShutdown()
{
	return 0;
}
