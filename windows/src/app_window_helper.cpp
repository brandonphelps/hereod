
#include "app_window_helper.h"
#include "console_another.h"


#include <string>

static std::string windowsNames[10];
static std::string windowsMenus[10];
static uint8_t numWindows = 0;


WNDCLASSEX CreateWindowEXHelperT(UINT style, WNDPROC update_func, HINSTANCE hInst,
                                const std::string menuName, const std::string className)
{
	WriteLine("Calling str version of window helper " + className);
	windowsNames[numWindows] = menuName;
	windowsMenus[numWindows] = className;
	WNDCLASSEX tmp = CreateWindowEXHelper(style, update_func,
	                                      hInst, windowsMenus[numWindows].c_str(),
	                                      windowsNames[numWindows].c_str());
	numWindows += 1;
	return tmp;
}


WNDCLASSEX CreateWindowEXHelper(UINT style, WNDPROC update_func,
                                HINSTANCE hInst, const TCHAR* menuName,
                                const TCHAR* className)
{
	WNDCLASSEX wnd = {};
	wnd.style = style;
	wnd.cbSize = sizeof(WNDCLASSEX);
	wnd.lpfnWndProc = update_func;
	wnd.hInstance = hInst;
	wnd.hCursor = LoadCursor(NULL, IDC_ARROW);
	wnd.lpszClassName = className;
	wnd.lpszMenuName = menuName;

	return wnd;
}
