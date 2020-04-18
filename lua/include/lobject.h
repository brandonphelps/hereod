

#ifndef lua_object_h
#define lua_object_h


#include "lgcobjects.h"
#include "llimits.h"
#include "lstate.h"






// Closures

#define ClosureHeader \
	CommonHeader; \
	lu_byte nupvalues; GCObject *gclist

struct CClosure {
	ClosureHeader;
	lua_CFunction f;
	// TValue upvalue[1]; 
};



#endif
