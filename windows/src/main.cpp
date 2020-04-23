#include <windows.h>
#include <wingdi.h>
#include <timeapi.h>
#include <conio.h>

#include <stdlib.h>
#include <vector>
#include <iostream>
#include <fstream>
#include <stdio.h>

#include <string>
#include <tchar.h>
#include <sstream>

#include <stdint.h>
#include <set>

#include "herod_bitmap.h"

#include "performance.h"
#include "keyboard_updates.h"
#include "app_window_helper.h"

#include "game_memory.h"
#include "game_recording.h"
#include "game_module.h"
#include "game_state.h"
#include "game_controller.h"
#include "console_another.h"
#include "video.h"


#include "clanger.h"
#include "subprocess.hpp"

void my_clanger_printer(const std::string& msg)
{
	WriteLine(msg);
}

void temp_main();

std::string ReadInput(const std::string& prompt);


// used for copying bitmap data into a screen data.
void FillScreenDataWithBitmap(HBitmap& source, ScreenData& destination)
{
	uint8_t bytes_per_pixel = source.bits_per_pixel / 8;
	destination.bytesPerPixel = 4;
	resize_buffer(destination, source.width, source.height);
	if(bytes_per_pixel == 3)
	{
		uint32_t dest_i = 0; 
		// bitmaps are blue, green, red for 3 bytes per pixel
		// source pointer
		uint8_t* pixel = destination.buffer;

		WriteLine("Filling out Screend ata: " + std::to_string(bytes_per_pixel * source.width * source.height));

		for(uint32_t i = 0; i < bytes_per_pixel * source.width * source.height; i += 4)
		{
			// blue
			*pixel = source.pixel_buffer[i];
			++pixel;

			// green
			*pixel = source.pixel_buffer[i+1];
			++pixel;

			// red
			*pixel = source.pixel_buffer[i+2];
			++pixel;

			// alpha
			*pixel = 0xFF;
			++pixel;
		}
	}
	else
	{
		throw std::runtime_error("Invalid bytes per pixel count");
	}
}


static char valueConvertTable[16] =
	{
	 48,
	 49,
	 50,
	 51,
	 52,
	 53,
	 54,
	 55,
	 56,
	 57,
	 65,
	 66,
	 67,
	 68,
	 69,
	 70
	};


template <typename T>
std::string toHex(T value)
{
	std::stringstream stream;
	stream << "0x" << std::hex << value;
	std::string msg = stream.str();
	return msg;
}

std::string toHex(uint8_t* start, size_t length)
{
	std::stringstream stream;
	for(int i = 0; i < length; i++)
	{
		uint8_t highNibble = 0x0F & (start[i] >> 4);
		uint8_t lowNibble  = 0x0F & start[i];
		stream << valueConvertTable[highNibble] << valueConvertTable[lowNibble] << " ";
	}
	std::string msg = stream.str();
	return msg;
}

const static std::set<uint32_t> console_display_keys = {'M', 'N'};


class Console
{
public:
	// render_dest is the location of where the console should be rendered onto. 
	void render(ScreenData& render_dest);

	// note this does not allow for multiple  key pressed at the same time.
	// update to comput a keybord or game controller input. 
	void update(uint32_t keyCode);
	// contains all messages that have been typed into the console,
	// does not include the currently active typing message.
	std::vector<std::string> buffer_history;
	std::string current_message;
	ScreenData screen_data;

	uint32_t x_position;
	uint32_t y_position;
};

void Console::update(uint32_t keycode)
{
	// if keycode is a normal key value A-Z0-9, add to current active string
	// if keycode is a enter key add string to buffer_history.
	if(console_display_keys.find(keycode) != console_display_keys.end())
	{
		current_message += static_cast<char>(keycode);
	}
	else
	{
		// current_message
	}
}

void Console::render(ScreenData& render_dest)
{
	BlitScreenData(screen_data, render_dest, 0, 0);
}


// // base class for Widget / gui like objects that can be interactived with
// class Widget
// {
// public:
// 	virtual Update() = 0;
// private:
	
// };

// class VisibleWidget : public Widget
// {
// public:
// 	Update()
// 	{
// 		std::cout << "I'm a visible widet" << std::endl;
// 	}
	
// 	ScreenData* screen_data;
// };

static bool Running = true;
HINSTANCE hInst;

