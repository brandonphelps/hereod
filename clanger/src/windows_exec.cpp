


#include <windows.h>
#include <tchar.h>
#include <stdio.h>
#include <strsafe.h>

#include "console_another.h"

HANDLE g_hChildStd_IN_Rd = NULL;
HANDLE g_hChildStd_IN_Wr = NULL;
HANDLE g_hChildStd_OUT_Rd = NULL;
HANDLE g_hChildStd_OUT_Wr = NULL;



void CreateChildProcess()
{

	WriteLine("Calling create child process, echo world");

	
	TCHAR szCmdLine[]=TEXT("echo world");
	PROCESS_INFORMATION piProcInfo;
	STARTUPINFO siStartInfo;
	BOOL bSuccess = FALSE;



	ZeroMemory(&piProcInfo, sizeof(PROCESS_INFORMATION));

	ZeroMemory(&siStartInfo, sizeof(STARTUPINFO));
	siStartInfo.cb = sizeof(STARTUPINFO);
	siStartInfo.hStdError = g_hChildStd_OUT_Wr;
	siStartInfo.hStdOutput = g_hChildStd_OUT_Wr;
	siStartInfo.hStdInput = g_hChildStd_IN_Rd;

	siStartInfo.dwFlags |= STARTF_USESTDHANDLES;


	bSuccess = CreateProcess(NULL,
	                         szCmdLine, // command line
	                         NULL,    // process secuirty attributes
	                         NULL,    // primary thread security attributes
	                         TRUE,   // handles are inherited
	                         0, // creation flags
	                         NULL, // use paren'ts enivonrment
	                         NULL, // use parent's current directory
	                         &siStartInfo, // STARUPINFO pointer
	                         &piProcInfo);

	if(!bSuccess)
	{
		WriteLine("Failed CreateProcess");
	}
	else
	{
		CloseHandle(piProcInfo.hProcess);
		CloseHandle(piProcInfo.hThread);
		CloseHandle(g_hChildStd_OUT_Wr);
		CloseHandle(g_hChildStd_IN_Rd);
	}
}

void ReadFromPipe(void)
{
	DWORD dwRead, dwWritten;
	CHAR chBuf[4096];
	BOOL bSuccess = FALSE;
	HANDLE hParentStdOut = GetStdHandle(STD_OUTPUT_HANDLE);

	for(;;)
	{
		bSuccess = ReadFile(g_hChildStd_OUT_Rd, chBuf, 4096, &dwRead, NULL);
		if(!bSuccess || dwRead == 0)
		{
			break;
		}
		std::string msg(chBuf);

		WriteLine(msg);
	}
}


void temp_main()
{
	SECURITY_ATTRIBUTES saAttr;

	saAttr.nLength = sizeof(SECURITY_ATTRIBUTES);
	saAttr.bInheritHandle = TRUE;
	saAttr.lpSecurityDescriptor = NULL;

	if(!CreatePipe(&g_hChildStd_OUT_Rd, &g_hChildStd_OUT_Wr, &saAttr, 0))
	{
		WriteLine("Unable to make stdout rd createpipe");
		return;
	}
	

	if(!SetHandleInformation(g_hChildStd_OUT_Rd, HANDLE_FLAG_INHERIT, 0))
	{
		WriteLine("Stdout SetHandle Information");
		return;
	}

	if(!CreatePipe(&g_hChildStd_IN_Rd, &g_hChildStd_IN_Wr, &saAttr, 0))
	{
		WriteLine("Uanble to create stdin CreatePipe");
		return;
	}

	if(!SetHandleInformation(g_hChildStd_IN_Wr, HANDLE_FLAG_INHERIT, 0))
	{
		return;
	}



	CreateChildProcess();


	WriteLine("Creaing from child process contents");
	ReadFromPipe();
}
