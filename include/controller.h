// Common Input layer

#ifndef CUSTOM_KEYBOARD_H
#define CUSTOM_KEYBOARD_H

#include <stdint.h>

typedef struct GameButtonState
{
	int HalfTransitionCount;
	bool EndedDown;
} GameButtonState;

typedef struct GameInputController
{

	bool isConnected;
	bool isAnalog;
	GameButtonState MoveUp;
	GameButtonState MoveDown;
	GameButtonState MoveLeft;
	GameButtonState MoveRight;

} GameInputController;



void GameInputControllerInit(GameInputController* controller);
void ProcessKeyMessage(GameButtonState* buttonState, bool IsDown);


#endif
