#include <stdint.h>

void drawBuf(uint8_t* buffer, uint32_t buf_width, uint32_t buf_height, uint32_t pitch)
{
	uint8_t* row = (uint8_t*)buffer;
	for(int y = 0; y < buf_height; ++y)
	{
		uint8_t* pixel = (uint8_t*)row;
		for(int x = 0; x < buf_width; ++x)
		{
			// red
			*pixel = 0;
			++pixel;

			// blue;
			*pixel = (uint8_t)y;
			++pixel;

			// green;
			*pixel = (uint8_t)x;
			++pixel;

			*pixel = 0xFF;
			++pixel;
		}
		row += pitch;
	}
}
