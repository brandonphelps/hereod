
#include "pixel_color.h"

#include <iomanip>
#include <iostream>

std::ostream& operator<<(std::ostream& out, const PixelColor& p)
{
	out << "Pixel: " << std::hex << std::setfill('0');
	out << std::setw(2) << static_cast<int>(p.blue);
	out << std::setw(2) << static_cast<int>(p.green);
	out << std::setw(2) << static_cast<int>(p.red);
	out << std::setw(2) << static_cast<int>(p.alpha);
	return out;
}
