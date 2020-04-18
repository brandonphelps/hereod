
#include "lua.h"
#include "lstate.h"
#include "lvalue.h"

void lua_pushcclosure(lua_State *L, lua_CFunction fn, int n)
{
	if(n == 0)
	{
		setfvalue(L->top, fn);
		// api_incr_top(L);
	}
	else
	{
	}
}
