/**
	Editor.h

	Purpose: calls all of core function in here.

	@author Lein
	@version 1.0 12/20/19
*/
#pragma once
#include "../VividEngine/AppHandle.h"
#include "../VividEngine/Input.h"
#include "../VividEngine/Time.h"
#include "../VividEngine/Renderer.h"

using namespace vivid;

class Entrance {
private:
	void Initialize();

public:
	void Run();
	void Start();
	void SetWindow(HWND hWnd, HINSTANCE hInstance);
	void Update();
	void FixedUpdate();

private:
	Input	 input;
	Time	 time;
	Renderer renderer;
	bool	 initialized = false;

	float	 deltaTimeAccumulator = 0;
};
