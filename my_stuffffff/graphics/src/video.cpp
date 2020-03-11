
#include "video.h"
#include "console_another.h"

void resize_buffer(PixelBuffer& buf, uint32_t new_width, uint32_t new_height)
{
	if(buf.buffer != NULL)
	{
		delete buf.buffer;
		buf.buffer = NULL;
	}

	WriteOut("Resizing buffer\n\r");

	buf.buffer = new uint32_t[new_height * new_width];	
	buf.width = new_width;
	buf.height = new_height;
	for(int i = 0; i < new_height * new_width+1; i++)
	{
		buf.buffer[i] = 0xFFFF0000;
	}
}


void BlitBuffer(void* source, uint32_t src_x, uint32_t src_y,
                void* dest, uint32_t dest_x, uint32_t dest_y)
{
	
}
