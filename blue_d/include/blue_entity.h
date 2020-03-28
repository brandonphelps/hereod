
#ifndef BLUE_ENTITY_CUS
#define BLUE_ENTITY_CUS

#include <stdint.h>

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
	float x_pos;
	float y_pos;
	uint32_t color_mask; // temporary
};

#endif
