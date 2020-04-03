#include <stdint.h>

#include "video.h"

#ifdef _WIN32
#include "console_another.h"
#endif

void ScreenData::set_pixel_color(uint32_t x, uint32_t y, uint32_t color_mask)
{
	if(x > width || y > height)
	{
		return;
	}

	uint32_t offset = 0;
	uint32_t* pixel = (uint32_t*)(buffer + x * bytesPerPixel + y * bytesPerPixel * width);
	*pixel = color_mask;
}


void drawBuf(uint8_t* buffer, uint32_t buf_width, uint32_t buf_height)
{
	drawBuf(buffer, buf_width, buf_height, 4 * buf_width);
}

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
		row += buf_width * 4; 
	}
	printme = 0;
}

void DrawRectangle(ScreenData* data,
                   uint32_t s_x, uint32_t s_y,
                   uint32_t width, uint32_t height,
                   uint32_t color_mask)
{
	uint32_t MaxX = s_x + width;
	uint32_t MaxY = s_y + height;
	if(MaxX > data->width)
	{
		MaxX = data->width;
	}

	if(MaxY > data->height)
	{
		MaxY = data->height;
	}

	for(int y = s_y; y < MaxY; ++y)
	{
		for(int x = s_x; x < MaxX; ++x)
		{
			data->set_pixel_color(x, y, color_mask);
		}
	}
}

void DrawRectangle(uint8_t* buffer,
                   uint32_t buf_width, uint32_t buf_height,
                   uint32_t s_x, uint32_t s_y,
                   uint32_t width, uint32_t height,
                   uint8_t red, uint8_t blue, uint8_t green)
{
	uint32_t color = red << 16 | green << 8 | blue << 0;

	uint32_t MaxX = s_x + width;
	uint32_t MaxY = s_y + height;

	if(MaxX > buf_width)
	{
		MaxX = buf_width;
	}

	if(MaxY > buf_height)
	{
		MaxY = buf_height;
	}

	uint8_t* row = (buffer + s_x*4 + s_y*buf_width*4);

	for(int y = s_y; y < MaxY; ++y)
	{
		uint32_t *pixel = (uint32_t*)row;
		for(int x = s_x; x < MaxX; ++x)
		{
			*pixel = color;
			pixel++;
		}
		row += buf_width * 4;
	}
}



void DrawText(ScreenData* screenData, uint32_t x, uint32_t y, const std::string& data)
{
	
}
