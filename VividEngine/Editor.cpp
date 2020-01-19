#include "stdafx.h"
#include "Renderer.h"
#include "Editor.h"
#include "Time.h"
#include "Input.h"

#include <unordered_map>
using namespace vivid;

void Editor::Start()
{

}

void Editor::Initialize()
{
	m_Renderer.Initialize();

	// initialize renderer to load mesh, shader
	Time::Initialize();
	m_Input.Initialize();
	m_Initialized = true;
}

void Editor::Run()
{
	if (!m_Initialized) {
		Initialize();
		Start();
	}

	Time::deltaTime = float(std::max(0.0, Time::Elapsed() / 1000.0));
	Time::Record();

	m_DeltaTimeAccumulator += Time::deltaTime;
	if (m_DeltaTimeAccumulator > 10)
	{
		m_DeltaTimeAccumulator = 0;
	}

	const float targetFrameRateInv = 1.0f / 60.0f;

	// call function for frame per second
	if (m_DeltaTimeAccumulator >= targetFrameRateInv)
	{
		m_Renderer.Render();
	}
	// call function for frame per second, but if lag occurred it will be calculated for skipped time line.
	while (m_DeltaTimeAccumulator >= targetFrameRateInv)
	{
		FixedUpdate();	// calculate physics here
		//Update();
		m_DeltaTimeAccumulator -= targetFrameRateInv;
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