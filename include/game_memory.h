#ifndef CUSTOM_GAME_MEM_H
#define CUSTOM_GAME_MEM_H

#include <stdint.h>

class MemorySection
{
public:
	MemorySection()
	{
		base = NULL;
		used = 0;
		size = 0;
	}
	uint8_t* base; // start of the area.
	size_t used;
	size_t size;
private:
};

#define AllocObj(MemSec, obj_type) reinterpret_cast<obj_type*>(push_mem(MemSec, sizeof(obj_type)))
#define AllocArray(MemSec, obj_type, count) reinterpret_cast<obj_type*>(push_mem(MemSec, sizeof(obj_type)*count))

void init_memory_section(MemorySection& mem, size_t size);
// amount in bytes.
void* push_mem(MemorySection& mem, size_t amount);

#endif
