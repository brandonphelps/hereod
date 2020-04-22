
#ifndef H_CUST_BITMAP_H
#define H_CUST_BITMAP_H

#include <iostream>
// #include <fstream> // can't pass a c string into std::ifstream constructor
// must be using wrong compiler flags or something.
#include <cstdio>

class HBitmap
{
public:
	uint8_t header[13];
private:
};

void print_byte_array(uint8_t* pointer, uint32_t size)
{
	for(int i = 0; i < size; i++)
	{
		std::cout << std::hex << pointer[i] << std::dec;
	}
	std::cout << std::endl;
}

// reads file and loads files contents into bitmap
void LoadBitmap(const std::string& filepath, HBitmap& bitmap)
{
	std::FILE* file_handle = fopen(filepath.c_str(), "rb");
	char temp_buffer[2000];
	std::memset(temp_buffer, 0, 2000);

	if(file_handle)
	{
		for(int i = 0; i < 13; i++)
		{
			temp_buffer[i] = std::fgetc(file_handle);
		}
	}
	else
	{
		throw std::runtime_error("Failed to open file: " + filepath);
	}

	print_byte_array(reinterpret_cast<uint8_t*>(temp_buffer), 13);
}

#endif
