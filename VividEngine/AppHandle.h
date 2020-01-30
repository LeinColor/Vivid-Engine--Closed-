/**
	AppHandle.h

	Purpose: Store window handle and instance handle both to render.

	@author Lein
	@version 1.0 12/18/19
*/

#pragma once
#include "stdafx.h"

class AppHandle {
public:
	static HWND GetWindowHandle();
	static HINSTANCE GetInstanceHandle();
	static void SetWindowHandle(HWND handle);
	static void SetInstanceHandle(HINSTANCE handle);
private:
	static HWND hWnd;
	static HINSTANCE hInstance;
};