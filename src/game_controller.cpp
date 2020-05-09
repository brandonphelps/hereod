

#include <stdint.h>
#include "game_controller.h"

void InitGameButtonState(GameButtonState& b)
{
	b.EndedDown = false;
	b.HalfTransitionCount = 0;
}

void InitKeyboardController(KeyboardInputController& keyboard)
{
	for(int i = 0; i < 40; i++)
	{
		InitGameButtonState(keyboard.keys[i]);
	}
	InitGameButtonState(keyboard.shift);
	InitGameButtonState(keyboard.enter);
	InitGameButtonState(keyboard.escape);
}

void GameInputControllerInit(GameInputController* controller)
{
	InitGameButtonState(controller->MoveUp);
	InitGameButtonState(controller->MoveDown);
	InitGameButtonState(controller->MoveLeft);
	InitGameButtonState(controller->MoveRight);
	InitKeyboardController(controller->keyboardController);
}

void ProcessKeyMessage(GameButtonState* buttonState, bool IsDown)
{
	if(buttonState->EndedDown != IsDown)
	{
		buttonState->EndedDown = IsDown;
		++(buttonState->HalfTransitionCount);
	}
}

