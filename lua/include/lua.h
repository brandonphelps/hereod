


#ifndef lua_h
#define lua_h

#include "luaconf.h"

#define LUA_VERSION_MAJOR "5"
#define LUA_VERSION_MINOR "3"
#define LUA_VERSION_NUM   503
#define LUA_VERSION_RELEASE "5"


#define LUA_SIGNATURE "\x1bLua"

#define LUA_MULTRET     (-1)


typedef LUA_NUMBER lua_Number;


typedef void* (*lua_Alloc)(void *ud, void* ptr, size_t osize, size_t nsize);


#endif
