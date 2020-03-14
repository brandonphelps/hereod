



#include <iostream>

#ifdef _WIN32
#include "console_another.h"
#endif

void GameInit()
{
	#ifdef _WIN32
	WriteOut("Hello world\n\r");
	#endif


}
