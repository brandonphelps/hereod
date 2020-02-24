// #include <iostream>
// #include <string>
// #include <windows.h>


// bool WIN_IsWindowsVistaOrGreater(void)
// {
// #ifdef __WINRT__
// 	return true;
// #else
// 	return false;
// #endif
// }

// class SDL_Window
// {
	
// };

// #define STYLE_BASIC (WS_CLIPSIBLINGS | WS_CLIPCHILDREN)

// int MyCreateWindow(HINSTANCE hinstance, int nCmdShow)
// {
// 	HWND hwnd, parent = NULL;
// 	DWORD style = STYLE_BASIC;
// 	int x, y;
// 	int w, h;
// 	HINSTANCE SDL_Instance;

// 	SDL_Instance = GetModuleHandle(NULL);

// 	hwnd = CreateWindow("My Cool App", "Hello World", STYLE_BASIC, 0, 0, 500, 500, NULL, NULL, hinstance, NULL);

// 	if(!hwnd)
// 	{
// 		return 0;
// 	}

// 	ShowWindow(hwnd, nCmdShow);
// 	UpdateWindow(hwnd);
// 	return 1;
// }


// //int main(int argc, char* argv[])
// int WINAPI WinMain(HINSTANCE hinst, HINSTANCE hPrevInst, LPSTR lpCmdLine, int nCmdShow)
// {
// 	std::cout << "hello world" << std::endl;

// 	MSG msg;

	
// 	SYSTEM_POWER_STATUS status;
// 	if(!GetSystemPowerStatus(&status))
// 	{
// 		std::cout << "failed to get power state" << std::endl;
// 	}
// 	else
// 	{
// 		std::cout << "Got a power state: " << std::to_string(status.SystemStatusFlag) << std::endl;
// 		std::cout << "Batt flag: " << std::to_string(status.BatteryFlag) << std::endl;
// 	}

// 	MyCreateWindow(hinst, nCmdShow);
	
// 	return 0;
// }
// HelloWindowsDesktop.cpp
// compile with: /D_UNICODE /DUNICODE /DWIN32 /D_WINDOWS /c

#include <windows.h>
#include <stdlib.h>
#include <string.h>
#include <tchar.h>

// Global variables

// The main window class name.
static TCHAR szWindowClass[] = _T("DesktopApp");

// The string that appears in the application's title bar.
static TCHAR szTitle[] = _T("Windows Desktop Guided Tour Application");

HINSTANCE hInst;

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
      500, 100,
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
   ShowWindow(hWnd,
      nCmdShow);
   UpdateWindow(hWnd);

   // Main message loop:
   MSG msg;
   while (GetMessage(&msg, NULL, 0, 0))
   {
      TranslateMessage(&msg);
      DispatchMessage(&msg);
   }

   return (int) msg.wParam;
}

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
   TCHAR greeting[] = _T("Hello, Windows desktop!");

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

      EndPaint(hWnd, &ps);
      break;
   case WM_DESTROY:
      PostQuitMessage(0);
      break;
   default:
      return DefWindowProc(hWnd, message, wParam, lParam);
      break;
   }

   return 0;
}

// // Global variable 
// #include <windows.h>

// HINSTANCE hinst; 
 
// // Function prototypes. 
 
// int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int); 
// int InitApplication(HINSTANCE); 
// int InitInstance(HINSTANCE, int); 

// LRESULT CALLBACK Win32MainWindowCallback(HWND Window, UINT message, WPARAM WParam, LPARAM lParam)
// {
// 	LRESULT result = 0;

// 	switch(message)
// 	{
// 		case WM_CLOSE:
// 		{
// 		}
// 		default:
// 		{
// 			result = DefWindowProcA(Window, message, WParam, lParam);
// 		}
// 	}

// 	return result;
// }
 
// // Application entry point. 
 
// int CALLBACK WinMain(HINSTANCE hinstance, HINSTANCE hPrevInstance, 
//     LPSTR lpCmdLine, int nCmdShow) 
// { 

// 	OutputDebugString("Hello World\n\r");
// 	return 23;

// 	WNDCLASSA WindowClass;

// 	WindowClass.style = CS_HREDRAW|CS_VREDRAW;
// 	WindowClass.lpfnWndProc = Win32MainWindowCallback;
// 	WindowClass.hInstance = hinstance;
// 	WindowClass.lpszClassName = "My cool app";

// 	if(RegisterClassA(&WindowClass))
// 	{
	
// 		MSG msg; 
	
 
// 		HWND my_window = CreateWindowExA( 
// 		                                 0,
// 		                                 WindowClass.lpszClassName,
// 		                                 "Sample",            // title-bar string 
// 		                                 WS_OVERLAPPEDWINDOW | WS_VISIBLE, // top-level window 
// 		                                 CW_USEDEFAULT,       // default horizontal position 
// 		                                 CW_USEDEFAULT,       // default vertical position 
// 		                                 CW_USEDEFAULT,       // default width 
// 		                                 CW_USEDEFAULT,       // default height 
// 		                                 (HWND) NULL,         // no owner window 
// 		                                 (HMENU) NULL,        // use class menu 
// 		                                 hinstance,           // handle to application instance 
// 		                                 (LPVOID) NULL);      // no window-creation data 
 
// 		if(my_window)
// 		{
			
// 			while(PeekMessage(&msg, 0,0,0,PM_REMOVE))
// 			{
// 				switch(msg.message)
// 				{
// 					case WM_QUIT:
// 					{
// 						break;
// 					}
// 					default:
// 					{
// 						TranslateMessage(&msg); 
// 						DispatchMessage(&msg); 
// 					}
// 				}
// 			}
// 		}
// 		return 1;
// 	}
// 	else
// 	{
// 		return 3;
// 	}
// 	return 2;
// } 
 
// BOOL InitInstance(HINSTANCE hinstance, int nCmdShow) 
// { 
//     HWND hwnd; 
 
//     // Save the application-instance handle. 
 
//     hinst = hinstance; 
 
//     // Create the main window. 
//     if (!hwnd) 
//         return FALSE; 
 
//     // Show the window and send a WM_PAINT message to the window 
//     // procedure. 
 
//     ShowWindow(hwnd, nCmdShow); 
//     UpdateWindow(hwnd); 
//     return TRUE; 
 
// } 
