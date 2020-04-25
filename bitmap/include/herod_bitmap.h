
#ifndef H_CUST_BITMAP_H
#define H_CUST_BITMAP_H

#include <iostream>
#include <iomanip>
// #include <fstream> // can't pass a c string into std::ifstream constructor
// must be using wrong compiler flags or something.
#include <cstdio>

#ifdef _WIN32
#include "console_another.h"
#endif

class PixelColor
{
public:
	uint8_t blue;
	uint8_t green;
	uint8_t red;
	uint8_t alpha;
};

std::ostream& operator<<(std::ostream& out, const PixelColor& p)
{
	out << "Pixel: " << std::hex << std::setfill('0');
	out << std::setw(2) << static_cast<int>(p.blue);
	out << std::setw(2) << static_cast<int>(p.green);
	out << std::setw(2) << static_cast<int>(p.red);
	out << std::setw(2) << static_cast<int>(p.alpha);
	return out;
}

struct MonoColor
{
};


struct FourtyTwoColor
{
};

struct ThirtyTwoColor
{
};

template <typename T>
class BitMapPixelIter
{
public:
	// length is number of pixels
	BitMapPixelIter(uint8_t* start_buffer, uint32_t length)
	{
		_pixel_buffer = start_buffer;
		_pixel_cursor = start_buffer;
		_length = length;
		_current_index = 0;
		_end_reached = false;
	}
	
	// return the current pixel color and move the pointer forward. 
	PixelColor next();
	bool end_iteration() const;

private:
	uint8_t* _pixel_buffer;
	uint8_t* _pixel_cursor;
	uint32_t _length;
	uint32_t _current_index;
	bool _end_reached;
};

template <typename T>
bool BitMapPixelIter<T>::end_iteration() const
{
	return _end_reached;
}

template <typename T>
PixelColor BitMapPixelIter<T>::next()
{
	// 
}

template <>
PixelColor BitMapPixelIter<FourtyTwoColor>::next()
{
	PixelColor result;

	if(_end_reached)
	{
		result.blue = 0xFF;
		result.green = 0xFF;
		result.red = 0xFF;
		result.alpha = 0xFF;
	}
	else
	{
		result.blue = static_cast<uint8_t>(_pixel_cursor[0]);
		result.green = static_cast<uint8_t>(_pixel_cursor[1]);
		result.red = static_cast<uint8_t>(_pixel_cursor[2]);
		result.alpha = 0xFF;

		_pixel_cursor += 3;
		_current_index += 1;
		if(_current_index == _length)
		{
			_end_reached = true;
		}
	}
	return result;
}

template <>
PixelColor BitMapPixelIter<ThirtyTwoColor>::next()
{
	PixelColor result;
	if(_end_reached)
	{
		result.blue = 0xFF;
		result.green = 0xFF;
		result.red = 0xFF;
		result.alpha = 0xFF;
	}
	else
	{
		result.blue = static_cast<uint8_t>(_pixel_cursor[0]);
		result.green = static_cast<uint8_t>(_pixel_cursor[1]);
		result.red = static_cast<uint8_t>(_pixel_cursor[2]);
		result.alpha = static_cast<uint8_t>(_pixel_cursor[3]);

		_pixel_cursor += 4;
		_current_index += 1;
		if(_current_index == _length)
		{
			_end_reached = true;
		}
	}
	
	return result;
}


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
	uint16_t planes;
	uint16_t bits_per_pixel;
	uint32_t compression;
	uint32_t image_size;
	uint32_t xpixels_per_meter;
	uint32_t ypixels_per_meter;
	uint32_t colors_used;
	uint32_t important_colors;

	uint8_t* pixel_buffer;

	void get_rgba_pixel(uint32_t pixel_x, uint32_t pixel_y,
	                    uint8_t& r, uint8_t& b, uint8_t& g, uint8_t a)
	{
		
	}
	
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

		uint32_t offset = 0;

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
		bitmap.magic_number[0] = temp_buffer[0];
		bitmap.magic_number[1] = temp_buffer[1];
		offset += 2;

		// replace with cool deserialization lib. 
		uint8_t* buf = reinterpret_cast<uint8_t*>(temp_buffer+offset);
		bitmap.file_size = from_byte_array32(buf);

		offset = 0x0A;
		buf = reinterpret_cast<uint8_t*>(temp_buffer + offset);
		bitmap.data_offset = from_byte_array32(buf);

		std::memcpy(bitmap.header, temp_buffer, 13);
		std::memset(temp_buffer, 0, 13);

		for(int i = 0; i < 40; i++)
		{
			temp_buffer[i] = std::fgetc(file_handle);
		}

		offset = 0;
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


		WriteLine("Bits per pixel: " + std::to_string(bitmap.bits_per_pixel));
		// load pixel data.
		// pitch is the number of bytes in a row.
		if(bitmap.bits_per_pixel == 1 || bitmap.bits_per_pixel == 4 || bitmap.bits_per_pixel == 8 || bitmap.bits_per_pixel == 16)
		{
			throw std::runtime_error("Unsported bitmap bits per pixel " + std::to_string(bitmap.bits_per_pixel));
		}
		
		uint8_t bytes_per_pixel = bitmap.bits_per_pixel / 8;
		
		uint32_t pitch = bitmap.width * bytes_per_pixel;
		bitmap.pixel_buffer = new uint8_t[pitch * bitmap.height];

		// each row is padded to be 4 byte align, calculate padding count that occurs
		// at the end of each row. 
		int row_byte_padding_count = 0;
		if(bitmap.width % 4 != 0)
		{
			row_byte_padding_count = 4 - (bitmap.width % 4);
		}

		uint32_t pixel_byte_read_count = 0;

		// todo: move the current file pointer to be aligned with data offet
		std::fseek(file_handle, bitmap.data_offset, 0);

		WriteLine("Padding byte count: " + std::to_string(row_byte_padding_count));
		
		uint32_t dest_byte_index = 0;
		for(int i = 0; i < bitmap.height; ++i)
		{
			// for each row in the source, obtain each pixel and map it into a uint8_t byte array.
			for(int column = 0; column < pitch; column++)
			{
				// for 24 bit pixels perform direct copy.
				// color is in order of (blue, green, red).
				bitmap.pixel_buffer[dest_byte_index++] = static_cast<uint8_t>(std::fgetc(file_handle));
				pixel_byte_read_count++;
			}

			// read out the padding bytes to get to 4 byte allignment. 
			for(int column = 0; column < row_byte_padding_count; column++)
			{
				std::fgetc(file_handle);
				pixel_byte_read_count++;
			}

			if((dest_byte_index + row_byte_padding_count) % 4 != 0)
			{
				delete bitmap.pixel_buffer;
				throw std::runtime_error("Failed to parse scanline with correct padding: " + std::to_string(dest_byte_index) +
				                         " " + std::to_string(row_byte_padding_count));
			}
		}

		if(pixel_byte_read_count != pitch * bitmap.height)
		{
			throw std::runtime_error("Failed to load bitmap blame the developer: " + std::to_string(pixel_byte_read_count) + ", " + std::to_string(pitch * bitmap.height));
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
	WriteLine("x pixels per meter: " + std::to_string(bitmap.xpixels_per_meter));
	WriteLine("y pixels per meter: " + std::to_string(bitmap.ypixels_per_meter));
}

#endif
