#ifndef CUSTOM_GAME_MEM_H
#define CUSTOM_GAME_MEM_H

#include <stdint.h>

class MemSection
{
public:
	void* start_ptr;
	uint32_t allocated;
	uint32_t max_length;
private:
};

void alloc_memory(void** mem_start, uint32_t size);

#endif
