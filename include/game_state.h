#ifndef CUSTOM_GAMESTATE_H
#define CUSTOM_GAMESTATE_H

#include <stdint.h>
#include "game_memory.h"

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
	uint64_t platform_size;
	uint8_t* module_data;
	uint64_t module_size;

public:
	MemorySection platform_mem;
	MemorySection module_mem;
};

#endif
