#ifndef CUSTOM_APP_WINDOW_HELPER
#define CUSTOM_APP_WINDOW_HELPER

#include <windows.h>
#include <wingdi.h>
#include <string>


WNDCLASSEX CreateWindowEXHelperT(UINT style, WNDPROC update_func, HINSTANCE hInst,
                                const std::string menuName, const std::string className);


WNDCLASSEX CreateWindowEXHelper(UINT style, WNDPROC update_func, HINSTANCE hInst,
                                const TCHAR* menuName, const TCHAR* className);

#endif
