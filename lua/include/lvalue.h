#ifndef lvalue_h
#define lvalue_h

#include "lgcobjects.h"

#include "lstate.h"
#include "luacapi.h"

struct Value {
	GCObject* gc;
	void *p; // light userdata
	int b;   // booleans
	lua_CFunction f; // light C Function
	// lua_Integer i;
	// lua_Number n;
};

#define TValuefields Value value_; int tt_


struct TValue
{
	Value value;
	int tt_;
};

typedef TValue *StkId;   // index to stack elements 

#define val_(o) ((o)->value_)


#define settt_(o, t) ((o)->tt_=(t))

#define setfvalue(obj, x) \
	{ TValue *io=(obj); val_(io).b=(x); settt_(io, LUA_TBOOLEAN); }

#endif
