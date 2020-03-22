
#include <vector.h>

static GameState initial_state;
static GameInput[1000] replay_buff;
static uint32_t replayIndex = 0;

void BeginRecordingInput(GameState* initialState)
{
	initial_state = *initial_state;
	replayIndex = 0;
}

void AppendInputState(GameInput* input)
{
	GameInput* copy = new GameInput();
	copy.keyboard = 
}
