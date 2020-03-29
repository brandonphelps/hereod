#include <iostream>
#include <stdint.h>

#include <vector>

#include "video.h"
#include "blue_entity.h"

#include "grid_helpers.h"
#include "game_state.h"
#include "game_input.h"
#include "game_controller.h"
#include "game_memory.h"

#ifdef _WIN32
#include "console_another.h"
#endif

const uint8_t TileWidthGraphic = 3;
const uint8_t TileHeightGraphic = 3;
const uint16_t MapXOffset = 1; // in Pixels
const uint16_t MapYOffset = 1; // in Pixels

const uint8_t MetersToPixel = 30; // 10 pixel per meter
const uint8_t TileWidthMeter = 30;
const uint8_t TileHeightMeter = 30;

class Map
{
public:
	// 10 by 10 grid
	uint8_t tile_info[100];
	uint8_t width;
	uint8_t height;
};

const uint8_t currentmap[10 * 10] = {1, 1, 1, 1, 1,  1, 1, 1, 1, 1, 
                                     1, 0, 0, 0, 0,  0, 0, 1, 1, 1, 
                                     1, 0, 1, 0, 0,  0, 0, 0, 0, 1, 
                                     1, 0, 1, 0, 0,  0, 0, 1, 1, 1, 
                                     1, 0, 1, 0, 0,  1, 0, 1, 1, 1, 
                                     
                                     1, 0, 1, 0, 0,  0, 0, 1, 1, 1, 
                                     1, 0, 1, 1, 1,  0, 0, 0, 0, 1, 
                                     1, 0, 1, 0, 0,  0, 0, 1, 1, 1, 
                                     1, 0, 0, 0, 0,  0, 0, 0, 0, 1, 
                                     1, 0, 0, 0, 0,  0, 0, 1, 1, 1};


class Tower
{
public:
	
private:
	
};




void DrawMap(ScreenData* screenData, uint8_t* tiles)
{
	uint32_t color_mask;
	for(int row = 0; row < 10; row++)
	{
		for(int col = 0; col < 10; col++)
		{
			if(tiles[col + (row * 10)] == 1)
			{
				color_mask = 0xFF00FFFF;
			}
			else
			{
				color_mask = 0xFFFF00FF;
			}

			DrawRectangle(screenData,
			              MapXOffset + col * TileWidthGraphic * MetersToPixel,
			              MapXOffset + row * TileHeightGraphic * MetersToPixel,
			              TileWidthGraphic * MetersToPixel, TileHeightGraphic * MetersToPixel, color_mask);
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

void DrawToon(ScreenData* screenData, PositionComponent* toon, uint32_t color_mask)
{
	DrawRectangle(screenData,
	              MapXOffset + static_cast<uint32_t>(toon->x_pos*MetersToPixel),
	              MapYOffset + static_cast<uint32_t>(toon->y_pos*MetersToPixel),
	              30, 30, color_mask);
}



EntityObj* ents; // all the entities live here. 
uint32_t entityId = 0;
PositionComponent* p_entities;
HealthComponent* h_entities;
bool enableWind = false;
std::vector<uint32_t> WindSystem;

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

	init_memory_section(game_state->module_mem, 10000);

	Map* p = AllocObj(game_state->module_mem, Map);
	if(p != NULL)
	{
		p->width = 10;
		p->height = 10;

		std::memset(p->tile_info, 0, 100);
		std::memcpy(p->tile_info, currentmap, 100);
	}

	ents = AllocArray(game_state->module_mem, EntityObj, 100);
	p_entities = AllocArray(game_state->module_mem, PositionComponent, 100);
	h_entities = AllocArray(game_state->module_mem, HealthComponent, 100);

	std::memset(p_entities, 0, 100);

	// add_player
	add_player(0, 0, 0x000000FF);

	add_leaf(70, 0, 0x00BBFFFF);
	add_leaf(45, 24, 0x44CCFFFF);
	add_leaf(205, 154, 0x4411FBFF);

	return 0;
}


// some sort of buffer for video data is passed back and forth here.
extern "C" int GameUpdate(ScreenData* screenData, GameState* game_state, GameInput* game_input)
{
	GameInputController* controller = &(game_input->keyboard);
	float dt = game_input->dtForFrame;

	Map* p = reinterpret_cast<Map*>(game_state->module_mem.base);


	if(controller != NULL)
	{
		if(controller->MoveUp.EndedDown)
		{
			enableWind = true;
		}
	}

	// first ent is the player.
	player_move_update(dt, game_input, ents, 1);

	// if(enableWind)
	// {
	// 	wind_movement_update(dt, WindSystem.data(), WindSystem.size());
	// }

	towerDraw(screenData);

	DrawMap(screenData, p->tile_info);

	// skip the player since he is first index.
	for(int i = 1; i < entityId; i++)
	{
		DrawToon(screenData, p_entities + ents[i].id, (p_entities + ents[i].id)->color_mask);
	}

	DrawToon(screenData, p_entities + ents[0].id, 0xAAAA00FF);
	
	return 0;
}

extern "C" int GameShutdown()
{
	return 0;
}
