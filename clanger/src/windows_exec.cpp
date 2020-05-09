


#include <windows.h>
#include <tchar.h>
#include <stdio.h>
#include <strsafe.h>

#include "console_another.h"

HANDLE g_hChildStd_IN_Rd = NULL;
HANDLE g_hChildStd_IN_Wr = NULL;
HANDLE g_hChildStd_OUT_Rd = NULL;
HANDLE g_hChildStd_OUT_Wr = NULL;
HANDLE g_hChildStd_ERR_Rd = NULL;
HANDLE g_hChildStd_ERR_Wr = NULL;

class ScopeHolder
{
public:
	ScopeHolder(char* obj)
	{
		// check if obj is null?
		_p = obj;
	}

	~ScopeHolder()
	{
		if(_p)
		{
			delete _p;
		}
		_p = NULL;
	}


	char* value()
	{
		return _p;
	}
	
private:
	char* _p;
};

bool CreateChildProcess(const std::string& commandline)
{

	WriteLine("Calling create child process, " + commandline);

	ScopeHolder p(new char[commandline.size()+1]);
	char *szCmdLine = p.value();

	std::memcpy(szCmdLine, commandline.c_str(), commandline.size());
	szCmdLine[commandline.size()] = '\0';
	
	std::string tmper(szCmdLine);

	WriteLine(tmper);
	
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
		CloseHandle(g_hChildStd_ERR_Wr);
		CloseHandle(g_hChildStd_IN_Rd);

	}
	return bSuccess;
}

void WriteToPipe(const std::string& msg)
{
	WriteLine("Start from write to pipe");
	DWORD dwRead, dwWritten;
	CHAR chBuf[4096];

	BOOL bSuccess = FALSE;

	for(;;)
	{
		bSuccess = WriteFile(g_hChildStd_IN_Wr, msg.c_str(), msg.size(),
		                     &dwWritten, NULL);
		if(!bSuccess)
		{
			break;
		}
	}
	WriteLine("End write to pipe");
}

void ReadFromPipe(void)
{
	WriteLine("Start Read from pipe");
	DWORD dwRead, dwWritten;
	CHAR chBuf[4096];
	BOOL bSuccess = FALSE;

	for(;;)
	{
		WriteLine("REading from subprocess child pipe");
		bSuccess = ReadFile(g_hChildStd_OUT_Rd, chBuf, 4096, &dwRead, NULL);
		if(!bSuccess || dwRead == 0)
		{
			break;
		}
		std::string msg(chBuf);

		WriteLine(msg);
	}
	WriteLine("End read of line");
}

void ReadFromPipeErr(void)
{
	WriteLine("Start Read from pipe ERRR");
	DWORD dwRead, dwWritten;
	CHAR chBuf[40];
	BOOL bSuccess = FALSE;

	for(;;)
	{
		bSuccess = ReadFile(g_hChildStd_ERR_Rd, chBuf, 40, &dwRead, NULL);
		if(!bSuccess || dwRead == 0)
		{
			break;
		}
		std::string msg(chBuf);

		WriteLine(msg);
	}
	WriteLine("End read of line");
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

	if(!CreatePipe(&g_hChildStd_ERR_Rd, &g_hChildStd_ERR_Wr, &saAttr, 0))
	{
		WriteLine("Unable to make stdout rd createpipe");
		return;
	}

	if(!SetHandleInformation(g_hChildStd_ERR_Rd, HANDLE_FLAG_INHERIT, 0))
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

	bool success = CreateChildProcess("echo hello");
	if(success)
	{
		// WriteToPipe("Heloo World");
		WriteLine("Creaing from child process contents");
		ReadFromPipe();
	}
	else
	{
		// ReadFromPipeErr();
	}

	CloseHandle(g_hChildStd_OUT_Wr);
	CloseHandle(g_hChildStd_ERR_Wr);
	CloseHandle(g_hChildStd_IN_Rd);
}
 
