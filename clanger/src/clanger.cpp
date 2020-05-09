#include "clanger.h"

clanger_print clanger_std_print = NULL;

void clanger_set_print(clanger_print p)
{
	clanger_std_print = p;
}

void clanger_main(void)
{
	if(clanger_std_print)
	{
		clanger_std_print("hello world from clanger");
	}
}
