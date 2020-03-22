#include <windows.h>
#include <stdio.h>
#include <fcntl.h>
#include <io.h>
#include <iostream>
#include <fstream>
#include <string>

using namespace std;

// FILE *fDummy;
// freopen_s(&fDummy, "CONIN$", "r", stdin);
// freopen_s(&fDummy, "CONOUT$", "w", stderr);
// freopen_s(&fDummy, "CONOUT$", "w", stdout);


HANDLE ConsoleStdout = NULL;
HANDLE ConsoleStdin = NULL;

void WriteOut(const std::string& tmp)
{
	if(ConsoleStdout != NULL)
	{
		WriteConsole(ConsoleStdout, reinterpret_cast<const VOID*>(tmp.c_str()), tmp.size(), NULL, NULL);
	}
}

void WriteLine(const std::string& tmp)
{
	WriteOut(tmp + "\n\r");
}

BOOL InitializeDebugConsole()
{
	//Create a console for this application
  BOOL succ = AllocConsole(); //Redirect unbuffered STDOUT to the console
  if(!succ)
  {
	  return false;
  }

  if(ConsoleStdout == NULL)
  {
	  ConsoleStdout = GetStdHandle(STD_OUTPUT_HANDLE);
  }
  
  if(ConsoleStdout == INVALID_HANDLE_VALUE || ConsoleStdout == NULL)
  {
	  return false;
  }

  // int SystemOutput =  _open_osfhandle(intptr_t(ConsoleOutput), _O_TEXT);
  // FILE *COutputHandle = _fdopen(SystemOutput, "w" );
  // *stdout = *COutputHandle;
  return true;
}

// void ShutdownDebugConsole(void)
// {
// 	//Write "Press any key to exit"
// 	HANDLE ConsoleOutput = GetStdHandle(STD_OUTPUT_HANDLE);
// 	DWORD CharsWritten;
	
// 	WriteConsole(ConsoleOutput, "\nPress any key to exit", 22, &CharsWritten, 0);
	
// 	//Disable line-based input mode so we can get a	single character
// 	HANDLE ConsoleInput = GetStdHandle(STD_INPUT_HANDLE);
// 	SetConsoleMode(ConsoleInput, 0);
// 	//Read a single character
// 	TCHAR InputBuffer;
// 	DWORD CharsRead;
// 	ReadConsole(ConsoleInput, &InputBuffer, 1, &CharsRead, 0);
// }
