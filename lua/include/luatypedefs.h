
#ifndef lua_typedefs_h
#define lua_typedefs_h

#include <stdint.h>
#include "luaconf.h"

typedef void* (*lua_Alloc) (void *ud, void *ptr, size_t osize, size_t nsize);
typedef LUA_NUMBER lua_Number;

#endif
