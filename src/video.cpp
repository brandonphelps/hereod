#include <stdint.h>

#ifdef _WIN32
#include "console_another.h"
#endif

void drawBuf(uint8_t* buffer, uint32_t buf_width, uint32_t buf_height, uint32_t pitch)
{
	static int printme = 1;
	#ifdef _WIN32
	if(printme)
	{
		WriteOut("Draw buff: " + std::to_string(buf_width) +", " + std::to_string(buf_height) + "\n\r");
	}
	
	#endif

	uint8_t* row = (uint8_t*)buffer;
	for(int y = 0; y < buf_height; ++y)
	{
		uint8_t* pixel = (uint8_t*)row;
		for(int x = 0; x < buf_width; ++x)
		{
			#ifdef _WIN32

			// blue
			*pixel = static_cast<uint8_t>(y);
			++pixel;

			// green
			*pixel = static_cast<uint8_t>(x);
			++pixel;

			// red
			*pixel = 0;
			++pixel;

			// alpha 
			*pixel = 0xFF;
			++pixel;

			#else

			// red
			*pixel = 0;
			++pixel;

			// blue;
			*pixel = (uint8_t)y;
			++pixel;

			// green;
			*pixel = (uint8_t)x;
			++pixel;

			// alpha
			*pixel = 0xFF;
			++pixel;

			#endif

			

		}
		row += pitch;
	}
	printme = 0;
}

void DrawRectangle(uint8_t* buffer, uint8_t buf_width, uint8_t x, uint8_t y, uint8_t width, uint8_t height, uint8_t red, uint8_t blue, uint8_t green)
{
	for(int s_y = y; s_y < height; s_y++)
	{
		for(int s_x = x; s_x < width; s_x++)
		{
			uint8_t* pixel = &(buffer[s_x + s_y * buf_width]);
			*pixel = red;
			++pixel;

			*pixel = blue;
			++pixel;

			*pixel = green;
			++pixel;

			*pixel = 0xFF;
			++pixel;
		}
	}
}
