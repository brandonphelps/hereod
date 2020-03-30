
#ifndef BLUE_ENTITY_CUS
#define BLUE_ENTITY_CUS

#include <stdint.h>
#include "game_input.h"
#include "blue_map.h"

typedef struct EntityObj
{
	uint32_t id;
	uint32_t component_mask; 
} EntityObj;

class HealthComponent
{
public:
	uint16_t amount;
};

class PositionComponent
{
public:
	float x_pos; // make this in Meters
	float y_pos; // make this in meters
	uint32_t color_mask; // temporary
};

// void wind_movement_update(float dt, EntityObj* start, uint32_t entity_size);
void wind_movement_update(float dt, uint32_t* entityIds, uint32_t entity_size);
void player_move_update(float dt, GameInput* game_input, EntityObj* start, uint32_t entity_size);

void enemey_path_finding_update(float dt, Map* p, uint32_t* entityIds, uint32_t entity_size);

#endif
