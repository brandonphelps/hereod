



#include <iostream>

#ifdef _WIN32
#include "console_another.h"
#endif

// works fine on windows, but something about console doesn't allow writing to.
int GameInit()
{
	#ifdef _WIN32
	WriteOut("Hello world\n\r");
	#endif
	WriteOut("Hello world\n\r");
	return 4;
}
