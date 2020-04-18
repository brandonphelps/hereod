

#ifndef lstate_h
#define lstate_h

#include "lgcobjects.h"
#include "lvalue.h"
#include "luatypedefs.h"

struct lua_State
{
	// common header. 
	GCObject *next;
	lu_byte tt;

	int stacksize;
	int hookcount;
	StkId top;
};



struct global_State
{
	lua_Alloc frealloc; // function to reallocate memory
	lua_State* mainthread;
	const lua_Number* version;
};


#endif
