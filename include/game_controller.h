// Common Input layer

#ifndef MY_CUSTOM_KEYBOARD_H
#define MY_CUSTOM_KEYBOARD_H

#include <stdint.h>

class GameButtonState
{
public:
	int HalfTransitionCount;
	bool EndedDown;
};


class GameInputController
{
public:
	bool isConnected;
	bool isAnalog;
	GameButtonState MoveUp;
	GameButtonState MoveDown;
	GameButtonState MoveLeft;
	GameButtonState MoveRight;
};



void GameInputControllerInit(GameInputController* controller);
void ProcessKeyMessage(GameButtonState* buttonState, bool IsDown);


#endif
