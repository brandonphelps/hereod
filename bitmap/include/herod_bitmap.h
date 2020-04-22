
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

	uint8_t info_header[39];
	
private:
};

// appears bitmaps are little endian. 
uint32_t from_byte_array(uint8_t* pointer)
{
	uint32_t result = 0;
	result = ((uint64_t)pointer[0]) | ((uint64_t)pointer[1]) << 8 | ((uint64_t)pointer[2]) << 16 | ((uint64_t)pointer[3]) << 24;
	// result = 0xFF(pointer[3] << 24) | ((uint32_t)pointer[2] << 16) | ((uint32_t)pointer[1]) << 8 | pointer[0];
	return result;
}

void print_byte_array(uint8_t* pointer, uint32_t size)
{
	std::stringstream stream; 
	for(int i = 0; i < size; i++)
	{
		stream << std::hex << pointer[i] << std::dec;
	}
	WriteLine(stream.str());
}

// reads file and loads files contents into bitmap
void LoadBitmap(const std::string& filepath, HBitmap& bitmap)
{
	std::FILE* file_handle = fopen(filepath.c_str(), "rb");
	char temp_buffer[2000];
	std::memset(temp_buffer, 0, 2000);

	if(file_handle)
	{
		// header
		for(int i = 0; i < 13; i++)
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
		bitmap.file_size = from_byte_array(buf);
		buf = reinterpret_cast<uint8_t*>(temp_buffer + 2 + 4+ 6);
		bitmap.data_offset = from_byte_array(buf);
		std::memcpy(bitmap.header, temp_buffer, 13);
		std::memset(temp_buffer, 0, 13);
		for(int i = 0; i < 0x35 - 0x0E; i++)
		{
			temp_buffer[i] = std::fgetc(file_handle);
		}

		
	}
	else
	{
		throw std::runtime_error("Failed to open file: " + filepath);
	}

	WriteLine("BITMAP loaded");
	WriteLine("File size: " + std::to_string(bitmap.file_size));
	WriteLine("Data Offset: " + std::to_string(bitmap.data_offset));
}

#endif
