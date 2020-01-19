/**
	Editor.h

	Purpose: calls all of core function in here.

	@author Lein
	@version 1.0 12/20/19
*/
#pragma once
#include "AppHandle.h"
#include "Input.h"
#include "Renderer.h"

using namespace vivid;

	class Editor {
	private:
		void Initialize();

	public:
		void Run();
		void Start();
		void SetWindow(HWND hWnd, HINSTANCE hInstance);
		void Update();
		void FixedUpdate();

	private:
		Input m_Input;
		Renderer m_Renderer;
		bool m_Initialized = false;
		
		float m_DeltaTimeAccumulator = 0;
	};
