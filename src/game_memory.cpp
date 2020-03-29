
#include "game_memory.h"
#if _WIN32
#include <windows.h>
#endif


void init_memory_section(MemorySection& mem, size_t size)
{
#if _WIN32
	mem.base = (uint8_t*)VirtualAlloc(0, size, MEM_RESERVE|MEM_COMMIT, PAGE_READWRITE);
#else
	mem.base = (uint8_t*)malloc(size);
#endif
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

