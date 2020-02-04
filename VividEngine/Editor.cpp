#include "stdafx.h"
#include "GraphicsAPI.h"
#include "Renderer.h"
#include "Editor.h"
#include "Time.h"
#include "Input.h"
#include "Importer.h"

#include "Scene.h"
#include "Components.h"

#include <unordered_map>
using namespace vivid;

void Editor::Start()
{
	Scene* scene = new Scene();
	renderer.SetScene(scene);


	//=======================================

	renderer.Apply();
}

void Editor::Initialize()
{
	// initialize renderer to load mesh, shader
	time.Initialize();
	input.Initialize();
	renderer.Initialize();
	initialized = true;
}

void Editor::Run()
{
	if (!initialized) {
		Initialize();
		Start();
	}

	time.deltaTime = float(std::max(0.0, time.Elapsed() / 1000.0));
	time.Record();
	
	deltaTimeAccumulator += time.deltaTime;
	if (deltaTimeAccumulator > 10)
	{
		deltaTimeAccumulator = 0;
	}

	const float targetFrameRateInv = 1.0f / 60.0f;

	// call function for frame per second
	if (deltaTimeAccumulator >= targetFrameRateInv)
	{
		renderer.Render();
	}
	// call function for frame per second, but if lag occurred it will be calculated for skipped time line.
	while (deltaTimeAccumulator >= targetFrameRateInv)
	{
		FixedUpdate();	// calculate physics here
		//Update();
		deltaTimeAccumulator -= targetFrameRateInv;
	}
	Update();
}

void Editor::FixedUpdate()
{
}

void Editor::Update()
{
}

void Editor::SetWindow(HWND hWnd, HINSTANCE hInstance)
{
	AppHandle::SetWindowHandle(hWnd);
	AppHandle::SetInstanceHandle(hInstance);
}