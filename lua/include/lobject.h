

#ifndef lua_object_h
#define lua_object_h

#include "llimits.h"

#define CommonHeader GCObject *next; lu_byte tt; lu_byte marked


struct GCObject
{
	GCObject *next;
};

// Closures

#define ClosureHeader \
	CommonHeader; \
	lu_byte nupvalues; GCObject *gclist



#endif
