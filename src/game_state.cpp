#include <string> // std::memcpy
#include "game_state.h"

#include "console_another.h"

GameState::GameState()
{
	platform_data = NULL;
	platform_size = 0;	

	module_data = NULL;
	module_size = 0;	

}

GameState::GameState(const GameState& other)
{
	platform_size = other.platform_size;
	platform_data = new uint8_t[platform_size];
	std::memcpy(platform_data, other.platform_data, platform_size);
		
	module_size = other.module_size;
	module_data = new uint8_t[module_size];
	std::memcpy(module_data, other.module_data, module_size);
}

GameState::~GameState()
{
	clear();
}

GameState& GameState::operator=(const GameState& other)
{
	if(&other == this)
	{
		return *this;
	}
	
	clear();

	platform_size = other.platform_size;
	platform_data = new uint8_t[platform_size];
	std::memcpy(platform_data, other.platform_data, platform_size);
		
	module_size = other.module_size;
	module_data = new uint8_t[module_size];
	std::memcpy(module_data, other.module_data, module_size);
	return *this;
}

bool GameState::operator!=(const GameState& other) const
{
	return !(*this == other);
}

bool GameState::operator==(const GameState& other) const
{
	if(platform_size != other.platform_size)
	{
		return false;
	}
	if(module_size != other.module_size)
	{
		return false;
	}
	if(std::memcmp(platform_data, other.platform_data, platform_size) != 0)
	{
		return false;
	}
	if(std::memcmp(module_data, other.module_data, module_size) != 0)
	{
		return false;
	}
	return true;
}

void GameState::clear()
{
	if(platform_data != NULL)
	{
		delete[] platform_data;
	}
	platform_data = NULL;
	platform_size = 0;

	if(module_data != NULL)
	{
		delete[] module_data;
	}
	module_data = NULL;
	module_size = 0;
}
