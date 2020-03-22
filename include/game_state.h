#ifndef CUSTOM_GAMESTATE_H
#define CUSTOM_GAMESTATE_H

#include <stdint.h>

class GameState
{
	
public:
	uint8_t* platform_data;
	uint32_t platform_size;
	uint8_t* module_data;
	uint32_t module_size;
};

#endif
