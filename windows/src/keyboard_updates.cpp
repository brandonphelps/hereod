
#include <windows.h>
#include "game_controller.h"

#include "console_another.h"

uint32_t char_to_index_functor(uint32_t input_character)
{
	WriteLine("Char to index func" + std::to_string(input_character));
	return 0;
}

void UpdateKeyboardInputs(const MSG& msg, GameInputController* newKeyboard)
{
	uint32_t VKCode = (uint32_t)msg.wParam;
	bool WasDown = ((msg.lParam & (1 << 30)) != 0);
	bool IsDown = ((msg.lParam & (1 << 31)) == 0);
	// seems like this triggers on ups and downs
	if(WasDown != IsDown)
	{
		// uint32_t keyboard_index = char_to_index_functor(VKCode);
		//ProcessKeyMessage(&(newKeyboard->keyboardController.keys[keyboard_index]), IsDown);
		if(VKCode == 'A')
		{
			ProcessKeyMessage(&(newKeyboard->MoveLeft), IsDown);
		}
		if(VKCode == 'W')
		{
			ProcessKeyMessage(&(newKeyboard->MoveUp), IsDown);
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
