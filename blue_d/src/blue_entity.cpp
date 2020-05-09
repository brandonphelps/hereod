#include "game_input.h"
#include "blue_entity.h"
#include "blue_map.h"

#include <stdint.h>


extern PositionComponent* p_entities; 
extern HealthComponent*   h_entities; 	
extern uint8_t* w_entities;

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

int8_t get_y_heading(float current_pos_y, float new_pos_y)
{
	if(current_pos_y < new_pos_y)
	{
		return 1;
	}
	else
	{
		return -1;
	}
}

int8_t get_x_heading(float current_pos_x, float new_pos_x)
{
	if(current_pos_x < new_pos_x)
	{
		return 1;
	}
	else
	{
		return -1;
	}
}

void enemey_path_finding_update(float dt, Map* p, uint32_t* entityIds, uint32_t entity_size)
{
	if(entity_size == 1)
	{
		uint32_t entityId = *entityIds;

		uint8_t next_point_index = w_entities[entityId];

		// already at destination.
		if(next_point_index+1 > p->way_point_count)
		{
			return;
		}
		Point next_point = p->way_points[next_point_index+1];
		next_point.x = 100;
		next_point.y = 100;

		int8_t x_heading = get_x_heading(p_entities[entityId].x_pos, next_point.x);
		int8_t y_heading = get_y_heading(p_entities[entityId].y_pos, next_point.y);
	

		float sped = 2;
		p_entities[entityId].x_pos += (dt * sped * x_heading);
		p_entities[entityId].y_pos += (dt * sped * y_heading);
		
		if((p_entities[entityId].x_pos - next_point.x) < 1 &&
		   (p_entities[entityId].y_pos - next_point.y < 1))
		{
			w_entities[entityId]++;
		}
	}
}
