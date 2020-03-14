#pragma once


#include <windows.h>
#include <string>

void WriteOut(const std::string& tmp);
void WriteLine(const std::string& tmp);
BOOL InitializeDebugConsole();
