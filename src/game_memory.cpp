
#include "game_memory.h"
#if _WIN32
#include <windows.h>
#endif


void init_memory_section(MemorySection& mem, size_t size)
{
	mem.base = (uint8_t*)malloc(size);
	mem.size = size; // in bytes.
	mem.used = 0;
}

void* push_mem(MemorySection& mem, size_t amount)
{
	void* result = NULL;
	if(mem.used + amount <= mem.size)
	{
		result = (void*)(mem.base + mem.used);
		mem.used += amount;
	}
	return result;
}

