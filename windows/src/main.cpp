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

#include <stdint.h>

#include "console_another.h"
#include "video.h"

// Global variables
// The main window class name.
static TCHAR szWindowClass[] = _T("My cool dude");

// The string that appears in the application's title bar.
static TCHAR szTitle[] = _T("My cool title");

static int counter = 0; 

static bool Running = true;

HINSTANCE hInst;

class PixelBuffer
{
public:
	PixelBuffer() : width(0), height(0), buffer(NULL)
	{
		
	}

	~PixelBuffer()
	{
		WriteOut("Pixel Buffer Deconstructor");
		if(buffer != NULL)
		{
			delete buffer;
			buffer = NULL;
		}
	}

	uint32_t width; // in pixels
	uint32_t height; // in pixels
	uint32_t* buffer;

	void DrawRect(uint32_t start_row, uint32_t start_col, uint32_t end_row, uint32_t end_col, uint32_t color_mask)
	{
		// todo(brandon): bounds checking


		for(uint32_t row = start_row; row < end_row; row++)
		{
			for(uint32_t col = start_col; col < end_col; col++)
			{
				buffer[row * col + width] = color_mask;
			}
		}
	}
};


void resize_buffer(ScreenData& screendata, uint32_t new_width, uint32_t new_height)
{
	if(screendata.buffer != NULL)
	{
		delete screendata.buffer;
		screendata.buffer = NULL;
	}

	WriteOut("Resizing buffer\n\r");

	screendata.width = new_width;
	screendata.height = new_height;
	screendata.pitch = screendata.width * screendata.bytesPerPixel;
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
	WriteOut("Finished with graphics buffer\n\r");
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

	ScreenData currentScreen;
	currentScreen.bytesPerPixel = 4;
	currentScreen.buffer = NULL;

	CurrentBuffer.video_buf = &currentScreen;
	CurrentBuffer.map_info.bmiHeader.biSize = sizeof(CurrentBuffer.map_info.bmiHeader);
	CurrentBuffer.map_info.bmiHeader.biPlanes = 1;
	CurrentBuffer.map_info.bmiHeader.biBitCount = 32;
	CurrentBuffer.map_info.bmiHeader.biCompression = BI_RGB;

	ResizeGraphicsBuffer(CurrentBuffer, 200, 200);


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



	WriteOut("Starting Running while loop\n\r");

	// Main message loop:
	MSG msg;
	while(Running)
	{
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
		{
			hdc = BeginPaint(hWnd, &ps);
		   
			// Here your application is laid out.
			// For this introduction, we just print out "Hello, Windows desktop!"
			// in the top left corner.
			// TextOut(hdc,
			//    5, 5,
			//    greeting, _tcslen(greeting));
			// End application-specific layout section.

			// PatBlt(hdc, 0, 0, 100, 100, BLACKNESS);
			// xDest, yDest, DestWidth, DestHeight, xSrc, ySrc, SrcWidth, SrcHeight
			StretchDIBits(hdc, 10, 10, 40, 40, 0, 0, 60, 60, CurrentBuffer.video_buf->buffer, &CurrentBuffer.map_info, DIB_RGB_COLORS, SRCCOPY);
			//StretchDIBits(hdc, 10, 10, 40, 40, 0, 0, 60, 60, display, &map_info, DIB_RGB_COLORS, SRCCOPY);


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
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
	}

	return 0;
}

