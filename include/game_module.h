
#ifndef CUSTOM_GAME_MODULE_H
#define CUSTOM_GAME_MODULE_H
#include "video.h"
#include "game_state.h"
#include <string>


typedef int (*FGameInit)(GameState*);
typedef int (*FGameShutdown)();
// todo(brandon): consider allowing for only specific implementations and or use dif names. 
// typedef int (*FGameUpdateA)(int, ScreenData*);
typedef int (*FGameUpdate)(int, ScreenData*, GameState*);

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
