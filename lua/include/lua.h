


#ifndef lua_h
#define lua_h

#include "luaconf.h"
#include "lstate.h"
#include "luatypedefs.h"

#define LUA_VERSION_MAJOR "5"
#define LUA_VERSION_MINOR "3"
#define LUA_VERSION_NUM   503
#define LUA_VERSION_RELEASE "5"


#define LUA_SIGNATURE "\x1bLua"

#define LUA_MULTRET     (-1)

// basic types
#define LUA_TTHREAD		8






void (lua_pushcclosure) (lua_State *L, lua_CFunction fn, int n);

#define lua_pushcfunction(L, f) lua_pushcclosure(L, (f), 0)


typedef void* (*lua_Alloc)(void *ud, void* ptr, size_t osize, size_t nsize);


#endif
