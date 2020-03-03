#include <windows.h>
#include <stdio.h>
#include <fcntl.h>
#include <io.h>
#include <iostream>
#include <fstream>
#include <string>

using namespace std;

HANDLE ConsoleStdout = NULL;
HANDLE ConsoleStdin = NULL;

void WriteOut(const std::string& tmp)
{
	if(ConsoleStdout != NULL)
	{
		WriteConsole(ConsoleStdout, reinterpret_cast<const VOID*>(tmp.c_str()), tmp.size(), NULL, NULL);
	}
	
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
	  // fprintf(tmp, "Failed to get STd handle\n");
	  return false;
  }

  // int SystemOutput =  _open_osfhandle(intptr_t(ConsoleOutput), _O_TEXT);
  // FILE *COutputHandle = _fdopen(SystemOutput, "w" );
  // *stdout = *COutputHandle;

  // fprintf(COutputHandle, "Hello World Yo\n\r");
  char tmp_buff[] = "Hello World\n";
  DWORD writtenChars;

  // WriteConsole(ConsoleStdout, reinterpret_cast<const VOID*>("hello world"), 5, &writtenChars, NULL);
  WriteConsole(ConsoleStdout, reinterpret_cast<const VOID*>("Hello World yo"), 7, NULL, NULL);

  WriteOut("\n\rWakka wakka\n\r");
  
  ConsoleStdin = GetStdHandle(STD_INPUT_HANDLE);
  if(ConsoleStdin == INVALID_HANDLE_VALUE || ConsoleStdin == NULL)
  {
	  return false;
  }


  char my_cool_buf[100];
  DWORD readCount;

  int tmp = ReadConsole(ConsoleStdin, reinterpret_cast<VOID*>(my_cool_buf), 10, &readCount, NULL);
  if(tmp)
  {
	  // WriteConsole(ConsoleStdout, reinterpret_cast<const VOID*>("Failed to read std in"), 10, &writtenChars, NULL);
	  WriteOut("Failed to read std in with status " + std::to_string(tmp));
	  WriteOut("Get last error: " + std::to_string(GetLastError()));
  }


  // setvbuf(stdout, NULL, _I0); //Redirect unbuffered STDERR to the console
  // HANDLE ConsoleError = GetStdHandle(STD_ERROR_HANDLE);
  // int SystemError = _open_osfhandle(intptr_t(ConsoleError), _O_TEXT);
  // FILE *CErrorHandle  = _fdopen(SystemError, "w" );
  // *stderr = *CErrorHandle;

  // setvbuf(stderr, NULL, _IONBF, 0);
  // //Redirect unbuffered STDIN to the console
  // HANDLE ConsoleInput = GetStdHandle(STD_INPUT_HANDLE);
  // int SystemInput = _open_osfhandle(intptr_t(ConsoleInput), _O_TEXT);
  // FILE *CInputHandle = _fdopen(SystemInput, "r" );

  // *stdin = *CInputHandle;
  // setvbuf(stdin, NULL, _IONBF, 0);
  // //make cout, wcout, cin, wcin,  wcerr, cerr, wclog and clog point to console as well
  // ios::sync_with_stdio(true);
  // fclose(tmp);

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