// todo(brandon): this could be a platform indepdenant function?

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

uint8_t* StartMemPrint = 0;
uint64_t StartAddress = 0;
bool DrawMemory = false;

LRESULT CALLBACK WndProc2(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
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

			if(DrawMemory)
			{
				HFONT hFont, hOldFont;
				hFont = (HFONT)GetStockObject(ANSI_FIXED_FONT);
				if(hOldFont = (HFONT)SelectObject(hdc, hFont))
				{

					StartAddress = reinterpret_cast<uintptr_t>(StartMemPrint);
					std::string Address = toHex(StartAddress);
					TextOut(hdc, 40, 40, Address.c_str(), Address.size());
					static uint8_t mem_print_width = 10;
					for(int i = 0; i < 13; i++)
					{
						uint8_t* platname = StartMemPrint + (i * mem_print_width);
						std::string valueStr = toHex(platname, mem_print_width);
						TextOut(hdc, 40, 65 + (15 * i), valueStr.c_str(), valueStr.size());
					}
				
					SelectObject(hdc, hOldFont);
				}
			}
			EndPaint(hWnd, &ps);
		} break;
		case WM_DESTROY:
		{
			PostQuitMessage(0);
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

	WNDCLASSEX wcex = CreateWindowEXHelperT(CS_HREDRAW | CS_VREDRAW, WndProc,
	                                       hInstance, "My Cool Dude", "My Cool Title");
	WNDCLASSEX wcexWind = CreateWindowEXHelperT(CS_HREDRAW | CS_VREDRAW, WndProc2,
	                                            hInstance, "Memory Dude", "Memory Window");
	if (!RegisterClassEx(&wcex))
	{
		MessageBox(NULL,
		           _T("Call to RegisterClassEx failed!"),
		           _T("Windows Desktop Guided Tour"),
		           NULL);

		return 1;
	}

	if(!RegisterClassEx(&wcexWind))
	{
		MessageBox(NULL,
		           _T("Call to registerclassex failed on mem with error " + GetLastError()),
		           _T("Windows deljfkl: "),
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
	                         "My Cool Dude",
	                         "My Cool Title",
	                         WS_OVERLAPPEDWINDOW,
	                         CW_USEDEFAULT, CW_USEDEFAULT,
	                         960, 540,
	                         NULL,
	                         NULL,
	                         hInstance,
	                         NULL
	                         );

	HWND memH = CreateWindow(
	                         "Memory Dude",
	                         "Memory Window",
	                         WS_OVERLAPPEDWINDOW,
	                         CW_USEDEFAULT, CW_USEDEFAULT,
	                         960, 540,
	                         NULL,
	                         NULL,
	                         hInstance,
	                         NULL);

	// clanger_State rootState;
	// rootState.next = NULL;
	
	// clanger_set_print(my_clanger_printer);

	// clanger_main();


	// 	ReadInput("Hello: ");


	// auto ret = subprocess::call({"dir"});
	// WriteLine("Ret: value: " + std::to_string(ret));

	// temp_main();

	if (!hWnd)
	{
		MessageBox(NULL,
		           _T("Call to CreateWindow failed!"),
		           _T("Windows Desktop Guided Tour"),
		           NULL);

		return 1;
	}

	if(!memH)
	{
		MessageBox(NULL,
		           _T("Failed to make memory window"),
		           _T("Hello world"),
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

	ScreenData testingScreenData;
	testingScreenData.bytesPerPixel = 4;
	testingScreenData.buffer = NULL;
	resize_buffer(testingScreenData, 50, 50);
	DrawRectangle(testingScreenData.buffer, 50, 50, 0, 0, 40, 40, 0xFF, 0, 0);

	Console main_console;
	main_console.screen_data.bytesPerPixel = 4;
	main_console.screen_data.buffer = NULL;
	resize_buffer(main_console.screen_data, 960 / 4, 540 / 4);

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
	// ShowWindow(memH, nCmdShow);

	MSG msg;
	// load custom game module 
	// Main message loop:
	GameState mahState;

	init_memory_section(mahState.platform_mem, 100);

	uint8_t * res = (uint8_t*)push_mem(mahState.platform_mem, 30);
	if(res == NULL)
	{
		Sleep(30000);
	}

	GameInput* mahInput = new GameInput();
	GameInputController* newKeyboard = &(mahInput->keyboard);
	GameInputControllerInit(newKeyboard);

	std::string PlatformIdent = "Windows";
	for(int i = 0; i < PlatformIdent.size(); i++)
	{
		res[i] = PlatformIdent[i];
	}

	int initRest = blueFuncs.GameInit(&mahState);
	if(initRest != 0)
	{
		WriteLine("Failed to init game, Exiting...");
		Sleep(30000);
		return 1;
	}
	if(mahState.module_mem.base != NULL)
	{
		StartMemPrint = mahState.module_mem.base;		
	}
	else
	{
		WriteLine("Failed to get module mem base, Exiting...");
		Sleep(20000);
		return 1;
	}
	// trigger a window update
	UpdateWindow(hWnd);
	UpdateWindow(memH);

	bool console_active = false;


	HBitmap tempBitmap;
	try
	{
		LoadBitmap("resources/fonts/cool_font.bmp", tempBitmap);
		WriteLine("Bitmap temp screen data");
		ScreenData temp_info;
		temp_info.buffer = NULL;
		temp_info.bytesPerPixel = 4;

		FillScreenDataWithBitmap(tempBitmap, temp_info);
		BlitScreenData(temp_info, main_console.screen_data, 0, 0);
	}
	catch(const std::runtime_error& e)
	{
		std::stringstream blah;
		blah << "@@@@Unable to load bitmap data@@: " << e.what();
		WriteLine(blah.str());
	}
	
	RECT new_rec;
	new_rec.left = 0;
	new_rec.top = 0;
	new_rec.right = 960;
	new_rec.bottom = 540;

	LARGE_INTEGER LastCounter = Win32GetWallClock();
	float SecondsElapsedForFrame = 0;
	bool RecordingStates = false;
	bool PlaybackInput = false;

	while(Running)
	{
		// using the specific windows classes and stuff, we need
		// to invaliate the paint region, so the WM_PAINT event is sent to our class.
		// using this we can also limite the amount of theings that need to be redrawn,
		// as well on when we need to perform a redraw.
		InvalidateRect(hWnd, &new_rec, true);
		if(DrawMemory)
		{
			InvalidateRect(memH, &new_rec, true);
		}
		
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
					bool IsDown  = ((msg.lParam & (1 << 31)) == 0);
					if(WasDown != IsDown)
					{
						if(VKCode == '1' && WasDown)
						{
							WriteLine("Activating console");
							console_active = !console_active;
						}
						if(!console_active)
						{
							if(VKCode == 'M' && WasDown)
							{
								DrawMemory = true;
							}
							else if(VKCode == 'N' && WasDown)
							{
								DrawMemory = false;
							}
							if(DrawMemory && VKCode == 'V' && WasDown)
							{
								StartMemPrint += 8;
							}
							else if(DrawMemory && VKCode == 'B' && WasDown)
							{
								StartMemPrint -= 8;							
							}
							if(VKCode == 'L' && WasDown)
							{
								BeginRecordingInput(&mahState);
								RecordingStates = true;
								WriteLine("Begin recording");
							}
							if(VKCode == 'P' && WasDown)
							{
								if(PlaybackInput)
								{
									PlaybackInput = false;
									ClearPlayback(&mahState);
								}
								else
								{
									WriteLine("Begin Replay");
									PlaybackInput = true;
								}
							}
						}
					}
					UpdateKeyboardInputs(msg, newKeyboard);
				} break;
				
				default:
				{
					
					TranslateMessage(&msg);
					DispatchMessageA(&msg);
				} break;
			}
		}

		mahInput->dtForFrame = SecondsElapsedForFrame;

		if(RecordingStates && !PlaybackInput)
		{
			AppendInputState(mahInput);
		}

		if(PlaybackInput)
		{
			GetNextInputStateUpdate(&mahState, mahInput);
		}

		// so basically we give the module reference to teh current screen, the key inputs that the user presed on the last frame
		// the state also corresponds to the current screen?
		initRest = blueFuncs.GameUpdate(&currentScreen,
		                                &mahState,
		                                mahInput);

		BlitScreenData(testingScreenData, currentScreen, 10, 10);


		if(console_active)
		{
			main_console.render(currentScreen);
		}


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
		LastCounter = EndCounter;
	}
	return (int) msg.wParam;
}


