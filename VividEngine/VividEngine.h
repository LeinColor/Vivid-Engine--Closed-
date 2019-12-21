/**
	VividEngine.h

	Purpose: Class responsible for rendering all objects.

	@author Lein
	@version 1.0 12/20/19
*/
#pragma once
#include "AppHandle.h"
#include "Renderer.h"

class TextureShader;

namespace vivid {
	class VividEngine {
	private:
		void Initialize();

	public:
		void Run();
		void SetWindow(HWND hWnd, HINSTANCE hInstance);
		void DrawScene();

	private:
		AppHandle appHandle;
		Renderer renderer;
		bool initialized = false;

		TextureShader ts;
	};
}