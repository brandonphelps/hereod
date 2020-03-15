// HelloWindowsDesktop.cpp
// compile with: /D_UNICODE /DUNICODE /DWIN32 /D_WINDOWS /c

#include <windows.h>
#include <stdlib.h>
#include <wingdi.h>
#include <vector>
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <conio.h>
#include <string>
#include <tchar.h>
#include <sstream>

#include <stdint.h>

#include "game_module.h"
#include "game_state.h"
#include "controller.h"
#include "console_another.h"
#include "video.h"

template <typename T>
std::string toHex(T value)
{
	std::stringstream stream;
	stream << "0x" << std::hex << value;
	std::string msg = stream.str();
	return msg;
}


// Global variables
// The main window class name.
static TCHAR szWindowClass[] = _T("My cool dude");

// The string that appears in the application's title bar.
static TCHAR szTitle[] = _T("My cool title");

static bool Running = true;

HINSTANCE hInst;

// todo(brandon): this could be a platform indepdenant function?
void resize_buffer(ScreenData& screendata, uint32_t new_width, uint32_t new_height)
{
	if(screendata.buffer != NULL)
	{
		WriteOut("Clearning out previously allocated buffer\n\r");
		delete screendata.buffer;
		screendata.buffer = NULL;
	}

	WriteOut("Resizing buffer\n\r");

	screendata.width = new_width;
	screendata.height = new_height;
	screendata.pitch = screendata.width * screendata.bytesPerPixel;

	WriteOut("Allocating " + std::to_string(screendata.pitch) + " * " + std::to_string(screendata.height) + "\n\r");
	screendata.buffer = new uint8_t[screendata.pitch * screendata.height];
}

struct win32_pixel_buffer
{
	ScreenData* video_buf;
	BITMAPINFO map_info;
};

win32_pixel_buffer CurrentBuffer;

void ResizeGraphicsBuffer(win32_pixel_buffer& pixel_buff, uint32_t new_width, uint32_t new_height)
{
	WriteOut("Reszing graphics buffer\n\r");
	resize_buffer(*(pixel_buff.video_buf), new_width, new_height);
	pixel_buff.map_info.bmiHeader.biWidth = new_width;
	pixel_buff.map_info.bmiHeader.biHeight = -1 * new_height;
	WriteOut("Finished with graphics buffer " + std::to_string(pixel_buff.map_info.bmiHeader.biWidth) + "\n\r");
}

// Forward declarations of functions included in this code module:
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

