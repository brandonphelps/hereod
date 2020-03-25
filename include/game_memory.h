#ifndef CUSTOM_GAME_MEM_H
#define CUSTOM_GAME_MEM_H

#include <stdint.h>

class MemorySection
{
public:
	uint8_t* base; // start of the area.
	size_t used;
	size_t size;
private:
};

#define AllocObj(MemSec, obj_type) (obj_type*)push_mem(MemSec, sizeof(obj_type))

void init_memory_section(MemorySection& mem, size_t size);
// amount in bytes.
void* push_mem(MemorySection& mem, size_t amount);

#endif
