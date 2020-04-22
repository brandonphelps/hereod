
#ifndef H_CUST_BITMAP_H
#define H_CUST_BITMAP_H

#include <iostream>
// #include <fstream> // can't pass a c string into std::ifstream constructor
// must be using wrong compiler flags or something.
#include <cstdio>

#ifdef _WIN32
#include "console_another.h"
#endif

class HBitmap
{
public:
	uint8_t header[13];
	uint32_t file_size; 
	uint32_t data_offset;

	uint8_t info_header[40];

	uint32_t info_header_size; // should be 40.
	uint32_t width; // width of bitmap in pixels
	uint32_t height;  // width of bitmap in pixels
	uint16_t planes;
	uint16_t bits_per_pixel;
	uint32_t compression;
	uint32_t image_size;
	uint32_t xpixels_per_meter;
	uint32_t ypixels_per_meter;
	uint32_t colors_used;
	uint32_t important_colors;

	uint8_t* pixel_buffer;

private:
};


// appears bitmaps are little endian.
uint32_t from_byte_array32(uint8_t* pointer)
{
	uint32_t result = 0;
	result = ((uint64_t)pointer[0]) | ((uint64_t)pointer[1]) << 8 | ((uint64_t)pointer[2]) << 16 | ((uint64_t)pointer[3]) << 24;
	return result;
}

uint16_t from_byte_array16(uint8_t* pointer)
{
	uint16_t result = 0;
	result = ((uint16_t)pointer[0]) | ((uint16_t)pointer[1]) << 8;
	return result;
}

static char valueConvertTableF[16] =
	{
	 48,
	 49,
	 50,
	 51,
	 52,
	 53,
	 54,
	 55,
	 56,
	 57,
	 65,
	 66,
	 67,
	 68,
	 69,
	 70
	};


void print_byte_array(uint8_t* pointer, uint32_t size)
{
	std::stringstream stream; 
	for(int i = 0; i < size; i++)
	{
		uint8_t highNibble = 0x0F & (pointer[i] >> 4);
		uint8_t lowNibble  = 0x0F & pointer[i];
		stream << valueConvertTableF[highNibble] << valueConvertTableF[lowNibble];
	}
	WriteLine(stream.str());
}

// reads file and loads files contents into bitmap
// new resources are allocated, in pixel_buffer, ensure they are cleaned up.
void LoadBitmap(const std::string& filepath, HBitmap& bitmap)
{
	std::FILE* file_handle = fopen(filepath.c_str(), "rb");
	char temp_buffer[2000];
	std::memset(temp_buffer, 0, 2000);

	if(file_handle)
	{
		// header
		for(int i = 0; i < 14; i++)
		{
			temp_buffer[i] = std::fgetc(file_handle);
		}

		// check header
		if(!(temp_buffer[0] == 'B' && temp_buffer[1] == 'M'))
		{
			throw std::runtime_error("Invalid signature");
		}

		// replace with cool deserialization lib. 
		uint8_t* buf = reinterpret_cast<uint8_t*>(temp_buffer + 2);
		bitmap.file_size = from_byte_array32(buf);
		buf = reinterpret_cast<uint8_t*>(temp_buffer + 2 + 4+ 6);
		bitmap.data_offset = from_byte_array32(buf);
		std::memcpy(bitmap.header, temp_buffer, 13);
		std::memset(temp_buffer, 0, 13);
		for(int i = 0; i < 40; i++)
		{
			temp_buffer[i] = std::fgetc(file_handle);
		}

		uint32_t offset = 0;

		buf = reinterpret_cast<uint8_t*>(temp_buffer);
		bitmap.info_header_size = from_byte_array32(buf);
		offset += 4;

		buf = reinterpret_cast<uint8_t*>(temp_buffer+offset);
		bitmap.width = from_byte_array32(buf);
		offset += 4;

		buf = reinterpret_cast<uint8_t*>(temp_buffer+offset);
		bitmap.height = from_byte_array32(buf);
		offset += 4;

		buf = reinterpret_cast<uint8_t*>(temp_buffer+offset);
		bitmap.planes = from_byte_array16(buf);
		offset += 2;

		WriteLine("Offset for bits per pixel: " + std::to_string(offset+14));
		buf = reinterpret_cast<uint8_t*>(temp_buffer+offset);
		print_byte_array(buf, 2);
		bitmap.bits_per_pixel = from_byte_array16(buf);
		offset += 2;

		buf = reinterpret_cast<uint8_t*>(temp_buffer+offset);
		bitmap.compression = from_byte_array32(buf);
		offset += 4;

		buf = reinterpret_cast<uint8_t*>(temp_buffer+offset);
		bitmap.image_size = from_byte_array32(buf);
		offset += 4;

		buf = reinterpret_cast<uint8_t*>(temp_buffer+offset);
		bitmap.xpixels_per_meter = from_byte_array32(buf);
		offset += 4;

		buf = reinterpret_cast<uint8_t*>(temp_buffer+offset);
		bitmap.ypixels_per_meter = from_byte_array32(buf);
		offset += 4;
		
		buf = reinterpret_cast<uint8_t*>(temp_buffer+offset);
		bitmap.colors_used = from_byte_array32(buf);
		offset += 4;

		buf = reinterpret_cast<uint8_t*>(temp_buffer+offset);
		bitmap.important_colors = from_byte_array32(buf);
		offset += 4;

		// end of info header.

		if(bitmap.bits_per_pixel < 8)
		{
			// load color table. 
			throw std::runtime_error("currently not supported for loading color used data: " + std::to_string(bitmap.colors_used));
		}

		// load pixel data.
		// pitch is the number of bytes in a row.
		if(bitmap.bits_per_pixel == 1 || bitmap.bits_per_pixel == 4)
		{
			throw std::runtime_error("Unsported bitmap bits per pixel");
		}

		uint8_t bytes_per_pixel = bitmap.bits_per_pixel / 8;

		uint32_t pitch = bitmap.width * bytes_per_pixel;

		bitmap.pixel_buffer = new uint8_t[pitch * bitmap.height];
		for(int i = 0; i < pitch * bitmap.height; ++i)
		{
			bitmap.pixel_buffer[i] = static_cast<uint8_t>(std::fgetc(file_handle));
		}
	}
	else
	{
		throw std::runtime_error("Failed to open file: " + filepath);
	}

	WriteLine("BITMAP loaded");
	WriteLine("File size: " + std::to_string(bitmap.file_size));
	WriteLine("Data Offset: " + std::to_string(bitmap.data_offset));
	WriteLine("Width: " + std::to_string(bitmap.width));
	WriteLine("Height: " + std::to_string(bitmap.height));
	WriteLine("Bits per pixel: " + std::to_string(bitmap.bits_per_pixel));
}

#endif
