
#include <windows.h>
#include <stdint.h>
#include "console_another.h"

static LARGE_INTEGER PerfCountFrequencyResult;
static int64_t GlobalPerfCountFrequency;

void InitPerformance()
{
	QueryPerformanceFrequency(&PerfCountFrequencyResult);
	GlobalPerfCountFrequency = PerfCountFrequencyResult.QuadPart;
	WriteLine("Global PerfCount: " + std::to_string(GlobalPerfCountFrequency));
}

LARGE_INTEGER Win32GetWallClock(void)
{
	LARGE_INTEGER Result;
	QueryPerformanceCounter(&Result);
	return Result;
}

float Win32GetSecondsElapsed(LARGE_INTEGER Start, LARGE_INTEGER End)
{
	LARGE_INTEGER ElapsedMicroseconds;
	ElapsedMicroseconds.QuadPart = End.QuadPart - Start.QuadPart;
	ElapsedMicroseconds.QuadPart *= 1000000;
	ElapsedMicroseconds.QuadPart /= PerfCountFrequencyResult.QuadPart;
	return (float)(ElapsedMicroseconds.QuadPart / 1000000.f);
}
