
#ifndef VIDEO_H
#define VIDEO_H

#include <stdint.h>
#include <string>

#ifdef _WIN32
#include "console_another.h"
#endif


class ScreenData
{
public:
	static uint32_t screenDataID;
	// investigate move semantics to allow for data copy from one struct into another
	// especially when that pervious constructed item is just temporary, such as being in place
	// constructed at an array.
	ScreenData()
	{
		bytesPerPixel = 4;
		buffer = new uint8_t[10];
		my_id = screenDataID++;
		pitch = 0;
		width = 0;
		height = 0;
		#ifdef _WIN32
		WriteLine("Screen data Constructor: " + std::to_string(my_id));
		#endif
	}

	ScreenData(const ScreenData& other)
	{
		bytesPerPixel = other.bytesPerPixel;
		buffer = new uint8_t[other.pitch * other.height];
		pitch = other.pitch;
		width = other.width;
		height = other.height;
		std::memcpy(buffer, other.buffer, other.pitch * other.height);

		my_id = screenDataID++;
	}

	~ScreenData()
	{
		if(buffer != NULL)
		{
			delete[] buffer;
			buffer = NULL;
		}

		#ifdef _WIN32
		WriteLine("Screen data destructor: " + std::to_string(my_id));
		#endif
	}
	
	ScreenData& operator=(ScreenData& other)
	{
		if(&other == this)
		{
			return *this;
		}
		if(buffer != NULL)
		{
			delete[] buffer;
			buffer = NULL;
		}
		bytesPerPixel = other.bytesPerPixel;
		buffer = new uint8_t[other.pitch * other.height];
		pitch = other.pitch;
		width = other.width;
		height = other.height;
		std::memcpy(buffer, other.buffer, other.pitch * other.height);

		my_id = screenDataID++;
		return *this;
	}

	// set the pixel at pos x, y 
	void set_pixel_color(uint32_t x, uint32_t y,
	                     uint32_t color_mask);

	uint8_t* get_buffer_at(uint32_t x, uint32_t y);

	// todo add constructor. deconstructor.
public:
  uint32_t width;
  uint32_t height;
  uint32_t pitch;
  uint8_t bytesPerPixel;
  uint8_t *buffer;
private:
	uint32_t my_id;
};

void drawBuf(uint8_t*, uint32_t, uint32_t);
void drawBuf(uint8_t*, uint32_t, uint32_t, uint32_t);

// copies the buffer information from the source to the destination. 
void BlitScreenData(ScreenData& source, ScreenData& dest, uint32_t dest_pixel_x, uint32_t dest_pixel_y);

void BlitScreenData(ScreenData& source, ScreenData& dest, uint32_t dest_pixel_x, uint32_t dest_pixel_y, uint32_t source_pixel_x, uint32_t source_pixel_y);

void DrawRectangle(uint8_t* buffer,
                   uint32_t buf_width, uint32_t buf_height,
                   uint32_t x, uint32_t y,
                   uint32_t width, uint32_t height, uint8_t red, uint8_t blue, uint8_t green);

void DrawRectangle(ScreenData* screenData,
                   uint32_t x, uint32_t y,
                   uint32_t width, uint32_t height,
                   uint32_t color_mask);

void DrawText(ScreenData* screenData, uint32_t x, uint32_t y, const std::string& data);
	
void resize_buffer(ScreenData& screenData, uint32_t width, uint32_t height);


#endif
