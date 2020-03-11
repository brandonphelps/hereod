
#include "video.h"

void resize_size_buffer(PixelBuffer& buf, uint32_t new_width, uint32_t new_height)
{
	if(buf.buffer != NULL)
	{
		delete buf.buffer;
		buf.buffer = NULL;
	}

	buf.buffer = new uint32_t[new_height * new_width];	
	buf.width = new_width;
	buf.height = new_height;
}


void BlitBuffer(void* source, uint32_t src_x, uint32_t src_y,
                void* dest, uint32_t dest_x, uint32_t dest_y)
{
	
}
