
#ifndef CUSTOM_GAME_MODULE_H
#define CUSTOM_GAME_MODULE_H

#include <string>
#include "video.h"
#include "game_state.h"
#include "controller.h"



typedef int (*FGameInit)(GameState*);

// todo(brandon): consider allowing for only specific implementations and or use dif names. 
// typedef int (*FGameUpdateA)(int, ScreenData*);
// todo(brandon): make controller, a list or something so games can be multi controllered.
typedef int (*FGameUpdate)(int, ScreenData*, GameState*, GameInputController*);

typedef int (*FGameShutdown)();

typedef struct ModuleFunctions
{
	FGameInit GameInit;
	FGameUpdate GameUpdate;
	FGameShutdown GameShutdown;
	void* lib_handle;
} ModuleFunctions;

void UnloadModule(ModuleFunctions& module_funcs);
void LoadModule(ModuleFunctions& module_funcs, const std::string& lib_path);
#endif
