
#ifndef PIXEL_COLOR_H
#define PIXEL_COLOR_H

#include <iostream>

class PixelColor
{
public:
	uint8_t blue;
	uint8_t green;
	uint8_t red;
	uint8_t alpha;
};


std::ostream& operator<<(std::ostream& out, const PixelColor& p);

#endif
