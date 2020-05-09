// Common Input layer

#ifndef MY_CUSTOM_KEYBOARD_H
#define MY_CUSTOM_KEYBOARD_H

#ifdef _WIN32
#include <windows.h>
#endif

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
	GameButtonState keys[1000];
	GameButtonState shift; 
	GameButtonState enter;
	GameButtonState escape;

#ifdef _WIN32
	void UpdateButtonState(const uint32_t keycode, bool IsDown);
#endif
private:
	
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

void InitKeyboardController(KeyboardInputController& keyboard);
void GameInputControllerInit(GameInputController* controller);
void ProcessKeyMessage(GameButtonState* buttonState, bool IsDown);


#endif
