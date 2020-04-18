

#ifndef lua_gc_object_h
#define lua_gc_object_h

#include "llimits.h"

#define CommonHeader GCObject *next; lu_byte tt; lu_byte marked


struct GCObject
{
	GCObject *next;
};

#endif
