#include <iostream>
#include <stdint.h>

#include <vector>

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

std::vector<uint32_t> WindSystem;

void DrawToon(ScreenData* screenData, PositionComponent* toon, uint32_t color_mask)
{
	DrawRectangle(screenData,
	              static_cast<uint32_t>(toon->x_pos), static_cast<uint32_t>(toon->y_pos),
	              30, 30, color_mask);
}

EntityObj* ents; // all the entities live here. 
uint32_t entityId = 0;

void add_player(float start_x, float start_y, uint32_t color_mask)
{
	EntityObj* current = ents + entityId;
	current->id = entityId;
	entityId++;
	p_entities[current->id].x_pos = start_x;
	p_entities[current->id].y_pos = start_y;
	p_entities[current->id].color_mask = color_mask;
	h_entities[current->id].amount = 10;
	current->component_mask = 0;
	WindSystem.push_back(current->id);
}

void add_leaf(float start_x, float start_y, uint32_t color_mask)
{
	EntityObj* current = ents + entityId;
	current->id = entityId;
	entityId++;
	p_entities[current->id].x_pos = start_x;
	p_entities[current->id].y_pos = start_y;
	p_entities[current->id].color_mask = color_mask;
	current->component_mask = 1;
	WindSystem.push_back(current->id);
}

void add_tile(float start_x, float start_y, uint32_t color_mask)
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
	uint8_t* platform_mem = game_state->platform_mem.base;
	if(platform_mem[0] != 'W')
	{
		return 0;
	}

	

	uint32_t arena_size = sizeof(Map) + sizeof(Point) + sizeof(EntityObj) * 10;
	game_state->module_data = new uint8_t[arena_size];
	game_state->module_size = arena_size;
	
	Map* p = reinterpret_cast<Map*>(game_state->module_data);
	if(p != NULL)
	{
		p->width = 10;
		p->height = 10;

		std::memset(p->tile_info, 0, 100);
		std::memcpy(p->tile_info, currentmap, 100);
	}

	Point* toon = reinterpret_cast<Point*>((game_state->module_data)+sizeof(Map));

	ents = reinterpret_cast<EntityObj*>((game_state->module_data)+sizeof(Map)+sizeof(Point));

	// add_player
	add_player(40, 40, 0x000000FF);

	add_leaf(70, 0, 0x00BBFFFF);
	add_leaf(45, 24, 0x44CCFFFF);
	add_leaf(205, 154, 0x4411FBFF);
	// leaf1.id = 2;
	// p_entities[leaf1.id].x_pos = 20;
	// p_entities[leaf1.id].y_pos = 20;
	return 0;
}

// void wind_movement_update(float dt, EntityObj* start, uint32_t entity_size);
void wind_movement_update(float dt, uint32_t* entityIds, uint32_t entity_size);
void player_move_update(float dt, GameInput* game_input, EntityObj* start, uint32_t entity_size);

bool enableWind = false;

// some sort of buffer for video data is passed back and forth here.
extern "C" int GameUpdate(ScreenData* screenData, GameState* game_state, GameInput* game_input)
{
	GameInputController* controller = &(game_input->keyboard);
	float dt = game_input->dtForFrame;

	// first ent is the player.
	player_move_update(dt, game_input, ents, 1);

	if(controller != NULL)
	{
		if(controller->MoveUp.EndedDown)
		{
			enableWind = true;
		}
	}

	if(enableWind)
	{
		// EntityObj moved_objs[10];
		// uint32_t move_count = 0;
		// for(int i = 0; i < 10; i++)
		// {
		// 	if(ents[i].component_mask == 1)
		// 	{
		// 		// is this doing a copy by value?
		// 		//   pretty certain.
		// 		moved_objs[move_count] = ents[i];
		// 		move_count++;
		// 	}
		// }
		// wind_movement_update(dt, moved_objs, move_count);
		wind_movement_update(dt, WindSystem.data(), WindSystem.size());
	}

	Map* p = reinterpret_cast<Map*>(game_state->module_data);
	Point* toon = reinterpret_cast<Point*>((game_state->module_data)+sizeof(Map));

	towerDraw(screenData);
	DrawMap(screenData, p->tile_info);

	// skip the player since he is first index.
	for(int i = 1; i < entityId; i++)
	{
		DrawToon(screenData, p_entities + ents[i].id, (p_entities + ents[i].id)->color_mask);
	}
	// DrawToon(screenData, p_entities + ents[1].id, (p_entities + ents[1].id)->color_mask);
	// DrawToon(screenData, p_entities + leaf1.id, 0xCCDD22FF);
	DrawToon(screenData, p_entities + ents[0].id, 0xAAAA00FF);
	
	return 0;
}

extern "C" int GameShutdown()
{
	return 0;
}
