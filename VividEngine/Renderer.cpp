/**
	Renderer.cpp

	Purpose: Class responsible for rendering all objects.

	@author Lein
	@version 1.0 12/20/19
*/
#include "Renderer.h"

using namespace vivid;

DirectX11Wrapper* Renderer::GetDevice()
{
	return device;
}

void Renderer::SetDevice(DirectX11Wrapper* value)
{
	device = value;
}