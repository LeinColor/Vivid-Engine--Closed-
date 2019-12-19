#include "VividEngine.h"

using namespace vivid;

void VividEngine::Initialize()
{
	if (renderer.GetDevice() == nullptr)
		renderer.SetDevice(new DirectX11Wrapper(appHandle.GetWindowHandle(), false));

	initialized = true;
}

void VividEngine::Run()
{
	if (!initialized) {
		Initialize();
	}
}

void VividEngine::SetWindow(HWND hWnd, HINSTANCE hInstance)
{
	appHandle.SetWindowHandle(hWnd);
	appHandle.SetInstanceHandle(hInstance);
}