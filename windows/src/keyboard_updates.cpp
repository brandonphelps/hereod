
#include <windows.h>
#include "game_controller.h"
#include "console_another.h"

uint32_t char_to_index_functor(uint32_t input_character)
{
	WriteLine("Char to index func " + std::to_string(input_character));
	uint32_t result = 0;
	// 1 - (90 - 65) = 25 - 1 = 24
	if(input_character >= 65 && input_character <= 90)
	{
		result = input_character - 64;
	}
	WriteLine("Result of index to char: " + std::to_string(result));
	return result;
}

void KeyboardInputController::UpdateButtonState(const uint32_t VKCode , bool IsDown)
{
	uint32_t keyboard_index = char_to_index_functor(VKCode);
	ProcessKeyMessage(&(keys[keyboard_index]), IsDown); 
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
		WriteLine("Raw VKCode: " + std::to_string(VKCode));
		WriteLine("Transition: " + std::to_string(msg.wParam) + " IsDown: " + std::to_string(IsDown));
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
