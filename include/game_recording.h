#ifndef GAME_RECORDING_H
#define GAME_RECORDING_H

#include "game_state.h"

class GameRecording
{
public:
	GameState initial_state;
};

void BeginRecordingInput(GameState* initialState);

void AppendInputState(GameInput* input);

void EndRecordingInput(GameState* state);





#endif
