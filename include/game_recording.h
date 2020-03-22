#ifndef GAME_RECORDING_H
#define GAME_RECORDING_H

#include "game_state.h"
#include "game_input.h"

void BeginRecordingInput(GameState* initialState);

void AppendInputState(GameInput* input);
GameInput* GetInputState(uint32_t index);

void EndRecordingInput(GameState* state);





#endif
