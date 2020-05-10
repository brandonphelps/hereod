
#include <windows.h>
#include "game_controller.h"
#include "console_another.h"

void KeyboardInputController::UpdateButtonState(const uint32_t VKCode , bool IsDown)
{
	if(VKCode == VK_SHIFT)
	{
		ProcessKeyMessage(&(shift), IsDown);
	}
	if(VKCode == VK_RETURN)
	{
		ProcessKeyMessage(&(enter), IsDown);
	}
	if(VKCode == VK_ESCAPE)
	{
		ProcessKeyMessage(&(escape), IsDown);
	}
}

void UpdateKeyboardInputs(const MSG& msg, GameInputController* newKeyboard)
{
	uint32_t VKCode = (uint32_t)msg.wParam;
	bool WasDown = ((msg.lParam & (1 << 30)) != 0);
	bool IsDown = ((msg.lParam & (1 << 31)) == 0);
	// this triggers on ups and downs, if this isn't done when a keyboard key is held down
	// the keyboards triggers multiple "down" key events .
	if(WasDown != IsDown)
	{
		newKeyboard->keyboardController.UpdateButtonState(VKCode, IsDown);
		
		// if(VKCode == 'A')
		// {
		// 	ProcessKeyMessage(&(newKeyboard->MoveLeft), IsDown);
		// }
		// if(VKCode == 'W')
		// {
		// 	ProcessKeyMessage(&(newKeyboard->MoveUp), IsDown);
		// }
		// if(VKCode == 'S')
		// {
		// 	ProcessKeyMessage(&(newKeyboard->MoveDown), IsDown);
		// }
		// if(VKCode == 'D')
		// {
		// 	ProcessKeyMessage(&(newKeyboard->MoveRight), IsDown);
		// }
	}
	
}
