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
#include "../VividEngine/Scene.h"

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
	Scene*   currentScene;
	bool	 initialized = false;

	float	 deltaTimeAccumulator = 0;
};
