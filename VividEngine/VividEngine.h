#pragma once
#include "AppHandle.h"
#include "Renderer.h"

namespace vivid {
	class VividEngine {
	private:
		void Initialize();

	public:
		void Run();
		void SetWindow(HWND hWnd, HINSTANCE hInstance);

	private:
		AppHandle appHandle;
		Renderer renderer;
		bool isInitialized = false;
	};
}