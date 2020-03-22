
#include <windows.h>
#include "game_controller.h"

void UpdateKeyboardInputs(const MSG& msg, GameInputController* newKeyboard)
{
	uint32_t VKCode = (uint32_t)msg.wParam;
	bool WasDown = ((msg.lParam & (1 << 30)) != 0);
	bool IsDown = ((msg.lParam & (1 << 31)) == 0);
	// seems like this triggers on ups and downs
	if(WasDown != IsDown)
	{
		if(VKCode == 'W')
		{
			ProcessKeyMessage(&(newKeyboard->MoveUp), IsDown);
		}
		if(VKCode == 'A')
		{
			ProcessKeyMessage(&(newKeyboard->MoveLeft), IsDown);
		}
		if(VKCode == 'S')
		{
			ProcessKeyMessage(&(newKeyboard->MoveDown), IsDown);
		}
		if(VKCode == 'D')
		{
			ProcessKeyMessage(&(newKeyboard->MoveRight), IsDown);
		}
	}
	
}
