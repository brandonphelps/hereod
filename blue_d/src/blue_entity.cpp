#include "blue_entity.h"
#include <stdint.h>

#include "game_input.h"

extern PositionComponent* p_entities; 
extern HealthComponent*   h_entities; 	

void player_move_update(float dt, GameInput* game_input, EntityObj* start, uint32_t entity_size)
{
	GameInputController* controller = &(game_input->keyboard);
	float move_speed = 3; // meters per second.

	PositionComponent* cur_pos = p_entities + start->id;
	float vel_x = 0, vel_y = 0;
	if(controller != NULL)
	{
		if(controller->MoveUp.EndedDown)
		{
			vel_y = -1 * move_speed;
		}

		if(controller->MoveDown.EndedDown)
		{
			vel_y = move_speed;
		}

		if(controller->MoveRight.EndedDown)
		{
			vel_x = move_speed;
		}
		if(controller->MoveLeft.EndedDown)
		{
			vel_x = -1 * move_speed;
		}
	}
	cur_pos->x_pos = dt * vel_x + cur_pos->x_pos;
	cur_pos->y_pos = dt * vel_y + cur_pos->y_pos;
}

void wind_movement_update(float dt, uint32_t* entityIds, uint32_t entity_size)
{
	float vel_x = -2;
	uint32_t* startIt = entityIds;
	for(uint32_t i = 0; i < entity_size; i++)
	{
		p_entities[*startIt].x_pos = dt * vel_x + p_entities[*startIt].x_pos ;
		startIt = startIt + 1;
	}
}

