



#include <iostream>

#include "video.h"

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


// some sort of buffer for video data is passed back and forth here.
int GameUpdate(int dt, ScreenData* screenData)
{
	// update the video buffer data as provided.
	// the width and height, etc will be updated for you. 

	return 0;

}

int GameShutdown()
{
	return 0;
}
