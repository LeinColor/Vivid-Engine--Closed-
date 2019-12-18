#include "VividEngine.h"

using namespace vivid;

void VividEngine::Initialize()
{
	if (renderer.GetDevice() == nullptr)
		renderer.SetDevice(new DirectX11Wrapper());

	isInitialized = true;
}

void VividEngine::Run()
{
	if (!isInitialized) {
		Initialize();
	}
}

void VividEngine::SetWindow(HWND hWnd, HINSTANCE hInstance)
{
	appHandle.SetWindowHandle(hWnd);
	appHandle.SetInstanceHandle(hInstance);
}