/**
	AppHandle.h

	Purpose: Store window handle and instance handle both to render.

	@author Lein
	@version 1.0 12/18/19
*/

#pragma once
#include "stdafx.h"

namespace vivid {
	class AppHandle {
	public:
		HWND GetWindowHandle();
		HINSTANCE GetInstanceHandle();
		void SetWindowHandle(HWND handle);
		void SetInstanceHandle(HINSTANCE handle);
	private:
		HWND hWnd;
		HINSTANCE hInstance;
	};
}