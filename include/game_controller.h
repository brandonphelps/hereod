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


class KeyboardInputController
{
public:
	GameButtonState keys[200];
};


class GameInputController
{
public:
	bool isConnected;
	bool isAnalog;
	KeyboardInputController keyboardController;
	GameButtonState MoveUp;
	GameButtonState MoveDown;
	GameButtonState MoveLeft;
	GameButtonState MoveRight;
};

void GameInputControllerInit(GameInputController* controller);
void ProcessKeyMessage(GameButtonState* buttonState, bool IsDown);


#endif
