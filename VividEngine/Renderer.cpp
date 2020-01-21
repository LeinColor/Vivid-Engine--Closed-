/**
	Renderer.cpp

	Purpose: Class responsible for rendering all objects.

	@author Lein
	@version 1.0 12/20/19
*/
#include "stdafx.h"
#include "Renderer.h"
#include "Descriptor.h"
#include "Debug.h"
using namespace vivid;
using namespace std;


void Renderer::Initialize()
{
	graphics = new GraphicsAPI(AppHandle::GetWindowHandle(), false);
}

void Renderer::Shutdown()
{}

void Renderer::Render()
{
	graphics->BeginScene(0.0f, 0.0f, 0.0f, 1.0f);

	graphics->EndScene();
}