int CALLBACK WinMain(
                     HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR     lpCmdLine,
                     int       nCmdShow
)
{
	InitializeDebugConsole();


	WNDCLASSEX wcex = {};
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style          = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc    = WndProc;
	wcex.hInstance      = hInstance;
	wcex.hCursor        = LoadCursor(NULL, IDC_ARROW);
	wcex.lpszClassName  = szWindowClass;
	// wcex.hIcon          = LoadIcon(hInstance, IDI_APPLICATION);

	// wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName   = NULL;
	// wcex.hIconSm        = LoadIcon(wcex.hInstance, IDI_APPLICATION);

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


	ScreenData currentScreen;
	currentScreen.bytesPerPixel = 4;
	currentScreen.buffer = NULL;

	CurrentBuffer.video_buf = &currentScreen;
	CurrentBuffer.map_info.bmiHeader.biSize = sizeof(CurrentBuffer.map_info.bmiHeader);
	CurrentBuffer.map_info.bmiHeader.biPlanes = 1;
	CurrentBuffer.map_info.bmiHeader.biBitCount = 32;
	CurrentBuffer.map_info.bmiHeader.biCompression = BI_RGB;

	ResizeGraphicsBuffer(CurrentBuffer, 400, 500);

	ModuleFunctions blueFuncs;
	ModuleFunctions towerFuncs;

	LoadModule(blueFuncs, "bin/blue_d.dll");
	LoadModule(towerFuncs, "bin/tower_d.dll");

	// The parameters to ShowWindow explained:
	// hWnd: the value returned from CreateWindow
	// nCmdShow: the fourth parameter from WinMain
	ShowWindow(hWnd, nCmdShow);

	WriteOut("Starting Running while loop\n\r");

	MSG msg;

	// load custom game module 
	// Main message loop:

	GameInputController mahKeyboard;
	GameInputControllerInit(&mahKeyboard);
	GameState mahState;
	mahState.platformData = NULL;
	mahState.module_data = NULL;

	mahState.platformData = new uint8_t[100];
	std::string PlatformIdent = "Windows";
	for(int i =0; i < PlatformIdent.size(); i++)
	{
		mahState.platformData[i] = PlatformIdent[i];
	}

	int initRest = towerFuncs.GameInit(&mahState);
	if(initRest != 0)
	{
		WriteLine("Failed to init game");
		return 1;
	}

	towerFuncs.GameUpdate(0, &currentScreen, &mahState, &mahKeyboard);
	// trigger a window update
	UpdateWindow(hWnd);

	RECT new_rec;
	new_rec.left = 0;
	new_rec.top = 0;
	new_rec.right = 400;
	new_rec.bottom = 400;

	while(Running)
	{
		initRest = towerFuncs.GameUpdate(0, &currentScreen, &mahState, &mahKeyboard);

		if(mahState.module_data != NULL)
		{
			uint16_t* tmpP = reinterpret_cast<uint16_t*>(mahState.module_data);
			WriteLine("Current offset: " + std::to_string(tmpP[0]));
		}

		// using the specific windows classes and stuff, we need
		// to invaliate the paint region, so the WM_PAINT event is sent to our class.
		// using this we can also limite the amount of theings that need to be redrawn,
		// as well on when we need to perform a redraw.
		InvalidateRect(hWnd, &new_rec, true);

		while (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
		{
			switch(msg.message)
			{
				case WM_QUIT:
				{
					Running = false;
				} break;

				case WM_SYSKEYDOWN:
				case WM_SYSKEYUP:
				case WM_KEYDOWN:
				case WM_KEYUP:
				{
					uint32_t VKCode = (uint32_t)msg.wParam;
					bool WasDown = ((msg.lParam & (1 << 30)) != 0);
					bool IsDown = ((msg.lParam & (1 << 31)) == 0);
					// seems like this triggers on ups and downs
					if(WasDown != IsDown)
					{
						if(VKCode == 'W')
						{
							ProcessKeyMessage(&(mahKeyboard.MoveUp), IsDown);
						}
						if(VKCode == 'A')
						{
							ProcessKeyMessage(&(mahKeyboard.MoveLeft), IsDown);
						}
						if(VKCode == 'S')
						{
							ProcessKeyMessage(&(mahKeyboard.MoveDown), IsDown);
						}
						if(VKCode == 'D')
						{
							ProcessKeyMessage(&(mahKeyboard.MoveRight), IsDown);
						}
					}
				} break;
				
				default:
				{
					
					TranslateMessage(&msg);
					DispatchMessageA(&msg);
				} break;
			}
		}
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
	LRESULT Result = 0;

	HDC hdc;
	switch (message)
	{
		case WM_PAINT:
		{
			PAINTSTRUCT ps;
			hdc = BeginPaint(hWnd, &ps);
			// Here your application is laid out.
			// For this introduction, we just print out "Hello, Windows desktop!"
			// in the top left corner.
			// TextOut(hdc,
			//    5, 5,
			//    greeting, _tcslen(greeting));
			// End application-specific layout section.
			PatBlt(hdc, 0, 0, 40, 40, BLACKNESS);
			// xDest, yDest, DestWidth, DestHeight, xSrc, ySrc, SrcWidth, SrcHeight

			StretchDIBits(hdc,
			              10, 10,
			              10 + CurrentBuffer.video_buf->width,
			              10 + CurrentBuffer.video_buf->height,
			              0, 0,
			              CurrentBuffer.video_buf->width, CurrentBuffer.video_buf->height,
			              CurrentBuffer.video_buf->buffer,
			              &CurrentBuffer.map_info, DIB_RGB_COLORS, SRCCOPY);

			// for(int i = 0; i < message_log.get_message_count(); i++)
			// {

			// 	TextOut(hdc,
			// 	        // todo(brandon): how to get size of characters? in pixels?
			// 	        50, 40+(20 * i),
			// 	        message_log.messages[i],
			// 	        _tcslen(message_log.messages[i]));
			// }

			EndPaint(hWnd, &ps);
		} break;
		case WM_DESTROY:
		{
			PostQuitMessage(0);
			Running = false;
		} break;
		default:
		{
			Result = DefWindowProcA(hWnd, message, wParam, lParam);
		} break;
	}

	return Result;
}

