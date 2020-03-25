
#include "game_memory.h"
#if _WIN32
#include <windows.h>
#endif


void alloc_memory(void** mem_start, uint32_t size)
{
	*mem_start = malloc(size);
}
