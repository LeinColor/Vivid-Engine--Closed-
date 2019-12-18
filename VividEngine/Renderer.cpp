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