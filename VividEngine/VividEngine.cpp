#include "VividEngine.h"
#include "GameObject.h"
#include "Texture.h"
#include "TextureShader.h"

using namespace vivid;

void VividEngine::Initialize()
{
	if (renderer.GetDevice() == nullptr)
		renderer.SetDevice(new DirectX11Wrapper(appHandle.GetWindowHandle(), false));
	
	GameObject camera;
	//objectManager.Add(camera);
	/*
	GameObject* sprite = new GameObject();
	sprite->AddComponent<Transform>();
	sprite->AddComponent<SpriteRenderer>();
	objectManager.Add(sprite);

	renderer.Initialize(objectManager);
	*/
	initialized = true;
}

void VividEngine::Run()
{
	if (!initialized) {
		Initialize();
	}

	Update();
}

void VividEngine::Update()
{
	static float timer = 0.0f;

	timer += 0.01f;

	if (timer >= 1.0f) {
		timer = 0;

		// Render all of 2D objects
	//	renderer.Render(objectManager);
	}
}

void VividEngine::SetWindow(HWND hWnd, HINSTANCE hInstance)
{
	appHandle.SetWindowHandle(hWnd);
	appHandle.SetInstanceHandle(hInstance);
}