

#ifndef lstate_h
#define lstate_h

#include "lua.h"
#include "lobject.h"


// chars used as small naturals
typedef unsigned char lu_byte;

struct lua_State
{
	// common header. 
	GCObject *next;
	lu_byte tt;

	int stacksize;
	int hookcount;
};


struct global_State
{
	lua_Alloc frealloc; // function to reallocate memory
	lua_State* mainthread;
	const lua_Number* version;
};


#endif
