
#ifndef H_CUST_BITMAP_H
#define H_CUST_BITMAP_H

#include <iostream>

// #include <fstream> // can't pass a c string into std::ifstream constructor
// must be using wrong compiler flags or something.
#include <cstdio>

#include "pixel_color.h"

#ifdef _WIN32
#include "console_another.h"
#endif

enum class BitMapPixelLayout
{
 BGR,
 BGRA
};

class BitMapPixelIter
{
public:
	// length is number of pixels
	// bitmappixel iter will not consume ownership of start_buffer.
	// should already be allocated. 
	BitMapPixelIter(uint8_t* start_buffer, uint32_t length, BitMapPixelLayout layout);

	// copy construct is able to be created by compiler.
	// performs membor wise copy
	// https://en.cppreference.com/w/cpp/language/copy_constructor
	// todo write tests to confirm this. 
	
	// return the current pixel color and move the pointer forward. 
	PixelColor next();

	bool end_iteration() const;

private:
	PixelColor nextBGR();
	PixelColor nextBGRA();

	uint8_t* _pixel_buffer;
	uint8_t* _pixel_cursor;
	BitMapPixelLayout _pixel_layout;
	uint32_t _length;
	uint32_t _current_index;
	bool _end_reached;
};

class HBitmap
{
public:
	uint8_t header[13];
	uint8_t magic_number[2];
	uint32_t file_size; 
	uint32_t data_offset;

	uint8_t info_header[40];

	uint32_t info_header_size; // should be 40.
	uint32_t width; // width of bitmap in pixels
	uint32_t height;  // width of bitmap in pixels
	bool y_flipped; // indicates that the height was negative.
	bool x_flipped; // indicates that the width was negative.
	uint16_t planes;
	uint16_t bits_per_pixel;
	uint32_t compression;
	uint32_t image_size;
	uint32_t xpixels_per_meter;
	uint32_t ypixels_per_meter;
	uint32_t colors_used;
	uint32_t important_colors;
	// consider bundling the pixel_buffer array with a color arrangment. layout
	// this could allow for a pixel color iter where T is the requested pixel color format.
	uint8_t* pixel_buffer;
	BitMapPixelLayout layout;

	// todo add in end method.
	BitMapPixelIter begin();

private:
};


void LoadBitmap(const std::string& filepath, HBitmap& bitmap);
#endif
