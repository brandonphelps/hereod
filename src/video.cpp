#ifdef _WIN32
#include "console_another.h"
#endif

#include "video.h"

#include <stdint.h>
#include <cstring>

uint8_t* ScreenData::get_buffer_at(uint32_t x, uint32_t y)
{
	return buffer + (x * bytesPerPixel) + (y * bytesPerPixel * width);
}

void ScreenData::set_pixel_color(uint32_t x, uint32_t y, uint32_t color_mask)
{
	if(x >= width || y >= height)
	{
		return;
	}

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




void BlitScreenData(ScreenData& source, ScreenData& dest, uint32_t dest_pixel_x, uint32_t dest_pixel_y)
{
	BlitScreenData(source, dest, dest_pixel_x, dest_pixel_y, 0, 0);
}

void BlitScreenData(ScreenData& source, ScreenData& dest, uint32_t dest_pixel_x, uint32_t dest_pixel_y, uint32_t source_pixel_x, uint32_t source_pixel_y)
{
	
	uint8_t* row = dest.buffer + dest_pixel_x*4 + dest_pixel_y*dest.width*4;
	uint8_t* copy_row = source.buffer + source_pixel_x*4 + source_pixel_y*source.width*4;

	uint32_t MaxX = dest_pixel_x + source.width;
	uint32_t MaxY = dest_pixel_y + source.height;

	if(MaxX > dest.width)
	{
		MaxX = dest.width;
	}

	if(MaxY > dest.height)
	{
		MaxY = dest.height;
	}

	for(int y = dest_pixel_y; y < MaxY; ++y)
	{
		uint32_t* pixel = (uint32_t*)row;
		uint32_t* copy_pixel = (uint32_t*)copy_row;
		for(int x = dest_pixel_x; x < MaxX; ++x)
		{
			*pixel = *copy_pixel;
			++pixel;
			++copy_pixel;
		}
		row += dest.width * 4;
		copy_row += source.width * 4;
	}
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



void resize_buffer(ScreenData& screendata,
                   uint32_t new_width,
                   uint32_t new_height)
{
	if(screendata.buffer != NULL)
	{
#if _WIN32
		WriteOut("Clearning out previously allocated buffer\n\r");
#endif

		delete screendata.buffer;
		screendata.buffer = NULL;
	}

#if _WIN32
	WriteOut("Resizing buffer\n\r");
#endif

	screendata.width = new_width;
	screendata.height = new_height;
	screendata.pitch = screendata.width * screendata.bytesPerPixel;

#if _WIN32
	WriteOut("Allocating (" + std::to_string(screendata.width) + "*" +
	         std::to_string(screendata.bytesPerPixel) + ")"  + std::to_string(screendata.pitch) + " * " +
	         std::to_string(screendata.height) +" == " + std::to_string(screendata.pitch * screendata.height) + "\n\r");
#endif
	screendata.buffer = new uint8_t[screendata.pitch * screendata.height];
	std::memset(screendata.buffer, 0, screendata.pitch * screendata.height);
}
