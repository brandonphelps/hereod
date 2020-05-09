
#ifndef Blue_MAP_H
#define Blue_MAP_H

#include <vector>
#include <stdint.h>

class Point
{
public:
	uint8_t x;
	uint8_t y;
};

class Map
{
public:
	// 10 by 10 grid
	uint8_t tile_info[100];
	uint8_t width;
	uint8_t height;
	Point* way_points;
	uint8_t way_point_count;
};


#endif
