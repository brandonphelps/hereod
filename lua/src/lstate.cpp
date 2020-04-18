
#include "lua.h"
#include "lstate.h"

struct LX
{
	lua_State l;
};

struct LG
{
	LX l;
	global_State g;
};


lua_State* lua_newstate(lua_Alloc f, void *ud)
{
	int i;
	lua_State* L;
	global_State *g;
	LG* l = reinterpret_cast<LG*>((*f)(ud, NULL, LUA_TTHREAD, sizeof(LG)));
	if(l == NULL)
	{
		return NULL;
	}

	L = &l->l.l;
	g = &l->g;
	L->next = NULL;
	L->tt = LUA_TTHREAD;
	
	return L;
}
