

#include <stdint.h>
#include "controller.h"



void ProcessKeyMessage(GameButtonState* buttonState, bool IsDown)
{
	if(buttonState->EndedDown != IsDown)
	{
		buttonState->EndedDown = IsDown;
		++(buttonState->HalfTransitionCount);
	}
}
