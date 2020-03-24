
#pragma once

#include <stdint.h>

typedef struct EntityObj
{
	uint32_t id;
} EntityObj;



class HealthComponent
{
public:
	uint16_t amount;
};

class PositionComponent
{
public:
	float x_pos;
	float y_pos;
};
