// HelloWindowsDesktop.cpp
// compile with: /D_UNICODE /DUNICODE /DWIN32 /D_WINDOWS /c

#include <windows.h>
#include <stdlib.h>
#include <vector>
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <conio.h>
#include <string>
#include <tchar.h>


#include "console_another.h"

// Global variables

// The main window class name.
static TCHAR szWindowClass[] = _T("My cool dude");

// The string that appears in the application's title bar.
static TCHAR szTitle[] = _T("My cool title");

static int counter = 0; 

static bool Running = true;

class Holder
{
public:

	~Holder()
	{
		for(int i = 0; i < messages.size(); i++)
		{
			delete messages[i];
		}
	}

	std::vector<TCHAR*> messages;
	int message_index;

	void add_message(const std::string& message)
	{
		TCHAR* tmp_One = new TCHAR[3000];
		for(int i = 0; i < message.size(); i++)
		{
			tmp_One[i] = message[i];
		}
		tmp_One[message.size()] = '\0';
		messages.push_back(tmp_One);
		message_index++;
	}

	int get_message_count() const
	{
		return messages.size();
	}
};


HINSTANCE hInst;

Holder message_log;

// Forward declarations of functions included in this code module:
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

int CALLBACK WinMain(
                     HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR     lpCmdLine,
                     int       nCmdShow
)
{
   WNDCLASSEX wcex;

   wcex.cbSize = sizeof(WNDCLASSEX);
   wcex.style          = CS_HREDRAW | CS_VREDRAW;
   wcex.lpfnWndProc    = WndProc;
   wcex.cbClsExtra     = 0;
   wcex.cbWndExtra     = 0;
   wcex.hInstance      = hInstance;
   wcex.hIcon          = LoadIcon(hInstance, IDI_APPLICATION);
   wcex.hCursor        = LoadCursor(NULL, IDC_ARROW);
   wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
   wcex.lpszMenuName   = NULL;
   wcex.lpszClassName  = szWindowClass;
   wcex.hIconSm        = LoadIcon(wcex.hInstance, IDI_APPLICATION);

   if (!RegisterClassEx(&wcex))
   {
      MessageBox(NULL,
         _T("Call to RegisterClassEx failed!"),
         _T("Windows Desktop Guided Tour"),
         NULL);

      return 1;
   }


   //


   message_log.add_message("bee");
   message_log.add_message("world");
   message_log.add_message("toges");
   message_log.add_message("mec");
   message_log.add_message("goes");
   // Store instance handle in our global variable
   hInst = hInstance;

   // The parameters to CreateWindow explained:
   // szWindowClass: the name of the application
   // szTitle: the text that appears in the title bar
   // WS_OVERLAPPEDWINDOW: the type of window to create
   // CW_USEDEFAULT, CW_USEDEFAULT: initial position (x, y)
   // 500, 100: initial size (width, length)
   // NULL: the parent of this window
   // NULL: this application does not have a menu bar
   // hInstance: the first parameter from WinMain
   // NULL: not used in this application
   HWND hWnd = CreateWindow(
      szWindowClass,
      szTitle,
      WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, CW_USEDEFAULT,
      500, 400,
      NULL,
      NULL,
      hInstance,
      NULL
   );

   if (!hWnd)
   {
      MessageBox(NULL,
         _T("Call to CreateWindow failed!"),
         _T("Windows Desktop Guided Tour"),
         NULL);

      return 1;
   }

   // The parameters to ShowWindow explained:
   // hWnd: the value returned from CreateWindow
   // nCmdShow: the fourth parameter from WinMain
   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);


   InitializeDebugConsole();



   WriteOut("Starting Running while loop\n\r");

   // Main message loop:
   MSG msg;


   while(Running)
   {
	   fprintf(stdout, "Enter an integer!\n");	   
	   counter += 1;
	   if(counter % 10000 == 0)
	   {
		   //message_log.add_message("Whoooo ");
	   }


	   while (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
	   {
		   
		   TranslateMessage(&msg);
		   DispatchMessage(&msg);

	   }

   }

   return (int) msg.wParam;
}
static TCHAR greeting[] = _T("Hello, Windows desktop!");




//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
   PAINTSTRUCT ps;
   HDC hdc;



   switch (message)
   {
   case WM_PAINT:
      hdc = BeginPaint(hWnd, &ps);

      // Here your application is laid out.
      // For this introduction, we just print out "Hello, Windows desktop!"
      // in the top left corner.
      TextOut(hdc,
         5, 5,
         greeting, _tcslen(greeting));
      // End application-specific layout section.

      for(int i = 0; i < message_log.get_message_count(); i++)
      {

	      TextOut(hdc,
	              // todo(brandon): how to get size of characters? in pixels?
	              50, 40+(20 * i),
	              message_log.messages[i],
	              _tcslen(message_log.messages[i]));
      }

      EndPaint(hWnd, &ps);
      break;
   case WM_DESTROY:
      PostQuitMessage(0);
      Running = false;
      break;
   default:
      return DefWindowProc(hWnd, message, wParam, lParam);
      break;
   }

   return 0;
}

