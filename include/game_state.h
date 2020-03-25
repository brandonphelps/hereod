#ifndef CUSTOM_GAMESTATE_H
#define CUSTOM_GAMESTATE_H

#include <stdint.h>

class GameState
{
	
public:
	GameState(); 
	GameState(const GameState& other);
	~GameState();

	GameState& operator=(const GameState& other);
	bool operator!=(const GameState& other) const;
	bool operator==(const GameState& other) const;

	// de allocates everything.
	void clear();

public:
	uint8_t* platform_data;
	uint32_t platform_size;
	uint8_t* module_data;
	uint32_t module_size;
};

#endif
