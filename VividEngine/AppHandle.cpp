/**
	AppHandle.cpp

	Purpose: Store window handle and instance handle both to render.

	@author Lein
	@version 1.0 12/18/19
*/

#include "AppHandle.h"
using namespace vivid;

// Forward declaration for static member variable
HWND AppHandle::hWnd;
HINSTANCE AppHandle::hInstance;
HWND AppHandle::GetWindowHandle()
{
	return hWnd;
}

HINSTANCE AppHandle::GetInstanceHandle()
{
	return hInstance;
}

void AppHandle::SetWindowHandle(HWND handle)
{
	hWnd = handle;
}

void AppHandle::SetInstanceHandle(HINSTANCE handle)
{
	hInstance = handle;
}