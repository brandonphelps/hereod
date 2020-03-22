
#include <string>

#include "game_state.h"
#include "game_input.h"
#include "game_recording.h"

#define ReplayBuffSize 1000
static GameState initial_state;
static GameInput replay_buff[ReplayBuffSize];
static uint32_t replayIndex;
static uint32_t replaySize;

void BeginRecordingInput(GameState* initialState)
{
	initial_state = *initialState;
	replayIndex = 0;
	replaySize = 0;
}

GameInput* GetInputState(uint32_t index)
{
	uint32_t cur_index = index % replaySize;
	return replay_buff + cur_index;
}

void AppendInputState(GameInput* input)
{
	if(replayIndex > ReplayBuffSize)
	{
		replayIndex = 0;
	}

	std::memcpy(replay_buff + replayIndex,
	            input, sizeof(GameInput));
	replayIndex += 1;
	replaySize += 1;
	if(replaySize > ReplayBuffSize)
	{
		replaySize = ReplayBuffSize;
	}
}
