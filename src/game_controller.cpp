

#include <stdint.h>
#include "game_controller.h"

void GameInputControllerInit(GameInputController* controller)
{
	controller->MoveUp.EndedDown = false;
	controller->MoveDown.EndedDown = false;
	controller->MoveLeft.EndedDown = false;
	controller->MoveRight.EndedDown = false;

	controller->MoveUp.HalfTransitionCount = false;
	controller->MoveDown.HalfTransitionCount = false;
	controller->MoveLeft.HalfTransitionCount = false;
	controller->MoveRight.HalfTransitionCount = false;
}

void ProcessKeyMessage(GameButtonState* buttonState, bool IsDown)
{
	if(buttonState->EndedDown != IsDown)
	{
		buttonState->EndedDown = IsDown;
		++(buttonState->HalfTransitionCount);
	}
}

