#pragma once
#include "stdafx.h"

namespace vivid {
	class Handle {
	public:
		static HWND GetWindowHandle();
		static HINSTANCE GetInstanceHandle();
		static void SetWindowHandle(HWND handle);
		static void SetInstanceHandle(HINSTANCE handle);
	private:
		static HWND hWnd;
		static HINSTANCE hInstance;
	};
}