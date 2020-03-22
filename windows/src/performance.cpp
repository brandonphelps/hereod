
#include <windows.h>
#include <stdint.h>

LARGE_INTEGER PerfCountFrequencyResult;
int64_t GlobalPerfCountFrequency;

void InitPerformance()
{
	QueryPerformanceCounter(&PerfCountFrequencyResult);
	GlobalPerfCountFrequency = PerfCountFrequencyResult.QuadPart;
}

LARGE_INTEGER Win32GetWallClock(void)
{
	LARGE_INTEGER Result;
	QueryPerformanceCounter(&Result);
	return Result;
}

float Win32GetSecondsElapsed(LARGE_INTEGER Start, LARGE_INTEGER End)
{
	float result = ((float)(End.QuadPart - Start.QuadPart) /
	                (float)GlobalPerfCountFrequency);
	return result;
}
