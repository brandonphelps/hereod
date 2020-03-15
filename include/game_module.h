
#ifndef CUSTOM_GAME_MODULE_H
#define CUSTOM_GAME_MODULE_H
#include "video.h"


typedef int (*FGameInit)();
typedef int (*FGameShutdown)();
typedef int (*FGameUpdate)(int, ScreenData*);

#endif
