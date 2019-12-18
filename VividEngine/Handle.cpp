#include "Handle.h"
using namespace vivid;

HWND Handle::GetWindowHandle() {
	return hWnd;
}

HINSTANCE Handle::GetInstanceHandle() {
	return hInstance;
}