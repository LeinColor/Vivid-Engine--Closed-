#include "VividEngine.h"
#include "GameObject.h"
#include "Texture.h"
#include "TextureShader.h"

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

	DrawScene();
}

void VividEngine::DrawScene()
{
	GameObject camera;
	camera.AddComponent<Transform>();
	camera.AddComponent<Camera>();

	GameObject sprite;
	renderer.GetDevice()->BeginScene(0.0f, 0.0f, 0.0f, 1.0f);
}

void VividEngine::SetWindow(HWND hWnd, HINSTANCE hInstance)
{
	appHandle.SetWindowHandle(hWnd);
	appHandle.SetInstanceHandle(hInstance);
}