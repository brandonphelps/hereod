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
	union
	{
		GameButtonState Buttons[4];
		struct 
		{
			GameButtonState MoveUp;
			GameButtonState MoveDown;
			GameButtonState MoveLeft;
			GameButtonState MoveRight;

			GameButtonState count;
		};
	};
} GameInputController;


void ProcessKeyMessage(GameButtonState* buttonState, bool IsDown);


#endif
