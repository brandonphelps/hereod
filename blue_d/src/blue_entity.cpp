#include "blue_entity.h"
#include <stdint.h>

#include "game_input.h"

PositionComponent p_entities[100]; 
HealthComponent   h_entities[100]; 	

void player_move_update(float dt, GameInput* game_input, EntityObj* start, uint32_t entity_size)
{
	GameInputController* controller = &(game_input->keyboard);
	int move_speed = 30;

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

void wind_movement_update(float dt, EntityObj* start, uint32_t entity_size)
{
	EntityObj* iter = start;
	float vel_x = -15;
	for(uint32_t i = 0; i < entity_size; i++)
	{
		p_entities[iter->id].x_pos = dt * vel_x + p_entities[iter->id].x_pos ;
		iter = iter+1;
	}
}
