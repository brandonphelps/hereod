
#include <string>

#include "console_another.h"
#include "game_state.h"
#include "game_input.h"
#include "game_recording.h"

#define ReplayBuffSize 100000
static GameState initial_state;
static GameInput replay_buff[ReplayBuffSize];
static uint32_t replayIndex;
static uint32_t replaySize;

static uint32_t playbackIndex;

void ClearPlayback(GameState* state)
{
	playbackIndex = 0;
	replayIndex = 0;
	replaySize = 0;
	*state = initial_state;
}

void BeginRecordingInput(GameState* initialState)
{
	initial_state = *initialState;
	
	replayIndex = 0;
	replaySize = 0;
	playbackIndex = 0;
}

void GetNextInputStateUpdate(GameState* State, GameInput* input)
{
	playbackIndex = playbackIndex % replaySize;
	if(playbackIndex == 0)
	{
		// start the replay over, set state to intial state.
		*State = initial_state;
	}
	*input = replay_buff[playbackIndex];
	playbackIndex++;
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
