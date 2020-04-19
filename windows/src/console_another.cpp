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
HANDLE ConsoleStderr = NULL;

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

// todo read about WriteConsoleInput
// 	INPUT_RECORD input_data;
// ReadConsoleInput(ConsoleStdin, 


std::string ReadInput(const std::string& prompt)
{
	WriteLine("Reading from std in");

	if(ConsoleStdin == NULL)
	{
		WriteLine("Failed to read from stdin");
		return "";
	}

	char buffer[4096];

	DWORD numberofCharsRead;

	ZeroMemory(buffer, sizeof(buffer));
	
	BOOL bsuccess = ReadConsole(ConsoleStdin, 
	                            reinterpret_cast<void*>(buffer),
	                            100,
	                            &numberofCharsRead,
	                            NULL);
	if(bsuccess)
	{
		WriteLine("Read in Success");
	}
	else
	{
		WriteLine("Read in failure");
	}

	return "";
}
	

BOOL InitializeDebugConsole()
{
	//Create a console for this application
  BOOL succ = AllocConsole(); //Redirect unbuffered STDOUT to the console
  if(!succ)
  {
	  return false;
  }

  // freopen("CONOUT$", "w" ,stderr);
  // freopen("CONIN$", "r" , stdin);


  SetConsoleTitle("Debug Console");


  if(ConsoleStdout == NULL)
  {
	  ConsoleStdout = GetStdHandle(STD_OUTPUT_HANDLE);
  }
  
  if(ConsoleStdout == INVALID_HANDLE_VALUE || ConsoleStdout == NULL)
  {
	  WriteLine("Failed to std output handle");
	  return false;
  }

  SetConsoleTextAttribute(ConsoleStdout, FOREGROUND_GREEN | FOREGROUND_BLUE);


  if(ConsoleStderr = NULL)
  {
	  ConsoleStderr = GetStdHandle(STD_ERROR_HANDLE);
  }

  if(ConsoleStderr == INVALID_HANDLE_VALUE || ConsoleStderr == NULL)
  {


	  ConsoleStderr = CreateFileA(
	                              "stderror.txt",
	                              GENERIC_WRITE,
	                              FILE_SHARE_READ,
	                              NULL,
	                              CREATE_ALWAYS,
	                              FILE_ATTRIBUTE_NORMAL,
	                              NULL);
	  BOOL success = SetStdHandle(STD_ERROR_HANDLE, ConsoleStderr);
	  if(success)
	  {
		  WriteLine("Sending stderr to stderror.txxt");
	  }
	  else
	  {
		  WriteLine("Failed to get sTD ERROR HANDLE: " + std::to_string(GetLastError()));
		  WriteLine("Failed to set stderro handle");
		  return false;
	  }
  }




  if(ConsoleStdin = NULL)
  {
	  ConsoleStdin = GetStdHandle(STD_INPUT_HANDLE);
  }

  if(ConsoleStdin == INVALID_HANDLE_VALUE || ConsoleStdin == NULL)
  {
	  WriteLine("Failed to get sTD INPUT HANDLE: " + std::to_string(GetLastError()));
	  ConsoleStderr = CreateFileA(
	                              "stdin.txt",
	                              GENERIC_READ,
	                              FILE_SHARE_WRITE,
	                              NULL,
	                              CREATE_ALWAYS,
	                              FILE_ATTRIBUTE_NORMAL,
	                              NULL);
	  BOOL success = SetStdHandle(STD_INPUT_HANDLE, ConsoleStdin);
	  if(success)
	  {
		  WriteLine("Sending stderr to stdin.txt");
	  }
	  else
	  {
		  WriteLine("Failed to get STD INPUT HANDLE: " + std::to_string(GetLastError()));
		  WriteLine("Failed to set stdin handle");
		  return false;
	  }
	  

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
