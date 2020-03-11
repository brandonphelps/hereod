#pragma once

#include <stdint.h>
#include "console_another.h"

class PixelBuffer
{
public:
	PixelBuffer() : width(0), height(0), buffer(NULL)
	{
		
	}

	~PixelBuffer()
	{
		WriteOut("Pixel Buffer Deconstructor");
		if(buffer != NULL)
		{
			delete buffer;
			buffer = NULL;
		}
	}

	uint32_t width; // in pixels
	uint32_t height; // in pixels
	uint32_t* buffer;

	void DrawRect(uint32_t start_row, uint32_t start_col, uint32_t end_row, uint32_t end_col, uint32_t color_mask)
	{
		// todo(brandon): bounds checking


		for(uint32_t row = start_row; row < end_row; row++)
		{
			for(uint32_t col = start_col; col < end_col; col++)
			{
				buffer[row * col + width] = color_mask;
			}
		}
	}
};

void resize_buffer(PixelBuffer& buf, uint32_t new_width, uint32_t new_height);
void BlitBuffer(void* source, uint32_t src_x, uint32_t src_y,
                void* dest, uint32_t dest_x, uint32_t dest_y);
