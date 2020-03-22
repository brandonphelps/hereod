#ifndef WINDOWS_PERFORMANCE_H
#define WINDOWS_PERFORMANCE_H



#include <windows.h>

void InitPerformance();
LARGE_INTEGER Win32GetWallClock(void);
float Win32GetSecondsElapsed(LARGE_INTEGER Start, LARGE_INTEGER End);

#endif
