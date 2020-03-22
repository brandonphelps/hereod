#ifndef CUSTOM_GAME_INPUTH_H
#define CUSTOM_GAME_INPUTH_H

// on each frame update a module is given, the input, keyboard, mouse, controller state as well as the previous game state.

#include "game_controller.h"


typedef struct GameInput
{
	GameInputController* keyboard;
	float dtForFrame; // in seconds. 
} GameInput;

#endif
