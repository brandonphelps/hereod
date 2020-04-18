



#ifndef clanger_h
#define clanger_h

#include <string>

// need a print function, needs to be dynamic, let user define which function to call for pint.
// need memory allocation function, same as above.
// need stack ish based objs that refer to each other.

// holds a scope's worth of variables
struct clanger_Env
{
	
};


typedef void (*clanger_print)(const std::string& msg);

typedef void (*clanger_fp) (void);

// holds reference to current states objs. 
struct clanger_State
{
	clanger_State* next; // pointer to next state in the list. 
	clanger_fp o;
};


void clanger_set_print(clanger_print p);

void clanger_main(void);


#endif
