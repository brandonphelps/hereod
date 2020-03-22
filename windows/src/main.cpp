#include <windows.h>
#include <wingdi.h>
#include <timeapi.h>
#include <stdlib.h>

#include <vector>
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <conio.h>
#include <string>
#include <tchar.h>
#include <sstream>

#include <stdint.h>

#include "performance.h"
#include "keyboard_updates.h"

#include "game_module.h"
#include "game_state.h"
#include "game_controller.h"
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

class WindowDimension
{
public:
	uint32_t width;
	uint32_t height;
};

WindowDimension GetWindowDimension(HWND Window)
{
	WindowDimension result;

	RECT ClientRect;
	GetClientRect(Window, &ClientRect);
	result.width = ClientRect.right - ClientRect.left;
	result.height = ClientRect.bottom - ClientRect.top;
	return result;
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
			WindowDimension window_dim = GetWindowDimension(hWnd);

			StretchDIBits(hdc,
			              0, 0,
			              CurrentBuffer.video_buf->width,
			              CurrentBuffer.video_buf->height,
			              0, 0,
			              CurrentBuffer.video_buf->width, CurrentBuffer.video_buf->height,
			              CurrentBuffer.video_buf->buffer,
			              &CurrentBuffer.map_info, DIB_RGB_COLORS, SRCCOPY);

			// TextOut(hdc,
			//         5, 5,
			//         "Hello World", _tcslen("hello World"));

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


int CALLBACK WinMain(
                     HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR     lpCmdLine,
                     int       nCmdShow
)
{
	InitializeDebugConsole();
	InitPerformance();

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
	                         960, 540,
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

	uint32_t desired_scheduler_ms = 1;
	bool is_sleep_granular = (timeBeginPeriod(desired_scheduler_ms) == TIMERR_NOERROR);

	uint8_t MonitorRefreshHz = 120;


	HDC RefreshDC = GetDC(hWnd);
	int Win32RefreshRate = GetDeviceCaps(RefreshDC, VREFRESH);
	ReleaseDC(hWnd, RefreshDC);

	if(Win32RefreshRate > 1)
	{
		MonitorRefreshHz = Win32RefreshRate;
	}
	float GameUpdateHz = (MonitorRefreshHz / 2);
	float TargetSecondsPerFrame = 1.0f / (float)GameUpdateHz;
	ScreenData currentScreen;
	currentScreen.bytesPerPixel = 4;
	currentScreen.buffer = NULL;

	CurrentBuffer.video_buf = &currentScreen;
	CurrentBuffer.map_info.bmiHeader.biSize = sizeof(CurrentBuffer.map_info.bmiHeader);
	CurrentBuffer.map_info.bmiHeader.biPlanes = 1;
	CurrentBuffer.map_info.bmiHeader.biBitCount = 32;
	CurrentBuffer.map_info.bmiHeader.biCompression = BI_RGB;

	ResizeGraphicsBuffer(CurrentBuffer, 960, 540);

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
	GameState mahState;
	mahState.platform_data = new uint8_t[100];
	mahState.platform_size = 100;

	GameInput mahInput;
	GameInputController* newKeyboard = &(mahInput.keyboard);
	GameInputControllerInit(newKeyboard);

	std::string PlatformIdent = "Windows";
	for(int i =0; i < PlatformIdent.size(); i++)
	{
		mahState.platform_data[i] = PlatformIdent[i];
	}

	int initRest = blueFuncs.GameInit(&mahState);
	if(initRest != 0)
	{
		WriteLine("Failed to init game");
		return 1;
	}
	// trigger a window update
	UpdateWindow(hWnd);

	RECT new_rec;
	new_rec.left = 0;
	new_rec.top = 0;
	new_rec.right = 960;
	new_rec.bottom = 540;

	LARGE_INTEGER LastCounter = Win32GetWallClock();
	float SecondsElapsedForFrame = 0;

	while(Running)
	{
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
					UpdateKeyboardInputs(msg, newKeyboard);
				} break;
				
				default:
				{
					
					TranslateMessage(&msg);
					DispatchMessageA(&msg);
				} break;
			}
		}

		mahInput.dtForFrame = SecondsElapsedForFrame;

		// so basically we give the module reference to teh current screen, the key inputs that the user presed on the last frame
		// the state also corresponds to the current screen?
		initRest = blueFuncs.GameUpdate(&currentScreen,
		                                &mahState,
		                                &mahInput);


		// timing information to ensure a steady framerate.
		LARGE_INTEGER WorkCounter = Win32GetWallClock();
		float WorkSecondsElapsed = Win32GetSecondsElapsed(LastCounter, WorkCounter);
		SecondsElapsedForFrame = WorkSecondsElapsed;
		if(SecondsElapsedForFrame < TargetSecondsPerFrame)
		{
			if(is_sleep_granular)
			{
				DWORD sleepMS = (DWORD)(1000.0f * (TargetSecondsPerFrame -
				                                   SecondsElapsedForFrame));
				if(sleepMS > 0)
				{
					Sleep(sleepMS);
				}
			}

			while(SecondsElapsedForFrame < TargetSecondsPerFrame)
			{
				SecondsElapsedForFrame = Win32GetSecondsElapsed(LastCounter,
				                                                Win32GetWallClock());
			}
		}
		else
		{
			// missed frame rate
			WriteLine("Missed frame rate: " + std::to_string(SecondsElapsedForFrame) + ", " + std::to_string(TargetSecondsPerFrame));
		}

		LARGE_INTEGER EndCounter = Win32GetWallClock();
		float MSPerFrame = 1000.0f * Win32GetSecondsElapsed(LastCounter, EndCounter);
		WriteLine("MS Per Frame: " + std::to_string(SecondsElapsedForFrame));
		LastCounter = EndCounter;
	}
	return (int) msg.wParam;
}


