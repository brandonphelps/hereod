

#ifndef lstate_h
#define lstate_h

#include "lua.h"

struct global_State
{
	lua_Alloc frealloc; // function to reallocate memory
	lua_State *mainthread;
	const lua_Number *version;
};


struct lua_State {
	int stacksize;
	int hookcount;
};
