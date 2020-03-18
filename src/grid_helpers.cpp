#include "grid_helpers.h"

uint32_t get_2d_offset(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
{
	return (width * y) + x;
}
