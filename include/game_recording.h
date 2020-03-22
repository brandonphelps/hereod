#ifndef GAME_RECORDING_H
#define GAME_RECORDING_H

#include "game_state.h"
#include "game_input.h"

void BeginRecordingInput(GameState* initialState);

void ClearPlayback(GameState* state);
void AppendInputState(GameInput* input);
void GetNextInputStateUpdate(GameState* State, GameInput* input);
GameInput* GetInputState(uint32_t index);

void EndRecordingInput(GameState* state);





#endif
