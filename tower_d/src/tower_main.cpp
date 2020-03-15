#include <iostream>
#include <stdint.h>

#include "video.h"

#ifdef _WIN32
#include "console_another.h"
#endif

// works fine on windows, but something about console doesn't allow writing to.
__attribute__((__visibility__("default"))) int GameInit()
{
	return 0;
}

// some sort of buffer for video data is passed back and forth here.
int GameUpdate(int dt, ScreenData* screenData)
{
	// update the video buffer data as provided.
	// the width and height, etc will be updated for you. 

	drawBuf(screenData->buffer, screenData->width,
	        screenData->height, screenData->pitch);

	return 0;

}

int GameShutdown()
{
	return 0;
}
